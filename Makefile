.PHONY: all configure debug release core test install clean

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

test:
	@$(MAKE) --silent --no-print-directory release > /dev/null
	@python3 tests/test_grades.py

install: release
	sudo cmake --install build --config release

clean:
	rm -rf build/
