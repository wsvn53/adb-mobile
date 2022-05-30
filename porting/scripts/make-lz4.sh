#!/bin/bash

. $(dirname $0)/defines.sh;

echo " - Lib Name: $LIB_NAME";
echo " - SDK Name: $SDK_NAME";
echo " - Arch Name: $ARCH_NAME";
echo " - CMake Toolchain: $CMAKE_TOOLCHAIN_FILE";

cmake_root=$SOURCE_ROOT/external/lz4/contrib/cmake_unofficial/out;

# Clean built products
[[ -d "$cmake_root" ]] && rm -rfv "$cmake_root";
mkdir -pv "$cmake_root";

cd "$cmake_root";

cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE -DPLATFORM=$PLATFORM \
	-DDEPLOYMENT_TARGET=$DEPLOYMENT_TARGET -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF;
cmake --build . --config Release --target lz4_static;
find . -name "*.a" -exec cp -av {} $FULL_OUTPUT \;
