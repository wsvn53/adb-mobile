iphone: lz4
	@echo "|-> Making liblz4 ..."

lz4: lz4/iphoneos/arm64 lz4/iphonesimulator/x86_64

lz4/%:
	@echo "|-> Making $@ ..."
	TARGET=$@ OUTPUT=../output bash ./scripts/make-lz4.sh
