MAKEINCL := ../Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
	include $(MAKEINCL)
endif

MODULES := 
MODULES += default_logger.dir
MODULES += simple_logger.dir
MODULES += config_logger.dir

CLEAN_MODULES := $(subst .dir,.clean, $(MODULES))


.PHONY: all package install clean doxygen

all: $(MODULES)

clean: $(CLEAN_MODULES)

%.dir: %
	$(MAKE) -C $<

%.clean: %
	$(MAKE) -C $< clean

