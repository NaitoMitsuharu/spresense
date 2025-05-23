############################################################################
# modules/asmp/worker/Makefile
#
#   Copyright 2018 Sony Semiconductor Solutions Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name of Sony Semiconductor Solutions Corporation nor
#    the names of its contributors may be used to endorse or promote
#    products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

include $(APPDIR)/Make.defs
-include $(SDKDIR)/Make.defs

include mkfiles/libc.mk
include mkfiles/fmsynth.mk
include mkfiles/cmsis.mk

VPATH   = arch $(CXD56XX_ARCH) $(EXT_VPATH)
SUBDIRS =
DEPPATH = --dep-path arch --dep-path . $(EXT_DEPPATH)

ASRCS  = exception.S

CSRCS  = common.c mpmq.c mpmutex.c mpshm.c printf.c
CSRCS += cpufifo.c cpuid.c doirq.c startup.c sysctl.c clock.c timer.c lowputc.c delay.c gpio.c
CSRCS += $(EXT_CSRCS)

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

LIB_OBJS = $(AOBJS) $(COBJS)

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libasmpw$(LIBEXT)

ARCHSRCDIR = $(TOPDIR)$(DELIM)arch$(DELIM)$(CONFIG_ARCH)$(DELIM)src

CFLAGS := $(EXT_INCPATH) $(CFLAGS)
CFLAGS += ${INCDIR_PREFIX}$(ARCHSRCDIR)$(DELIM)chip
CFLAGS += ${INCDIR_PREFIX}$(ARCHSRCDIR)$(DELIM)common
ifneq ($(CONFIG_ARCH_FAMILY),)
CFLAGS += ${INCDIR_PREFIX}$(ARCHSRCDIR)$(DELIM)$(CONFIG_ARCH_FAMILY)
endif

CFLAGS += -fno-tree-loop-distribute-patterns -DCXD5602_WORKER

AFLAGS += -isystem $(SDKDIR)/modules/include

all: $(BIN) | $(EXT_LIBS)
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(LIB_OBJS)
	$(call ARLOCK, $@, $(LIB_OBJS))

.depend: Makefile $(SRCS) $(TOPDIR)$(DELIM).config
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

.context:
	$(Q) touch $@

context: .context

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, $(BIN).lock)
	$(call DELFILE, .context)
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

