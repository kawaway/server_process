DEPS = libuv http-parser
.PHONY:	all
all:
	./build.sh
.PHONY:	clean
clean:	
	@for dir in $(DEPS); do (cd $$dir; $(MAKE) clean) ; done
	rm -rf lib include
