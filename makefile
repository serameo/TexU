#
##  "@(#)Makefile	
#
MAKES=mono color tb2

all:
	for name in $(MAKES) ;\
	do\
		echo makeing $$name ;\
		make -f makefile.$$name clean all; \
	done

clean:
	for name in $(MAKES) ;\
	do\
		echo makeing clean $$name ;\
		make -f makefile.$$name clean ; \
	done

