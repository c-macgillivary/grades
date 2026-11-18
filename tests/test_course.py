"""Integration tests for every currently implemented course operation.

Run from the repository root:
    python3 tests/test_course.py

Set GRADES_EXECUTABLE to override the executable path.
"""

import os

from UnitTest import TestRunner

EXECUTABLE = os.environ.get("GRADES_EXECUTABLE", "./build/bin/release/grades")
EMPTY_LIST = (
    'There are no courses saved.\n'
    'Add one with "grades course add <course_name>"\n'
)


def main() -> int:
    with TestRunner(EXECUTABLE) as test:
        test.run_many(
            [
                # Empty state and command/argument parsing.
                (("List courses in an empty database", "course", "list"),
                 {"expected_stdout": EMPTY_LIST}),
                (("List empty courses through aliases", "c", "l"),
                 {"expected_stdout": EMPTY_LIST}),
                (("Course parent requires a subcommand", "course"),
                 {"error": True, "expected_returncode": 106}),
                (("Course parent alias requires a subcommand", "c"),
                 {"error": True, "expected_returncode": 106}),
                (("Unknown course subcommand is rejected", "c", "not-a-command"),
                 {"error": True, "expected_returncode": 106}),
                (("Add requires a course code", "course", "add"),
                 {"error": True, "expected_returncode": 106}),
                (("Remove requires a course code", "course", "remove"),
                 {"error": True, "expected_returncode": 106}),
                (("List rejects an unexpected positional argument", "c", "l", "extra"),
                 {"error": True, "expected_returncode": 109}),
                (("Add rejects an unexpected extra argument", "c", "a", "CS145", "extra"),
                 {"error": True, "expected_returncode": 109}),
                (("Remove rejects an unexpected extra argument", "c", "r", "CS145", "extra"),
                 {"error": True, "expected_returncode": 109}),

                # Add validation: only all-whitespace codes are rejected.
                (("Add rejects an empty course code", "c", "a", ""),
                 {"error": True, "stderr_contains": "is an invalid course code"}),
                (("Add rejects a spaces-only course code", "c", "a", "   "),
                 {"error": True, "stderr_contains": "is an invalid course code"}),
                (("Add rejects all common whitespace characters", "c", "a", " \t\n\r\v\f"),
                 {"error": True, "stderr_contains": "is an invalid course code"}),
                (("Failed whitespace additions do not create courses", "c", "l"),
                 {"expected_stdout": EMPTY_LIST}),
                (("Add accepts a course code with leading and trailing spaces", "c", "a", " CS145 "),
                 {"stdout_contains": "Successfully added course  CS145 "}),
                (("Add accepts an ordinary course code", "course", "add", "MATH136"),
                 {"stdout_contains": "Successfully added course MATH136"}),
                (("Add accepts punctuation as part of a course code", "c", "a", "CS-145/1"),
                 {"stdout_contains": "Successfully added course CS-145/1"}),
                (("Add accepts a numeric-only course code", "c", "a", "101"),
                 {"stdout_contains": "Successfully added course 101"}),
                (("Course listing includes all distinct added codes", "course", "list"),
                 {"stdout_contains": (" CS145 ", "MATH136", "CS-145/1", "101"),
                  "stdout_occurrences": {"MATH136": 1, "CS-145/1": 1, "101": 1}}),
                (("Course codes are case-sensitive", "c", "a", "math136"),
                 {"stdout_contains": "Successfully added course math136"}),
                (("Case-distinct course codes both appear", "c", "l"),
                 {"stdout_occurrences": {"MATH136": 1, "math136": 1}}),
                (("Duplicate course code is rejected", "course", "add", "MATH136"),
                 {"error": True,
                  "stderr_contains": ("Could not add course MATH136", "Course MATH136 already exists")}),
                (("Duplicate addition leaves one copy in the list", "c", "l"),
                 {"stdout_occurrences": {"MATH136": 1}}),
                (("A code with different whitespace is a different key", "c", "a", "MATH136 "),
                 {"stdout_contains": "Successfully added course MATH136 "}),
                (("Leading/trailing spaces are preserved exactly", "c", "l"),
                 {"stdout_contains": ("MATH136\n", "MATH136 \n", " CS145 \n")}),

                # Remove behaviour and its lack of add-style whitespace validation.
                (("Removing an unknown course fails", "course", "remove", "PHYS111"),
                 {"error": True,
                  "stderr_contains": ("Could not remove course PHYS111", "Course PHYS111 does not exist")}),
                (("Removing an empty code is a not-found error", "c", "r", ""),
                 {"error": True, "stderr_contains": "Course  does not exist"}),
                (("Removing a whitespace-only code is a not-found error", "c", "r", "   "),
                 {"error": True, "stderr_contains": "Course     does not exist"}),
                (("Failed removals do not change the course list", "c", "l"),
                 {"stdout_occurrences": {"MATH136\n": 1, "MATH136 \n": 1,
                                          "math136": 1, "CS-145/1": 1, "101": 1}}),
                (("Remove a course using the short alias", "c", "r", "MATH136"),
                 {"stdout_contains": "Successfully removed course MATH136"}),
                (("Removed course is absent but other codes remain", "c", "l"),
                 {"stdout_not_contains": "MATH136\n",
                  "stdout_contains": ("MATH136 \n", "math136", "CS-145/1", "101")}),
                (("Removing the same course twice fails", "c", "r", "MATH136"),
                 {"error": True, "stderr_contains": "Course MATH136 does not exist"}),
                (("A removed code can be added again", "course", "add", "MATH136"),
                 {"stdout_contains": "Successfully added course MATH136"}),
                (("Re-added code coexists with the distinct trailing-space code", "c", "l"),
                 {"stdout_occurrences": {"MATH136\n": 1, "MATH136 \n": 1}}),
                (("Remove a code containing surrounding spaces exactly", "c", "r", " CS145 "),
                 {"stdout_contains": "Successfully removed course  CS145 "}),
                (("Remove trailing-space variant", "course", "remove", "MATH136 "),
                 {"stdout_contains": "Successfully removed course MATH136 "}),
                (("Remove punctuation-containing code", "c", "r", "CS-145/1"),
                 {"stdout_contains": "Successfully removed course CS-145/1"}),
                (("Remove numeric code", "course", "remove", "101"),
                 {"stdout_contains": "Successfully removed course 101"}),
                (("Remove case-distinct code", "c", "r", "math136"),
                 {"stdout_contains": "Successfully removed course math136"}),
                (("Remove final remaining course", "course", "remove", "MATH136"),
                 {"stdout_contains": "Successfully removed course MATH136"}),
                (("List is empty after removing every course", "c", "l"),
                 {"expected_stdout": EMPTY_LIST}),
            ]
        )

        return test.summary()


if __name__ == "__main__":
    raise SystemExit(main())
