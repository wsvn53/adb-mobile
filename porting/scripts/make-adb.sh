#!/bin/bash

. $(dirname $0)/defines.sh;

cmake_root=$SOURCE_ROOT/android-tools/out;

# Clean built products
[[ -d "$cmake_root" ]] && rm -rfv "$cmake_root";
mkdir -pv "$cmake_root";

cd "$cmake_root";

# Copy CMakeLists.txt to source folder
cp -av $SOURCE_ROOT/porting/cmake/CMakeLists.vendor.txt ../vendor/CMakeLists.txt;
cp -av $SOURCE_ROOT/porting/cmake/CMakeLists.adb.txt ../vendor/CMakeLists.adb.txt;

cmake -DCMAKE_OSX_SYSROOT=$SDK_NAME -DCMAKE_OSX_ARCHITECTURES=$ARCH_NAME  \
	-DCMAKE_OSX_DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET -DCMAKE_BUILD_TYPE=Debug ..;
make -j8 libadb crypto decrepit libcutils libzip libdiagnoseusb libbase \
	libadb_crypto_defaults libcrypto libadb_tls_connection_defaults;

find . -name "*.a" -exec cp -av {} $FULL_OUTPUT \;
