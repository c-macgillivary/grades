"""Small integration-test runner for the grades CLI.

Two execution modes are provided:

* run_isolated(...): runs one command with a fresh temporary XDG_DATA_HOME.
  The temporary directory is removed as soon as that command finishes.
* run_stateful(...): runs a command using a shared temporary XDG_DATA_HOME.
  Consecutive stateful calls see the same data until reset_state() or close().

Each call is one test. Failures are written to stderr, counted, and do not
prevent later tests from running.
"""

from __future__ import annotations

import os
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Sequence


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
        error: bool,
        expected_returncode: int | None,
    ) -> int:
        if expected_returncode is not None:
            return expected_returncode

        return 1 if error else 0

    def _run(
        self,
        name: str,
        args: Sequence[str],
        data_home: str,
        *,
        expected_returncode: int,
        expected_stdout: str | None,
        expected_stderr: str | None,
        stdout_contains: Sequence[str],
        stderr_contains: Sequence[str],
        input_text: str | None,
    ) -> bool:
        command = [self.executable, *map(str, args)]
        env = os.environ.copy()
        env["XDG_DATA_HOME"] = data_home

        try:
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                input=input_text,
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
                    f"Expected the executable to run successfully, "
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

        for expected in stdout_contains:
            if expected not in result.stdout:
                problems.append(
                    f"Expected stdout to contain {expected!r}, but it did not.\n"
                    f"Actual stdout was:\n  {result.stdout!r}"
                )

        for expected in stderr_contains:
            if expected not in result.stderr:
                problems.append(
                    f"Expected stderr to contain {expected!r}, but it did not.\n"
                    f"Actual stderr was:\n  {result.stderr!r}"
                )

        if problems:
            self._record_failure(name, command, problems)
            return False

        self.passed += 1
        print(f"\033[92mPASS\033[0m: {name}")
        return True

    def _record_failure(
        self,
        name: str,
        command: Sequence[str],
        problems: Sequence[str],
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
        expected_stdout: str | None = None,
        expected_stderr: str | None = None,
        stdout_contains: Sequence[str] = (),
        stderr_contains: Sequence[str] = (),
        input_text: str | None = None,
    ) -> bool:
        """Run one command with a brand-new, disposable data directory.

        By default, expects exit code 0.

        Set error=True to expect exit code 1.

        Set expected_returncode to explicitly specify another exit code.
        An explicitly specified return code takes precedence over error.

        Use input_text to simulate user input to the command.

        Set expected_stdout/expected_stderr to an exact string when
        exact formatting matters. Use *_contains for partial output checks.
        """
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        returncode = self._resolve_returncode(error, expected_returncode)

        with tempfile.TemporaryDirectory(
            prefix="grades-test-isolated-"
        ) as data_home:
            return self._run(
                name,
                args,
                data_home,
                expected_returncode=returncode,
                expected_stdout=expected_stdout,
                expected_stderr=expected_stderr,
                stdout_contains=stdout_contains,
                stderr_contains=stderr_contains,
                input_text=input_text,
            )

    def run_stateful(
        self,
        name: str,
        *args: str,
        error: bool = False,
        expected_returncode: int | None = None,
        expected_stdout: str | None = None,
        expected_stderr: str | None = None,
        stdout_contains: Sequence[str] = (),
        stderr_contains: Sequence[str] = (),
        input_text: str | None = None,
    ) -> bool:
        """Run a command using data shared by all stateful calls.

        The shared temporary data directory is created on the first call and
        removed by reset_state() or close().

        By default, expects exit code 0.

        Set error=True to expect exit code 1.

        Set expected_returncode to explicitly specify another exit code.
        An explicitly specified return code takes precedence over error.

        Use input_text to simulate user input to the command.
        """
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        returncode = self._resolve_returncode(error, expected_returncode)

        if self._stateful_temp is None:
            self._stateful_temp = tempfile.TemporaryDirectory(
                prefix="grades-test-stateful-"
            )

        return self._run(
            name,
            args,
            self._stateful_temp.name,
            expected_returncode=returncode,
            expected_stdout=expected_stdout,
            expected_stderr=expected_stderr,
            stdout_contains=stdout_contains,
            stderr_contains=stderr_contains,
            input_text=input_text,
        )

    def reset_state(self) -> None:
        """Discard stateful test data; the next stateful call starts fresh."""
        if self._closed:
            raise RuntimeError("TestRunner is already closed")

        if self._stateful_temp is not None:
            self._stateful_temp.cleanup()
            self._stateful_temp = None

    def summary(self) -> bool:
        """Print totals and return True only when every test passed."""
        total = self.passed + self.failed

        print("\nTest summary:")
        print(f"  Passed: {self.passed}")
        print(f"  Failed: {self.failed}")
        print(f"  Total:  {total}")

        return self.failed == 0

    def close(self) -> None:
        """Remove shared temporary data. Safe to call more than once."""
        if self._stateful_temp is not None:
            self._stateful_temp.cleanup()
            self._stateful_temp = None

        self._closed = True
