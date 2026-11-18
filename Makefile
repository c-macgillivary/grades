.PHONY: all configure debug release core test-verbose test install clean

all: debug

configure: build/.configured

CMAKE_FILES = $(shell find . -name CMakeLists.txt)

build/.configured: $(CMAKE_FILES) CMakePresets.json
	cmake --preset default
	@touch $@

debug: configure
	cmake --build --preset debug --parallel

release: configure
	cmake --build --preset release --parallel

core: configure
	cmake --build --preset debug --target core --parallel

cli: configure
	cmake --build --preset debug --target cli --parallel

TEST_SCRIPTS := \
								tests/test_course.py \
								tests/test_assessment.py \
								tests/test_grade.py \
								tests/test_dump.py \
								tests/test_clean.py

test-verbose:
	@$(MAKE) --no-print-directory release > /tmp/grades-build.log 2>&1 || (cat /tmp/grades-build.log; exit 1)
	@set -e; \
	for script in $(TEST_SCRIPTS); do \
		name=$${script##*/}; \
		name=$${name%.py}; \
		name=$${name#test_}; \
		echo "Testing $$name subcommand"; \
		python3 "$$script"; \
		echo; \
	done

test:
	@$(MAKE) --no-print-directory release > /tmp/grades-build.log 2>&1 || (cat /tmp/grades-build.log; exit 1)
	@failed=0; \
	passed=0; \
	failed_suites=0; \
	for script in $(TEST_SCRIPTS); do \
		output=$$(python3 "$$script" 2>&1); \
		result=$$?; \
		summary=$$(printf '%s\n' "$$output" | sed -E 's/\x1B\[[0-9;]*[[:alpha:]]//g'); \
		passed_count=$$(printf '%s\n' "$$summary" | sed -n 's/^[[:space:]]*Passed: \([0-9]*\).*/\1/p'); \
		failed_count=$$(printf '%s\n' "$$summary" | sed -n 's/^[[:space:]]*Failed: \([0-9]*\).*/\1/p'); \
		passed=$$((passed + $${passed_count:-0})); \
		failed=$$((failed + $${failed_count:-0})); \
		if [ $$result -ne 0 ]; then \
			failed_suites=$$((failed_suites + 1)); \
		fi; \
	done; \
	if [ $$failed -eq 0 ] && [ $$failed_suites -eq 0 ]; then \
		printf 'Test Summary: \033[32mPASS\033[0m\n'; \
	else \
		printf 'Test Summary: \033[31mFAIL\033[0m\n'; \
	fi; \
	printf 'Passed tests: %d\n' $$passed; \
	printf 'Failed tests: %d\n' $$failed; \
	exit $$((failed_suites > 0))

install: release
	sudo cmake --install build --config release

clean:
	rm -rf build/
