"""Integration tests for the implemented interactive clean command.

Run from the repository root:
    python3 tests/test_clean.py
"""

import os

from UnitTest import TestRunner

EXECUTABLE = os.environ.get("GRADES_EXECUTABLE", "./build/bin/release/grades")
PROMPT = 'This will delete ALL stored data. Are you sure? Type "clean" to confirm: '


def main() -> int:
    with TestRunner(EXECUTABLE) as test:
        test.run_many(
            [
                (("Clean command rejects unexpected arguments", "clean", "extra"),
                 {"error": True, "expected_returncode": 109}),
                (("Create a course before clean tests", "c", "a", "CS145"),
                 {"stdout_contains": "Successfully added course CS145"}),
                (("Add an assessment before clean tests", "a", "a", "CS145", "Quiz", "20", "80"),
                 {"stdout_contains": "Successfully added assessment Quiz"}),

                # Every response other than the exact token "clean" declines.
                (("Declining with no stdin preserves data", "clean"),
                 {"input_text": "", "expected_stdout": PROMPT}),
                (("Data survives EOF decline", "d"),
                 {"stdout_contains": ('"CS145"', '"Quiz"')}),
                (("Declining with another word preserves data", "clean"),
                 {"input_text": "no\n", "expected_stdout": PROMPT}),
                (("Data survives ordinary decline", "c", "l"),
                 {"stdout_contains": "CS145"}),
                (("Confirmation is case-sensitive", "clean"),
                 {"input_text": "CLEAN\n", "expected_stdout": PROMPT}),
                (("Data survives uppercase response", "d"),
                 {"stdout_contains": '"Quiz"'}),
                (("Leading whitespace does not confirm", "clean"),
                 {"input_text": " clean\n", "expected_stdout": PROMPT}),
                (("Trailing whitespace does not confirm", "clean"),
                 {"input_text": "clean \n", "expected_stdout": PROMPT}),
                (("Carriage return remains part of the response", "clean"),
                 {"input_text": "clean\r\n", "expected_stdout": PROMPT}),
                (("Data survives whitespace-modified confirmation attempts", "c", "l"),
                 {"stdout_contains": "CS145"}),

                # Exact confirmation deletes the backing file; a later invocation
                # loads empty state and save() does not recreate it.
                (("Exact lowercase clean confirms deletion", "clean"),
                 {"input_text": "clean\n", "stdout_contains": (PROMPT, "Successfully deleted ALL stored data at ")}),
                (("Course list is empty after clean", "c", "l"),
                 {"stdout_contains": "There are no courses saved", "stdout_not_contains": "CS145"}),
                (("Dump is empty after clean", "dump"),
                 {"expected_stdout": "{}\n"}),
                (("Assessment data was also deleted", "g", "s", "CS145", "Quiz"),
                 {"error": True, "expected_stdout": "", "expected_stderr": "Could not show grade\nError: Course CS145 does not exist\n"}),
                (("Clean can be confirmed while already empty", "clean"),
                 {"input_text": "clean\n", "stdout_contains": (PROMPT, "Successfully deleted ALL stored data at ")}),
                (("Empty database remains empty after second clean", "d"),
                 {"expected_stdout": "{}\n"}),

                # The application remains usable after cleaning.
                (("Add a new course after clean", "c", "a", "MATH136"),
                 {"stdout_contains": "Successfully added course MATH136"}),
                (("New data persists after clean", "c", "l"),
                 {"stdout_contains": "MATH136", "stdout_not_contains": "CS145"}),
                (("New database can be cleaned again", "clean"),
                 {"input_text": "clean\n", "stdout_contains": "Successfully deleted ALL stored data at "}),
                (("Database is empty after final clean", "dump"),
                 {"expected_stdout": "{}\n"}),
            ]
        )

        return test.summary()


if __name__ == "__main__":
    raise SystemExit(main())
