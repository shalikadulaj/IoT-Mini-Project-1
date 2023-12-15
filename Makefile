# name of your application
APPLICATION = Sensing_Layer

# If no BOARD is found in the environment, use this default:
BOARD ?=iotlab-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default
# Specify the mandatory networking module for a IPv6 routing node
USEMODULE += gnrc_ipv6_router_default
# Include MQTT-SN
USEMODULE += emcute
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
# For testing we also include the ping6 command and some stats
USEMODULE += gnrc_icmpv6_echo
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single
# For measuring time
USEMODULE += ztimer_usec
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl
# Add the required sensor modules here
USEMODULE += lps331ap
USEMODULE += isl29020


# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
    CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# Instead of simulating an Ethernet connection, we can also simulate
# an IEEE 802.15.4 radio using ZEP
USE_ZEP ?= 0

# set the ZEP port for native
ZEP_PORT_BASE ?= 17754
ifeq (1,$(USE_ZEP))
  TERMFLAGS += -z [::1]:$(ZEP_PORT_BASE)
  USEMODULE += socket_zep
endif


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Comment this out to join RPL DODAGs even if DIOs do not contain
# DODAG Configuration Options (see the doc for more info)
# CFLAGS += -DGNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN

CC = gcc-9
CFLAGS = -Wall -Werror -Wno-error=unused-parameter

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk
