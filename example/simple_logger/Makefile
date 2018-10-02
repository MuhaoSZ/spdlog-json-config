# Include Makeincl
MAKEINCL := ../../Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
	include $(MAKEINCL)
endif


INCLUDE += -I $(ROOTDIR)/include/spdlog_json_config


all: test_simple_logger

test_simple_logger: test_simple_logger.cc
	$(GXX) $(CFLAGS) $(INCLUDE) -o $@ $^

clean:
	rm -rf test_simple_logger ./logs

