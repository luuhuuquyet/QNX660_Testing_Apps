ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk
-include $(PROJECT_ROOT)/roots.mk
##############################################################
define PINFO
PINFO DESCRIPTION=Memory test
endef

USEFILE = $(PROJECT_ROOT)/Usemsg
#########################################################
# Make sure the build environment is configured properly
#########################################################
include $(MKFILES_ROOT)/qtargets.mk
