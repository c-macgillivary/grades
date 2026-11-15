"""Integration tests for the implemented JSON dump command.

Run from the repository root:
    python3 tests/test_dump.py
"""

import os

from UnitTest import TestRunner

EXECUTABLE = os.environ.get("GRADES_EXECUTABLE", "./build/bin/release/grades")


def main() -> int:
    with TestRunner(EXECUTABLE) as test:
        test.run_many(
            [
                (("Empty dump is an empty JSON object", "dump"),
                 {"expected_stdout": "{}\n"}),
                (("Empty dump works through alias", "d"),
                 {"expected_stdout": "{}\n"}),
                (("Dump accepts no positional arguments", "dump"),
                 {"expected_stdout": "{}\n"}),
                (("Dump rejects an extra positional argument", "dump", "extra"),
                 {"error": True, "expected_returncode": 109}),
                (("Dump alias rejects an extra positional argument", "d", "extra"),
                 {"error": True, "expected_returncode": 109}),

                (("Create a course", "c", "a", "CS145"),
                 {"stdout_contains": "Successfully added course CS145"}),
                (("Dump includes a course keyed by its code", "dump"),
                 {"stdout_contains": ('"CS145"', '"code": "CS145"', '"assessments": {}')}),
                (("Dump is read-only and does not change course state", "c", "l"),
                 {"stdout_contains": "CS145"}),

                (("Add an ungraded assessment", "a", "a", "CS145", "Quiz", "20"),
                 {"stdout_contains": "Successfully added assessment Quiz"}),
                (("Dump serializes an ungraded mark as null", "d"),
                 {"stdout_contains": ('"Quiz"', '"grade": null', '"name": "Quiz"', '"weight"', '"part": 20.0', '"total": 100.0')}),

                (("Add a graded assessment with ratio weight and ratio grade", "a", "a", "CS145", "Midterm", "25/50", "17/20"),
                 {"stdout_contains": "Successfully added assessment Midterm"}),
                (("Dump serializes the supplied grade numerator and denominator", "dump"),
                 {"stdout_contains": ('"Midterm"', '"part": 17.0', '"total": 20.0')}),
                (("Dump serializes ratio weight without normalization", "d"),
                 {"stdout_contains": ('"part": 25.0', '"total": 50.0')}),

                (("Update an assessment grade", "g", "u", "CS145", "Quiz", "88.25%"),
                 {"stdout_contains": "Added grade 88%"}),
                (("Dump reflects updated grade values", "d"),
                 {"stdout_contains": ('"Quiz"', '"part": 88.25', '"total": 100.0')}),
                (("Repeated dump produces the same serialized state", "dump"),
                 {"stdout_contains": ('"CS145"', '"Quiz"', '"Midterm"', '"part": 88.25')}),

                (("Create a second course", "c", "a", "MATH136"),
                 {"stdout_contains": "Successfully added course MATH136"}),
                (("Dump contains both courses", "d"),
                 {"stdout_occurrences": {'"CS145"': 2, '"MATH136"': 2}}),
                (("Add assessment to second course", "a", "a", "MATH136", "Assignment", "10", "9"),
                 {"stdout_contains": "Successfully added assessment Assignment"}),
                (("Dump contains assessments under their respective course objects", "dump"),
                 {"stdout_contains": ('"CS145"', '"Quiz"', '"Midterm"', '"MATH136"', '"Assignment"')}),

                (("Duplicate course add is rejected", "c", "a", "CS145"),
                 {"error": True, "stderr_contains": "Course CS145 already exists"}),
                (("Duplicate course did not duplicate the dump entry", "d"),
                 {"stdout_occurrences": {'"CS145"': 2}}),
                (("Duplicate assessment add is rejected", "a", "a", "CS145", "Quiz", "20"),
                 {"error": True, "stderr_contains": "Assessment Quiz already exists"}),
                (("Duplicate assessment did not duplicate the dump entry", "dump"),
                 {"stdout_occurrences": {'"Quiz"': 2}}),

                (("Remove one assessment", "a", "r", "CS145", "Midterm"),
                 {"stdout_contains": "Successfully removed assessment Midterm"}),
                (("Dump excludes removed assessment but retains others", "d"),
                 {"stdout_contains": ('"CS145"', '"Quiz"', '"MATH136"', '"Assignment"'),
                  "stdout_not_contains": '"Midterm"'}),
                (("Remove last assessment from first course", "a", "r", "CS145", "Quiz"),
                 {"stdout_contains": "Successfully removed assessment Quiz"}),
                (("Dump retains the course with an empty assessments object", "dump"),
                 {"stdout_contains": ('"CS145"', '"assessments": {}'), "stdout_not_contains": '"Quiz"'}),
                (("Remove the second course", "c", "r", "MATH136"),
                 {"stdout_contains": "Successfully removed course MATH136"}),
                (("Dump contains only the remaining empty course", "d"),
                 {"stdout_contains": ('"CS145"', '"assessments": {}'), "stdout_not_contains": '"MATH136"'}),
                (("Remove the final course", "c", "r", "CS145"),
                 {"stdout_contains": "Successfully removed course CS145"}),
                (("Dump returns to empty JSON after all removals", "dump"),
                 {"expected_stdout": "{}\n"}),
            ]
        )

        return test.summary()


if __name__ == "__main__":
    raise SystemExit(main())
