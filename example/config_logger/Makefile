# Include Makeincl
MAKEINCL := ../../Makeincl
ifeq ($(shell ls $(MAKEINCL)), $(MAKEINCL))
	include $(MAKEINCL)
endif


INCLUDE += -I $(ROOTDIR)/include/spdlog_json_config

all: test_config_logger

test_config_logger: config_logger.o test_config_logger.o
	$(GXX) $(CFLAGS) $(INCLUDE) -o $@ $^

%.o: %.cc
	$(GXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<


clean:
	rm -rf test_config_logger ./logs *.o
