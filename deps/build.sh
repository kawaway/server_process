#!/bin/sh
PREFIX=`/bin/pwd`
HTTP_PARSER_VER=2.7.0
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
	cd lib
	ln -s libhttp_parser.${HTTP_PARSER_VER}.${SOEXT} libhttp_parser.${SOEXT} 
	cd ..
}

#mkdir -p lib include
#uv_build
parser_build
