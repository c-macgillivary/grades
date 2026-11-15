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
	for script in $(TEST_SCRIPTS); do \
		python3 "$$script" > /dev/null 2>&1; \
		result=$$?; \
		failed=$$((failed + result)); \
	done; \
	if [ $$failed -eq 0 ]; then \
		printf 'Test Summary: \033[32mPASS\033[0m\n'; \
	else \
		printf 'Test Summary: \033[31mFAIL\033[0m\nFailed tests: %d\n' $$failed; \
		exit 1; \
	fi

install: release
	sudo cmake --install build --config release

clean:
	rm -rf build/
