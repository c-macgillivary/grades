import os

from UnitTest import TestRunner

EXECUTABLE = os.environ.get("GRADES_EXECUTABLE", "./build/bin/release/grades")


def main() -> int:
    with TestRunner(EXECUTABLE) as test:
        test.run_stateful(
            "List courses before adding",
            "c",
            "l",
            stdout_contains=("no courses"),
        )

        test.run_stateful(
            "Add course with empty code",
            "c",
            "a",
            "",
            error=True,
        )

        test.run_stateful(
            "Add course with whitespace-only code",
            "c",
            "a",
            "   ",
            error=True,
        )

        test.run_stateful(
            "Add course without a code",
            "c",
            "a",
            error=True,
            expected_returncode=106,
        )

        test.run_stateful(
            "Remove course without a code",
            "c",
            "r",
            error=True,
            expected_returncode=106,
        )

        test.run_stateful(
            "Use invalid course subcommand",
            "c",
            "invalid",
            error=True,
            expected_returncode=106,
        )

        test.run_stateful(
            "Run course command without a subcommand",
            "c",
            error=True,
            expected_returncode=106,
        )

        test.run_stateful(
            "List courses with unexpected argument",
            "c",
            "l",
            "unexpected",
            error=True,
            expected_returncode=109,
        )

        test.run_stateful(
            "Remove course when no courses exist",
            "c",
            "r",
            "missing101",
            error=True,
        )

        test.run_stateful(
            "List courses after failed removal",
            "c",
            "l",
            stdout_contains=("no courses",),
        )

        test.run_stateful(
            "Add new Course",
            "c",
            "a",
            "test101",
            stdout_contains=("Success", "add", "test101"),
        )

        test.run_stateful(
            "List courses after adding",
            "c",
            "l",
            stdout_contains=("test101",),
        )

        test.run_stateful(
            "Add existing course",
            "c",
            "a",
            "test101",
            error=True,
            stderr_contains=("Error", "Could not add", "test101", "already exists"),
        )

        test.run_stateful(
            "List courses contains test101 exactly once",
            "c",
            "l",
            stdout_occurrences={"test101": 1},
        )

        test.run_stateful(
            "Add additional course",
            "c",
            "a",
            "test102",
            stdout_contains=("Success", "add", "test102"),
        )

        test.run_stateful(
            "List after adding additional course",
            "c",
            "l",
            stdout_contains=("test101", "test102"),
        )

        test.run_stateful(
            "Re-add additional course",
            "c",
            "a",
            "test102",
            error=True,
            stderr_contains=("Error", "Could not add", "test102", "already exists"),
        )

        test.run_stateful(
            "List courses after re-adding additional course",
            "c",
            "l",
            stdout_occurrences={"test101": 1, "test102": 1},
        )

        test.run_stateful(
            "Remove non-existent course",
            "c",
            "r",
            "missing101",
            error=True,
        )

        test.run_stateful(
            "List courses after attempting to remove non-existent course",
            "c",
            "l",
            stdout_occurrences={"test101": 1, "test102": 1},
        )

        test.run_stateful(
            "Remove first course",
            "c",
            "r",
            "test101",
            stdout_contains=("Success", "remove", "test101"),
        )

        test.run_stateful(
            "List after removing",
            "c",
            "l",
            stdout_contains=("test102",),
            stdout_not_contains=("test101",),
        )

        test.run_stateful(
            "Add removed course",
            "c",
            "a",
            "test101",
            stdout_contains=("Success", "add", "test101"),
        )

        test.run_stateful(
            "List after adding removed course",
            "c",
            "l",
            stdout_contains=("test101", "test102"),
        )

        test.run_stateful(
            "Re-add removed course",
            "c",
            "a",
            "test101",
            error=True,
            stderr_contains=("Error", "Could not add", "test101", "already exists"),
        )

        test.run_stateful(
            "List after re-adding removed course",
            "c",
            "l",
            stdout_contains=("test101", "test102"),
            stdout_occurrences={"test101": 1},
        )

        test.run_stateful(
            "Re-remove course",
            "c",
            "r",
            "test101",
            stdout_contains=("Success", "remove", "test101"),
        )

        test.run_stateful(
            "Remove other course",
            "c",
            "r",
            "test102",
            stdout_contains=("Success", "remove", "test102"),
        )

        test.run_stateful(
            "List courses after removing",
            "c",
            "l",
            stdout_contains=("no courses"),
        )

    return 0 if test.summary() else 1


if __name__ == "__main__":
    raise SystemExit(main())
