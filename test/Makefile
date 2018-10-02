# Include Makeincl
MAKEINCL := ../Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
    include $(MAKEINCL)
endif

INCLUDE += -I $(ROOTDIR)/deps/catch2/include
INCLUDE += -I $(ROOTDIR)/include/spdlog_json_config

.PHONY: all clean


all: unit_test

unit_test: unit_test.cc
	$(GXX) $(CFLAGS) $(INCLUDE) -o unit_test unit_test.cc

clean:
	rm -rf unit_test *.o ./logs

