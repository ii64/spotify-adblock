CC := "C:\Program Files\mingw32\bin\gcc.exe"
INJECTOR_BASE := src
DLL_BASE := src/dll

YAML_MAJOR := 0
YAML_MINOR := 2
YAML_PATCH := 5

YAML_RELEASE := 0
YAML_CURRENT := 2
YAML_REVISION := 9

default: build-dll

all: build-injector build-dll

clean:
	rm a.exe
	rm inject.dll

build-injector:
	$(CC) \
		-D LOG_USE_COLOR=1 \
		-lShlwapi \
		-static-libgcc \
		-static \
		-o a \
		$(wildcard $(DLL_BASE)/lib/log.c/src/*.c) \
		$(wildcard $(INJECTOR_BASE)/*.c)

build-dll:
	$(CC) \
		-D VERSION_MAJOR=1 \
		-D VERSION_MINOR=2 \
		-D VERSION_PATCH=1 \
		-D VERSION_DEVEL=1 \
		-D YAML_VERSION_MAJOR=${YAML_MAJOR} \
		-D YAML_VERSION_MINOR=${YAML_MINOR} \
		-D YAML_VERSION_PATCH=${YAML_PATCH} \
		-D YAML_VERSION_STRING="\"${YAML_MAJOR}.${YAML_MINOR}.${YAML_PATCH}\"" \
		-D YAML_LOG_LEVEL=CYAML_LOG_INFO \
		-o inject.dll \
		-static-libgcc \
		-fPIC \
		-shared \
		-lShlwapi \
		-lPsapi \
		-lws2_32 \
		-lwsock32 \
		-lgdi32 \
		-Wunused-value \
		-Wall \
		-I$(DLL_BASE)/lib/libyaml/include \
		-I$(DLL_BASE)/lib/libcyaml/include \
		-I$(DLL_BASE)/lib/cef \
		-I$(DLL_BASE)/lib/chromium \
		$(wildcard $(DLL_BASE)/lib/log.c/src/*.c) \
		$(wildcard $(DLL_BASE)/lib/minhook/src/*.c $(DLL_BASE)/lib/minhook/src/hde/*.c) \
		$(wildcard $(DLL_BASE)/lib/libyaml/src/*.c) \
		$(wildcard $(DLL_BASE)/lib/libcyaml/src/*.c) \
		$(wildcard $(DLL_BASE)/*.c)
