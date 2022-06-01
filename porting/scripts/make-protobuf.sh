#!/bin/bash

. $(dirname $0)/defines.sh;

cmake_root=$SOURCE_ROOT/external/protobuf/out;

# Clean built products
[[ -d "$cmake_root" ]] && rm -rfv "$cmake_root";
mkdir -pv "$cmake_root";

cd "$cmake_root";

which protoc || {
	echo "** ERROR: protoc not installed";
	exit 1;
}
protoc_version=$(protoc --version | cut -d' ' -f2 | cut -d. -f1 -f2).x;

# Switch to version
git checkout $protoc_version;

# Copy CMakeLists.txt to source folder
cp $SOURCE_ROOT/porting/cmake/CMakeLists.protobuf.txt $SOURCE_ROOT/external/protobuf/CMakeLists.txt;

cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE -DPLATFORM=$PLATFORM -DDEPLOYMENT_TARGET=$DEPLOYMENT_TARGET;
cmake --build . --target protobuf --config Debug --parallel 8;
find . -name "*.a" -exec cp -av {} $FULL_OUTPUT \;
