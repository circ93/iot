# name of your application
APPLICATION = weather_station_iotlab

# Use the ST B-L072Z-LRWAN1 board by default:
BOARD ?= b-l072z-lrwan1

BOARD_INSUFFICIENT_MEMORY := nucleo-f031k6 nucleo-f042k6 nucleo-l031k6
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/..

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
LORA_DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac

USEMODULE += $(LORA_DRIVER)
USEMODULE += fmt
USEMODULE += hts221
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer
FEATURES_REQUIRED += periph_rtc

CFLAGS += -DREGION_$(LORA_REGION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(LORA_REGION)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include