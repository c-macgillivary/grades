"""Small integration-test runner for the grades CLI.

Execution methods are provided for isolated and stateful commands:

* run_isolated(...): runs one command with a fresh temporary XDG_DATA_HOME.
* run_stateful(...): runs one command using shared data.
* run_many(...): runs a sequence of stateful test cases in order, each in a
  separate process but sharing the same temporary XDG_DATA_HOME.

Each call is one test. Failures are written to stderr, counted, and do not
prevent later tests from running.

Both execution modes accept input_text to simulate text entered by a user.
They also accept error=True as shorthand for expecting exit code 1. An
explicit expected_returncode takes precedence over the error shorthand.

Successful commands (expected exit code 0) must not write anything to stderr
unless expected_stderr or stderr_contains is explicitly supplied.
"""

from __future__ import annotations

import os
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any, Mapping, Sequence


class TestRunner:
    def __init__(self, executable: str | Path, timeout: float = 10.0) -> None:
        self.executable = str(executable)
        self.timeout = timeout
        self.passed = 0
        self.failed = 0
        self._stateful_temp: tempfile.TemporaryDirectory[str] | None = None
        self._closed = False

    def __enter__(self) -> "TestRunner":
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.close()

    @staticmethod
    def _resolve_returncode(
        *, error: bool, expected_returncode: int | None
    ) -> int:
        """Resolve the expected status code.

        An explicitly supplied expected_returncode wins. Otherwise, error=True
        expects 1 and the default (error=False) expects 0.
        """
        if expected_returncode is not None:
            return expected_returncode
        return 1 if error else 0

    @staticmethod
    def _normalize_expectations(
        values: str | Sequence[str],
    ) -> tuple[str, ...]:
        """Treat a single string as one substring, not as characters."""
        if isinstance(values, str):
            return (values,)
        return tuple(values)

    def _run(
        self,
        name: str,
        args: Sequence[str],
        data_home: str,
        *,
        input_text: str | None,
        expected_returncode: int,
        expected_stdout: str | None,
        expected_stderr: str | None,
        stdout_contains: str | Sequence[str],
        stderr_contains: str | Sequence[str],
        stdout_not_contains: str | Sequence[str],
        stderr_not_contains: str | Sequence[str],
        stdout_occurrences: Mapping[str, int] | None,
        stderr_occurrences: Mapping[str, int] | None,
    ) -> bool:
        command = [self.executable, *map(str, args)]
        env = os.environ.copy()
        env["XDG_DATA_HOME"] = data_home

        try:
            result = subprocess.run(
                command,
                input=input_text,
                capture_output=True,
                text=True,
                env=env,
                timeout=self.timeout,
                check=False,
            )
        except subprocess.TimeoutExpired as error:
            self._record_failure(
                name,
                command,
                [
                    f"Expected the command to finish within {self.timeout} seconds.",
                    f"It timed out. Partial stdout: {error.stdout!r}",
                    f"Partial stderr: {error.stderr!r}",
                ],
            )
            return False
        except OSError as error:
            self._record_failure(
                name,
                command,
                [
                    "Expected the executable to run successfully, "
                    f"but it could not be started: {error}"
                ],
            )
            return False

        problems: list[str] = []

        if result.returncode != expected_returncode:
            problems.append(
                f"Expected exit code {expected_returncode}, "
                f"but got {result.returncode}."
            )

        if expected_stdout is not None and result.stdout != expected_stdout:
            problems.append(
                "Expected stdout to equal:\n"
                f"  {expected_stdout!r}\n"
                "Actual stdout was:\n"
                f"  {result.stdout!r}"
            )

        if expected_stderr is not None and result.stderr != expected_stderr:
            problems.append(
                "Expected stderr to equal:\n"
                f"  {expected_stderr!r}\n"
                "Actual stderr was:\n"
                f"  {result.stderr!r}"
            )

        for expected in self._normalize_expectations(stdout_contains):
            if expected not in result.stdout:
                problems.append(
                    f"Expected stdout to contain {expected!r}, but it did not.\n"
                    f"Actual stdout was:\n  {result.stdout!r}"
                )

        for unexpected in self._normalize_expectations(stdout_not_contains):
            if unexpected in result.stdout:
                problems.append(
                    f"Expected stdout not to contain {unexpected!r}, "
                    "but it did.\n"
                    f"Actual stdout was:\n  {result.stdout!r}"
                )

        for expected in self._normalize_expectations(stderr_contains):
            if expected not in result.stderr:
                problems.append(
                    f"Expected stderr to contain {expected!r}, but it did not.\n"
                    f"Actual stderr was:\n  {result.stderr!r}"
                )

        for unexpected in self._normalize_expectations(stderr_not_contains):
            if unexpected in result.stderr:
                problems.append(
                    f"Expected stderr not to contain {unexpected!r}, "
                    "but it did.\n"
                    f"Actual stderr was:\n  {result.stderr!r}"
                )

        for expected, expected_count in (stdout_occurrences or {}).items():
            actual_count = result.stdout.count(expected)
            if actual_count != expected_count:
                problems.append(
                    f"Expected {expected!r} to occur {expected_count} time(s) "
                    f"in stdout, but it occurred {actual_count} time(s).\n"
                    f"Actual stdout was:\n  {result.stdout!r}"
                )

        for expected, expected_count in (stderr_occurrences or {}).items():
            actual_count = result.stderr.count(expected)
            if actual_count != expected_count:
                problems.append(
                    f"Expected {expected!r} to occur {expected_count} time(s) "
                    f"in stderr, but it occurred {actual_count} time(s).\n"
                    f"Actual stderr was:\n  {result.stderr!r}"
                )

        if problems:
            self._record_failure(name, command, problems)
            return False

        self.passed += 1
        print(f"\033[92mPASS\033[0m: {name}")
        return True

    def _record_failure(
        self, name: str, command: Sequence[str], problems: Sequence[str]
    ) -> None:
        self.failed += 1
        print(f"\033[91mFAIL\033[0m: {name}", file=sys.stderr)
        print(f"  Command: {shlex.join(command)}", file=sys.stderr)
        for problem in problems:
            print(f"  {problem}", file=sys.stderr)
        print(file=sys.stderr)

    def run_isolated(
        self,
        name: str,
        *args: str,
        error: bool = False,
        expected_returncode: int | None = None,
        input_text: str | None = None,
        expected_stdout: str | None = None,
        expected_stderr: str | None = None,
        stdout_contains: str | Sequence[str] = (),
        stderr_contains: str | Sequence[str] = (),
        stdout_not_contains: str | Sequence[str] = (),
        stderr_not_contains: str | Sequence[str] = (),
        stdout_occurrences: Mapping[str, int] | None = None,
        stderr_occurrences: Mapping[str, int] | None = None,
    ) -> bool:
        """Run one command with a brand-new, disposable data directory.

        By default, the command is expected to exit with status 0. Set
        error=True to expect status 1, or supply expected_returncode to check
        an exact status code (which takes precedence over error).

        Successful commands must leave stderr empty by default. Supplying
        expected_stderr or stderr_contains overrides that default check.

        Set input_text to simulate user input, for example "y\\n" to confirm
        a prompt. Set expected_stdout/expected_stderr for exact output checks;
        use *_contains and *_not_contains for partial output checks. Each
        substring argument can be either one string or a sequence of strings.
        Use stdout_occurrences/stderr_occurrences to assert exact substring
        counts, e.g. stdout_occurrences={"CS145": 1}.
        """
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        returncode = self._resolve_returncode(
            error=error, expected_returncode=expected_returncode
        )
        if (
            returncode == 0
            and expected_stderr is None
            and not stderr_contains
            and not stderr_occurrences
        ):
            expected_stderr = ""

        with tempfile.TemporaryDirectory(prefix="grades-test-isolated-") as data_home:
            return self._run(
                name,
                args,
                data_home,
                input_text=input_text,
                expected_returncode=returncode,
                expected_stdout=expected_stdout,
                expected_stderr=expected_stderr,
                stdout_contains=stdout_contains,
                stderr_contains=stderr_contains,
                stdout_not_contains=stdout_not_contains,
                stderr_not_contains=stderr_not_contains,
                stdout_occurrences=stdout_occurrences,
                stderr_occurrences=stderr_occurrences,
            )

    def run_stateful(
        self,
        name: str,
        *args: str,
        error: bool = False,
        expected_returncode: int | None = None,
        input_text: str | None = None,
        expected_stdout: str | None = None,
        expected_stderr: str | None = None,
        stdout_contains: str | Sequence[str] = (),
        stderr_contains: str | Sequence[str] = (),
        stdout_not_contains: str | Sequence[str] = (),
        stderr_not_contains: str | Sequence[str] = (),
        stdout_occurrences: Mapping[str, int] | None = None,
        stderr_occurrences: Mapping[str, int] | None = None,
    ) -> bool:
        """Run a command using data shared by all stateful calls.

        The shared temporary data directory is created on the first call and
        removed by reset_state() or close(). By default, the command must exit
        with status 0. Use error=True to expect status 1, or supply an exact
        expected_returncode (which takes precedence over error).

        Successful commands must leave stderr empty by default. Supplying
        expected_stderr or stderr_contains overrides that default check.

        Set input_text to simulate user input, for example "n\\n" to decline
        a prompt. Use *_contains and *_not_contains for partial output checks.
        Each substring argument can be either one string or a sequence of
        strings.
        """
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        returncode = self._resolve_returncode(
            error=error, expected_returncode=expected_returncode
        )
        if (
            returncode == 0
            and expected_stderr is None
            and not stderr_contains
            and not stderr_occurrences
        ):
            expected_stderr = ""

        if self._stateful_temp is None:
            self._stateful_temp = tempfile.TemporaryDirectory(
                prefix="grades-test-stateful-"
            )

        return self._run(
            name,
            args,
            self._stateful_temp.name,
            input_text=input_text,
            expected_returncode=returncode,
            expected_stdout=expected_stdout,
            expected_stderr=expected_stderr,
            stdout_contains=stdout_contains,
            stderr_contains=stderr_contains,
            stdout_not_contains=stdout_not_contains,
            stderr_not_contains=stderr_not_contains,
            stdout_occurrences=stdout_occurrences,
            stderr_occurrences=stderr_occurrences,
        )

    def run_many(
        self,
        test_cases: Sequence[tuple[Sequence[str], Mapping[str, Any]]],
    ) -> bool:
        """Run multiple stateful tests in order.

        Each test case is a pair of ``(positional_args, keyword_options)``.
        The positional arguments are exactly what would be passed to
        ``run_stateful`` (the test name first, followed by CLI arguments).
        The keyword options are the assertions and settings for that test.

        Every case launches a separate process, but all cases share the same
        temporary data directory. Tests continue running after a failure.
        Returns True only if every test case passed.

        Example::

            test.run_many([
                (("List courses", "c", "l"),
                 {"stdout_contains": "no courses"}),
                (("Add course", "c", "a", "CS145"), {}),
            ])
        """
        all_passed = True

        for index, test_case in enumerate(test_cases, start=1):
            if not isinstance(test_case, (tuple, list)) or len(test_case) != 2:
                raise TypeError(
                    f"Test case {index} must be a pair of "
                    "(positional_args, keyword_options)"
                )

            positional_args, keyword_options = test_case

            if (
                isinstance(positional_args, (str, bytes))
                or not isinstance(positional_args, Sequence)
                or not positional_args
                or not all(isinstance(arg, str) for arg in positional_args)
            ):
                raise TypeError(
                    f"Test case {index} positional arguments must be a non-empty "
                    "sequence of strings, beginning with the test name"
                )

            if not isinstance(keyword_options, Mapping):
                raise TypeError(
                    f"Test case {index} keyword options must be a mapping"
                )

            if not all(isinstance(key, str) for key in keyword_options):
                raise TypeError(
                    f"Test case {index} contains a non-string keyword name"
                )

            passed = self.run_stateful(*positional_args, **keyword_options)
            if not passed:
                all_passed = False

        return all_passed

    def reset_state(self) -> None:
        """Discard stateful test data; the next stateful call starts fresh."""
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        if self._stateful_temp is not None:
            self._stateful_temp.cleanup()
            self._stateful_temp = None

    def summary(self) -> int:
        """Print totals and return number of failed tests."""
        total = self.passed + self.failed
        print("\nTest summary:")
        print(f"  Passed: {self.passed}")
        print(f"  Failed: {self.failed}")
        print(f"  Total:  {total}")
        return self.failed

    def close(self) -> None:
        """Remove shared temporary data. Safe to call more than once."""
        if self._stateful_temp is not None:
            self._stateful_temp.cleanup()
            self._stateful_temp = None
        self._closed = True
