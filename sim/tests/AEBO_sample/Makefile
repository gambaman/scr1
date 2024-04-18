src_dir := $(dir $(lastword $(MAKEFILE_LIST)))

c_src := sc_print.c AEBO_sample.c

include $(inc_dir)/common.mk

default: log_requested_tgt $(bld_dir)/AEBO_sample.elf $(bld_dir)/AEBO_sample.hex $(bld_dir)/AEBO_sample.dump

log_requested_tgt:
	echo AEBO_sample.hex>> $(bld_dir)/test_info

clean:
	$(RM) $(c_objs) $(asm_objs) $(bld_dir)/AEBO_sample.elf $(bld_dir)/AEBO_sample.hex $(bld_dir)/AEBO_sample.dump
