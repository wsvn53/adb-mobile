iphone: lz4 zstd brotli protobuf adb

## lz4
lz4: lz4/iphoneos/arm64 lz4/iphonesimulator/arm64 lz4/iphonesimulator/x86_64
lz4/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-lz4.sh

## zstd
zstd: zstd/iphoneos/arm64 zstd/iphonesimulator/arm64 zstd/iphonesimulator/x86_64
zstd/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-zstd.sh

## brotli
brotli: brotli/iphoneos/arm64 brotli/iphonesimulator/arm64 brotli/iphonesimulator/x86_64
brotli/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-brotli.sh

## protobuf
protobuf: protobuf/iphoneos/arm64 protobuf/iphonesimulator/arm64 protobuf/iphonesimulator/x86_64
protobuf/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-protobuf.sh

## adb
adb: adb/iphoneos/arm64 adb/iphonesimulator/arm64 adb/iphonesimulator/x86_64
adb/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-adb.sh
