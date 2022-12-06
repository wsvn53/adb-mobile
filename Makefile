all: check-protobuf-version libs libadb.a libadb.include

check-protobuf-version:
	@grep $$(protoc --version | cut -d' ' -f2) porting/protobuf.rb || { echo "* To compile adb please install protobuf version 3.19.4 by:" && echo "brew unlink protobuf && brew install porting/protobuf.rb" && echo "" && exit 1; }
	protoc --version
	exit 0

libs:
	[[ -d output ]] || mkdir output && echo "mkdir output"
	make -C porting

libadb.include:
	[[ ! -d output/include ]] && mkdir output/include || echo "make include";
	cp -av porting/adb/include/*.h output/include;

libadb.a:
	libtool -static -o output/libadb-iphoneos.a output/iphoneos/arm64/*.a
	libtool -static -o output/libadb-iphonesimulator.a output/iphonesimulator/x86_64/*.a
	lipo -create output/libadb-*.a -output output/libadb.a
