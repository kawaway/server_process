DRIVERS=test src deps

.PHONY:	all
all:
	@for dir in $(DRIVERS); do (cd $$dir; $(MAKE)) ; done

.PHONY:	clean
clean:	
	@for dir in $(DRIVERS); do (cd $$dir; $(MAKE) clean) ; done
