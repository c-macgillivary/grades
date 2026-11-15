import os

from UnitTest import TestRunner

EXECUTABLE = os.environ.get("GRADES_EXECUTABLE", "./build/bin/release/grades")


def main() -> int:
    with TestRunner(EXECUTABLE) as test:
        test.run_many(
            [
                (("List courses before adding", "c", "l"),
                 {"stdout_contains": "no courses"}),

                (("Add course with empty code", "c", "a", ""),
                 {"error": True}),

                (("Add course with whitespace-only code", "c", "a", "   "),
                 {"error": True}),

                (("Add course without a code", "c", "a"),
                 {"error": True, "expected_returncode": 106}),

                (("Remove course without a code", "c", "r"),
                 {"error": True, "expected_returncode": 106}),

                (("Use invalid course subcommand", "c", "invalid"),
                 {"error": True, "expected_returncode": 106}),

                (("Run course command without a subcommand", "c"),
                 {"error": True, "expected_returncode": 106}),

                (("List courses with unexpected argument", "c", "l", "unexpected"),
                 {"error": True, "expected_returncode": 109}),

                (("Remove course when no courses exist", "c", "r", "missing101"),
                 {"error": True}),

                (("List courses after failed removal", "c", "l"),
                 {"stdout_contains": "no courses"}),

                (("Add new course", "c", "a", "test101"),
                 {"stdout_contains": ("Success", "add", "test101")}),

                (("List courses after adding", "c", "l"),
                 {"stdout_contains": "test101"}),

                (("Add existing course", "c", "a", "test101"),
                 {
                     "error": True,
                     "stderr_contains": ("Error", "Could not add", "test101", "already exists"),
                 }),

                (("List courses contains test101 exactly once", "c", "l"),
                 {"stdout_occurrences": {"test101": 1}}),

                (("Add additional course", "c", "a", "test102"),
                 {"stdout_contains": ("Success", "add", "test102")}),

                (("List after adding additional course", "c", "l"),
                 {"stdout_contains": ("test101", "test102")}),

                (("Re-add additional course", "c", "a", "test102"),
                 {
                     "error": True,
                     "stderr_contains": ("Error", "Could not add", "test102", "already exists"),
                 }),

                (("List courses after re-adding additional course", "c", "l"),
                 {"stdout_occurrences": {"test101": 1, "test102": 1}}),

                (("Remove non-existent course", "c", "r", "missing101"),
                 {"error": True}),

                (("List courses after attempting to remove non-existent course",
                  "c", "l"),
                 {"stdout_occurrences": {"test101": 1, "test102": 1}}),

                (("Remove first course", "c", "r", "test101"),
                 {"stdout_contains": ("Success", "remove", "test101")}),

                (("List after removing", "c", "l"),
                 {"stdout_contains": "test102", "stdout_not_contains": "test101"}),

                (("Add removed course", "c", "a", "test101"),
                 {"stdout_contains": ("Success", "add", "test101")}),

                (("List after adding removed course", "c", "l"),
                 {"stdout_contains": ("test101", "test102")}),

                (("Re-add removed course", "c", "a", "test101"),
                 {
                     "error": True,
                     "stderr_contains": ("Error", "Could not add", "test101", "already exists"),
                 }),

                (("List after re-adding removed course", "c", "l"),
                 {
                     "stdout_contains": ("test101", "test102"),
                     "stdout_occurrences": {"test101": 1},
                 }),

                (("Re-remove course", "c", "r", "test101"),
                 {"stdout_contains": ("Success", "remove", "test101")}),

                (("Remove other course", "c", "r", "test102"),
                 {"stdout_contains": ("Success", "remove", "test102")}),

                (("List courses after removing", "c", "l"),
                 {"stdout_contains": "no courses"}),
            ]
        )

        return 0 if test.summary() else 1


if __name__ == "__main__":
    raise SystemExit(main())
