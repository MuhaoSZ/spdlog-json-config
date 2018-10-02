MAKEINCL := ./Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
	include $(MAKEINCL)
endif

MODULES := 
MODULES += example.dir
MODULES += test.dir

CLEAN_MODULES := $(subst .dir,.clean, $(MODULES))


.PHONY: all clean doxygen

all: $(MODULES)

clean: $(CLEAN_MODULES)

%.dir: %
	$(MAKE) -C $<

%.clean: %
	$(MAKE) -C $< clean

doxygen:
	doxygen .doxygen-config
