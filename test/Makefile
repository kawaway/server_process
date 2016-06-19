#DRIVERS=$(shell ls -l | grep ^d | awk '{print $NF}')
DRIVERS=log_driver

.PHONY:	all
all:
	@for dir in $(DRIVERS); do (cd $$dir ; $(MAKE) link; $(MAKE)) ; done

.PHONY:	clean
clean:	
	@for dir in $(DRIVERS); do (cd $$dir ; $(MAKE) clean) ; done
