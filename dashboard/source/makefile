# Information
FILENAME = dashboard
TITLE = ETS2 Dashboard

# Linking
OBJS = main.o PNGLoad.o SegmentDisplay.o ETS2_Telemetry.o NetPackets.o

# Compiler flags
CFLAGS = -O1 -G0 -Wall -pedantic
CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)

# Configuration
TARGET = $(FILENAME)
PSP_EBOOT_TITLE = $(TITLE)
LIBS = -lstdc++ -losl -lpng -ljpeg -lz -lintraFont -lpsphttp -lpspssl -lpspwlan \
       -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum -lpspaudiolib -lpspaudio \
       -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -lpsphprm -lpspsdk
EXTRA_TARGETS = EBOOT.PBP

# PSP SDK
PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak