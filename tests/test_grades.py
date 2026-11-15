"""Example integration tests for the grades CLI.

Run from the repository root after building the executable:
    python3 test_grades.py

Replace the example expected messages with the exact output from your CLI.
"""

from UnitTest import TestRunner

import os

EXECUTABLE = os.environ.get(
    "GRADES_EXECUTABLE",
    "./build/bin/release/grades"
)

def main() -> int:
    with TestRunner(EXECUTABLE) as tests:
        # ISOLATED: this command gets a fresh, empty data directory that is
        # deleted immediately after the command finishes.
        tests.run_isolated(
            "Help displays usage information",
            "--help",
            expected_returncode=0,
            stdout_contains=("OPTIONS",),
        )

        # STATEFUL: these commands all share one temporary data directory.
        # The course added by the first command should still exist for the
        # following command.
        tests.run_stateful(
            "Add CS145 course",
            "c", "a", "CS145",
            expected_returncode=0,
            stdout_contains=("CS145",),
        )

        tests.run_stateful(
            "List courses includes CS145",
            "c", "l",
            expected_returncode=0,
            stdout_contains=("CS145",),
        )

        # Resetting removes the shared data. The next stateful command starts
        # with an empty database, but the pass/fail counters are not reset.
        tests.reset_state()

        tests.run_stateful(
            "Course list is empty after reset",
            "c", "l",
            expected_returncode=0,
            # Replace this with the actual empty-list output from your CLI.
            stdout_contains=("There are no courses",),
        )

        all_passed = tests.summary()

    # A nonzero process exit makes this script suitable for CTest and CI.
    return 0 if all_passed else 1


if __name__ == "__main__":
    raise SystemExit(main())
