#!/bin/sh
PREFIX=`/bin/pwd`
mkdir -p lib include
PLATFORM=`sh -c 'uname -s | tr "[A-Z]" "[a-z]"'`

if [ "x${PLATFORM}" = "xdarwin" ]; then
	SOEXT=dylib
else
	SOEXT=so
fi
uv_build()
{
	cd libuv
	./autogen.sh
	./configure --prefix=$PREFIX
	make
	make install
	cd ..
}
parser_build()
{
	cd http-parser
	make
	make library
	cp http_parser.h ../include
	cp libhttp_parser.*.${SOEXT} ../lib
	cd ..
}

uv_build
parser_build
