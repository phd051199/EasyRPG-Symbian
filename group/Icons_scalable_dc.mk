ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
ICONTARGETFILENAME=$(TARGETDIR)\EasyRPG_0xE31EA5A1.mif
ICONSOURCE=..\platform\symbian\icon\app_icon.svg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN :
	@echo ...Deleting $(ICONTARGETFILENAME)
	del /q /f $(ICONTARGETFILENAME)

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : $(ICONTARGETFILENAME)

$(ICONTARGETFILENAME) : $(ICONSOURCE)
	mifconv $(ICONTARGETFILENAME) \
		/c32 $(ICONSOURCE)

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
