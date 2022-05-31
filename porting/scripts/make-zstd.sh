#!/bin/bash

. $(dirname $0)/defines.sh;

cmake_root=$SOURCE_ROOT/external/zstd/build/cmake/out;

# Clean built products
[[ -d "$cmake_root" ]] && rm -rfv "$cmake_root";
mkdir -pv "$cmake_root";

cp -av $PORTING_ROOT/cmake/CMakeLists.zstd.txt $cmake_root/../programs/CMakeLists.txt;

cd "$cmake_root";

cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE -DPLATFORM=$PLATFORM \
	-DDEPLOYMENT_TARGET=$DEPLOYMENT_TARGET;
cmake --build . --config Release --target libzstd_static --parallel 8;
find . -name "*.a" -exec cp -av {} $FULL_OUTPUT \;
