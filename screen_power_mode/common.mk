#  
# Copyright 2015 QNX Software Systems. 
#  
# Licensed under the Apache License, Version 2.0 (the "License"). You 
# may not reproduce, modify or distribute this software except in 
# compliance with the License. You may obtain a copy of the License 
# at: http://www.apache.org/licenses/LICENSE-2.0 
#  
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
# 
# This file may contain contributions from others, either as 
# contributors under the License or as licensors under other terms.  
# Please review this entire file for other proprietary rights or license 
# notices, as well as the QNX Development Suite License Guide at 
# http://licensing.qnx.com/license-guide/ for other information.
# 
#
# General purpose makefile for building a Neutrino character device driver
#
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=SCREEN POWER MODE
endef

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
   USE_INSTALL_ROOT=1
##############################################################

NAME := screen_power_mode
USEFILE = $(PROJECT_ROOT)/Usemsg
INSTALLDIR = sbin
LIBS = screen

include $(MKFILES_ROOT)/qtargets.mk

EXTRA_INCVPATH+= $(EXTRA_INCVPATH_$(SECTION)) \
	         $(PRODUCT_ROOT)/../../install/usr/include
