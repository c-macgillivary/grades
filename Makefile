.PHONY: all configure clean debug release install

all: debug

configure: build/.configured

CMAKE_FILES = $(shell find . -name CMakeLists.txt)

build/.configured: $(CMAKE_FILES) CMakePresets.json
	cmake --preset default
	@touch $@

debug: configure
	cmake --build --preset debug

release: configure
	cmake --build --preset release

install: release
	sudo cmake --install build --config release

clean:
	rm -rf build/
