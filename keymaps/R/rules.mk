
COMMON_VPATH += tmk_core/protocol/nrf
COMMON_VPATH += tmk_core/protocol/nrf/sdk15
COMMON_VPATH += tmk_core/protocol/nrf/microshell/core

MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control

POINTING_DEVICE_ENABLE = yes
#DYNAMIC_MACRO_ENABLE = yes
RAW_ENABLE = yes


SRC += i2c_master.c paw3204.c


