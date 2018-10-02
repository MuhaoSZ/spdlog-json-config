# Include Makeincl
MAKEINCL := ../../Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
	include $(MAKEINCL)
endif



INCLUDE += -I $(ROOTDIR)/include/spdlog_json_config


all: test_default_logger

test_default_logger: test_default_logger.cc
	$(GXX) $(CFLAGS) $(INCLUDE) -o $@ $^

clean:
	rm -f test_default_logger
