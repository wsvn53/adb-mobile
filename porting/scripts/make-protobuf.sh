#!/bin/bash

. "$(dirname $0)/defines.sh";

cmake_root=$SOURCE_ROOT/external/protobuf/out;

# Clean built products
[[ -d "$cmake_root" ]] && rm -rfv "$cmake_root";
mkdir -pv "$cmake_root";

cd "$cmake_root" || exit;

which protoc || {
	echo "** ❌ ERROR: protoc not installed";
	exit 1;
}
protoc_version=$(protoc --version | cut -d' ' -f2);
echo "using protobuf {$protoc_version}";

# Switch to version
git clean -f;
git checkout v$protoc_version || exit 1;
git submodule update --init --recursive || exit 1;

# Fix absl version
$(cd $SOURCE_ROOT/external/protobuf/third_party/abseil-cpp && git checkout 20240722.0);

# Copy CMakeLists.txt to source folder
cp $SOURCE_ROOT/porting/cmake/CMakeLists.protobuf.txt $SOURCE_ROOT/external/protobuf/CMakeLists.txt;

cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE -DPLATFORM=$PLATFORM -DDEPLOYMENT_TARGET=$DEPLOYMENT_TARGET;

# Hack files
cp -av $PORTING_ROOT/protobuf/src/google/protobuf/map_probe_benchmark.cc \
  $SOURCE_ROOT/src/google/protobuf/map_probe_benchmark.cc

cmake --build . --target protobuf --config Debug --parallel 8;
find . -name "*.a" -exec cp -av {} $FULL_OUTPUT \;
