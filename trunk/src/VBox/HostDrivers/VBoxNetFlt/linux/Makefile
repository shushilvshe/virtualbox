#
# Makefile for the VirtualBox Linux Host Network Filter Driver.
#

#
# Copyright (C) 2006-2019 Oracle Corporation
#
# This file is part of VirtualBox Open Source Edition (OSE), as
# available from http://www.virtualbox.org. This file is free software;
# you can redistribute it and/or modify it under the terms of the GNU
# General Public License (GPL) as published by the Free Software
# Foundation, in version 2 as it comes in the "COPYING" file of the
# VirtualBox OSE distribution. VirtualBox OSE is distributed in the
# hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
#
# The contents of this file may alternatively be used under the terms
# of the Common Development and Distribution License Version 1.0
# (CDDL) only, as it comes in the "COPYING.CDDL" file of the
# VirtualBox OSE distribution, in which case the provisions of the
# CDDL are applicable instead of those of the GPL.
#
# You may elect to license modified versions of this file under the
# terms and conditions of either the GPL or the CDDL or both.
#

# Linux kbuild sets this to our source directory if we are called from
# there
obj ?= $(CURDIR)
include $(obj)/Makefile.include.header

MOD_NAME = vboxnetflt
MOD_OBJS = \
	linux/VBoxNetFlt-linux.o \
	VBoxNetFlt.o \
	SUPR0IdcClient.o \
	SUPR0IdcClientComponent.o \
	linux/SUPR0IdcClient-linux.o

ifeq ($(BUILD_TARGET_ARCH),x86)
MOD_OBJS += math/gcc/divdi3.o \
	math/gcc/moddi3.o \
	math/gcc/qdivrem.o \
	math/gcc/udivdi3.o \
	math/gcc/udivmoddi4.o \
        math/gcc/divdi3.o \
	math/gcc/umoddi3.o
endif

MOD_INCL = $(addprefix -I$(KBUILD_EXTMOD),/ /include /r0drv/linux)
MOD_DEFS = -DRT_OS_LINUX -DIN_RING0 -DIN_RT_R0 \
	-DIN_SUP_R0 -DVBOX -DRT_WITH_VBOX -DVBOX_WITH_HARDENING \
	-Wno-declaration-after-statement
ifeq ($(BUILD_TARGET_ARCH),amd64)
 MOD_DEFS += -DRT_ARCH_AMD64
else
 MOD_DEFS += -DRT_ARCH_X86
endif
# must be consistent with Config.kmk!
MOD_DEFS += -DVBOX_WITH_64_BITS_GUESTS

# By default we use remap_pfn_range() kernel API to make kernel pages
# visible for userland. Unfortunately, it leads to situation that
# during debug session all structures on that page (such as PVM pointer)
# are not accessible to the debugger (see #3214).
# This code enables experimental support
# for vm_insert_page() kernel API, allowing to export kernel pages
# to the userland in more debugger-friendly way. Due to stability
# concerns, not enabled by default yet.
ifdef VBOX_USE_INSERT_PAGE
 KFLAGS  += -DVBOX_USE_INSERT_PAGE
endif

# build defs
MOD_CFLAGS = -include $(KBUILD_EXTMOD)/include/VBox/SUPDrvMangling.h -fno-pie

include $(obj)/Makefile.include.footer
