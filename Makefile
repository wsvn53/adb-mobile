all: libs libadb.a libadb.include

libs:
	make -C porting

libadb.include:
	[[ ! -d output/include ]] && mkdir output/include || echo "make include";
	cp -av porting/adb/include/*.h output/include;

libadb.a:
	libtool -static -o output/libadb-iphoneos.a output/iphoneos/arm64/*.a
	libtool -static -o output/libadb-iphonesimulator.a output/iphonesimulator/x86_64/*.a
	lipo -create output/libadb-*.a -output output/libadb.a
