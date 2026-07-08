TARGET = neighbours-from-hell

OBJS = \
	src/engine/graphics/g2d.o \
	src/engine/graphics/glib2d.o \
	src/engine/controls/controls.o \
	src/engine/audio/pspaalib.o \
	src/engine/audio/pspaalibat3.o \
	src/engine/audio/pspaalibcommon.o \
	src/engine/audio/pspaalibeffects.o \
	src/engine/audio/pspaalibwav.o \
	src/engine/fonts/intraFont.o \
	src/engine/fonts/libccc.o \
	src/engine/NFHSound/NFHSound.o \
	src/engine/json/cJSON.o \
	src/engine/json/json_helper.o \
	src/types/item_use_names.o \
	src/objects/scene_manager.o \
	src/objects/woody.o \
	src/gfxdata/woody/generic.o \
	src/animations/woody/generic.o \
	src/gfxdata/woody/generic2.o \
	src/animations/woody/generic2.o \
	src/gfxdata/woody/generic3.o \
	src/animations/woody/generic3.o \
	src/gfxdata/woody/doorleft.o \
	src/animations/woody/doorleft.o \
	src/gfxdata/woody/doorright.o \
	src/animations/woody/doorright.o \
	src/gfxdata/woody/doorback.o \
	src/animations/woody/doorback.o \
	src/objects/intro.o \
	src/objects/neighbour.o \
	src/gfxdata/neighbour/generic.o \
	src/animations/neighbour/generic.o \
	src/gfxdata/neighbour/doorleft.o \
	src/animations/neighbour/doorleft.o \
	src/gfxdata/neighbour/doorright.o \
	src/animations/neighbour/doorright.o \
	src/gfxdata/neighbour/sofa.o \
	src/animations/neighbour/sofa.o \
	src/gfxdata/neighbour/binoculars.o \
	src/animations/neighbour/binoculars.o \
	src/gfxdata/neighbour/generic2.o \
	src/animations/neighbour/generic2.o \
	src/gfxdata/neighbour/generic3.o \
	src/animations/neighbour/generic3.o \
	src/gfxdata/neighbour/look.o \
	src/animations/neighbour/look.o \
	src/gfxdata/neighbour/shout0.o \
	src/animations/neighbour/shout0.o \
	src/gfxdata/neighbour/sofa1.o \
	src/animations/neighbour/sofa1.o \
	src/gfxdata/neighbour/gameover.o \
	src/animations/neighbour/gameover.o \
	src/objects/sign.o \
	src/gfxdata/ui/sign.o \
	src/animations/ui/sign.o \
	src/objects/tutorial.o \
	src/gfxdata/ui/joe.o \
	src/animations/ui/joe.o \
	src/objects/level_end.o \
	src/scenes/menu/disclaimer.o \
	src/scenes/menu/madebydntrnk.o \
	src/scenes/menu/cinemamenu.o \
	src/scenes/menu/mainmenu.o \
	src/scenes/menu/pause.o \
	src/scenes/levels/tutorial_1.o \
	src/scenes/levels/level_peep.o \
	src/main.o

INCDIR = include
CFLAGS = -G0 -O3 -fomit-frame-pointer -fsingle-precision-constant -fno-math-errno -ffast-math -Wall -Wno-trigraphs
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

BUILD_PRX = 1
PSP_FW_VERSION = 660

LIBS = -lpspgu -lpspge -lpspdisplay -lpsppower -lpspctrl -lpspaudio -lpspatrac3 -lpspvram -lpng -lfreetype -lbz2 -lz

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Neighbours from Hell
PSP_EBOOT_ICON = res/ICON0.PNG
PSP_EBOOT_PIC1 = res/PIC1.PNG
PSP_EBOOT_SND0 = res/SND0.AT3

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

all: EBOOT.PBP copy

copy: EBOOT.PBP
	mkdir -p build
	cp -f EBOOT.PBP build/EBOOT.PBP
