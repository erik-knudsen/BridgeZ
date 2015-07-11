#-------------------------------------------------
#
# Project created by QtCreator 2012-12-18T10:40:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZBridge
TEMPLATE = app


SOURCES +=\
        cmainframe.cpp \
    czbridgedoc.cpp \
    czbridgeapp.cpp \
    cfilepropertiesdialog.cpp \
    caboutdlg.cpp \
    cfilecommentsdialog.cpp \
    cbidoptionspropsheet.cpp \
    cdealoptionspropsheet.cpp \
    cdispoptionspropsheet.cpp \
    cgameoptionspropsheet.cpp \
    cprogramconfigwizard.cpp \
    cprogconfigintropage.cpp \
    cprogconfighelplevelpage.cpp \
    cprogconfigviewsettingspage.cpp \
    cprogconfigsuitsdisplaypage.cpp \
    cprogconfiggamemechanicspage.cpp \
    cprogconfigpausespage.cpp \
    cprogconfigbiddingpage.cpp \
    cprogconfigfinishpage.cpp \
    cstatuswnd.cpp \
    cstatusbar.cpp \
    chistorywnd.cpp \
    cbiddialog.cpp \
    cseatconfiguration.cpp \
    cbidoptions.cpp \
    cnewconvention.cpp \
    cbidoptiondoc.cpp \
    cseatoptiondoc.cpp \
    cdealoptiondoc.cpp \
    cdisplayoptiondoc.cpp \
    cgameoptiondoc.cpp \
    cwizardoptiondoc.cpp \
    cplayview.cpp \
    ctopinfoplay.cpp \
    ctopinfoauction.cpp \
    ctopinfo.cpp \
    cmidinfoplay.cpp \
    cmidinfoauction.cpp \
    cmidinfo.cpp \
    ccentercards.cpp \
    ccard.cpp \
    cbottominfoplay.cpp \
    cbottominfoauction.cpp \
    cbottominfo.cpp \
    cbidengine.cpp \
    cactor.cpp \
    cactorremote.cpp \
    cplayengine.cpp \
    cbidhistory.cpp \
    cbid.cpp \
    ctblmngrserver.cpp \
    ctblmngrclient.cpp \
    ../src-gen/ZBridgeServer.c \
    ../src-gen/ZBridgeClient.c \
    CTblMngr.cpp \
    cremoteactorserver.cpp \
    cremoteactorclient.cpp \
    cremoteprotocol.cpp \
    cplayhistory.cpp \
    cactorlocal.cpp \
    cbiditem.cpp \
    cBottominfoButton.cpp \
    csquare.cpp \
    cbidandplay.cpp \
    ccards.cpp \
    clrcards.cpp \
    ctbcards.cpp \
    ../src-gen/ZBridgeClientSync.c \
    ../src-gen/ZBridgeServerSync.c \
    cmainscoredialog.cpp \
    cauctionandplaydialog.cpp \
    crubberscoredialog.cpp \
    ccomparescoredialog.cpp \
    cgamesdoc.cpp \
    cpointscoredialog.cpp \
    crankscoredialog.cpp \
    cplaydialog.cpp \
    cplayshow.cpp

HEADERS  += cmainframe.h \
    czbridgedoc.h \
    czbridgeapp.h \
    cfilepropertiesdialog.h \
    caboutdlg.h \
    cfilecommentsdialog.h \
    Defines.h \
    cbidoptionspropsheet.h \
    cdealoptionspropsheet.h \
    cdispoptionspropsheet.h \
    cgameoptionspropsheet.h \
    cprogramconfigwizard.h \
    cprogconfigintropage.h \
    cprogconfighelplevelpage.h \
    cprogconfigviewsettingspage.h \
    cprogconfigsuitsdisplaypage.h \
    cprogconfiggamemechanicspage.h \
    cprogconfigpausespage.h \
    cprogconfigbiddingpage.h \
    cprogconfigfinishpage.h \
    cstatuswnd.h \
    cstatusbar.h \
    chistorywnd.h \
    cbiddialog.h \
    cseatconfiguration.h \
    cbidoptions.h \
    cnewconvention.h \
    cbidoptiondoc.h \
    cseatoptiondoc.h \
    cdealoptiondoc.h \
    cdisplayoptiondoc.h \
    cgameoptiondoc.h \
    cwizardoptiondoc.h \
    cplayview.h \
    ctopinfoplay.h \
    ctopinfoauction.h \
    ctopinfo.h \
    cmidinfoplay.h \
    cmidinfoauction.h \
    cmidinfo.h \
    ccentercards.h \
    ccard.h \
    cbottominfoplay.h \
    cbottominfoauction.h \
    cbottominfo.h \
    cbidengine.h \
    cactor.h \
    cactorremote.h \
    cplayengine.h \
    cbidhistory.h \
    cbid.h \
    ctblmngrserver.h \
    ctblmngrclient.h \
    ../src-gen/ZBridgeServer.h \
    ../src-gen/ZBridgeClient.h \
    ../src-gen/sc_types.h \
    CTblMngr.h \
    cremoteactorserver.h \
    cremoteactorclient.h \
    cremoteprotocol.h \
    ZBridgeException.h \
    cplayhistory.h \
    cactorlocal.h \
    cbiditem.h \
    cBottominfoButton.h \
    csquare.h \
    cbidandplay.h \
    ccards.h \
    clrcards.h \
    ctbcards.h \
    ZBridge_documentation.h \
    ../src-gen/ZBridgeClientSync.h \
    ../src-gen/ZBridgeServerSync.h \
    cmainscoredialog.h \
    cauctionandplaydialog.h \
    crubberscoredialog.h \
    ccomparescoredialog.h \
    cgamesdoc.h \
    cpointscoredialog.h \
    crankscoredialog.h \
    cplaydialog.h \
    cplayshow.h

FORMS    += cmainframe.ui \
    cfilepropertiesdialog.ui \
    caboutdlg.ui \
    cfilecommentsdialog.ui \
    cbidoptionspropsheet.ui \
    cdealoptionspropsheet.ui \
    cdispoptionspropsheet.ui \
    cgameoptionspropsheet.ui \
    cprogramconfigwizard.ui \
    cprogconfigintropage.ui \
    cprogconfighelplevelpage.ui \
    cprogconfigviewsettingspage.ui \
    cprogconfigsuitsdisplaypage.ui \
    cprogconfiggamemechanicspage.ui \
    cprogconfigpausespage.ui \
    cprogconfigbiddingpage.ui \
    cprogconfigfinishpage.ui \
    cstatuswnd.ui \
    chistorywnd.ui \
    cbiddialog.ui \
    cseatconfiguration.ui \
    cbidoptions.ui \
    cnewconvention.ui \
    cmainscoredialog.ui \
    cauctionandplaydialog.ui \
    crubberscoredialog.ui \
    ccomparescoredialog.ui \
    cpointscoredialog.ui \
    cplaydialog.ui \
    crankscoredialog.ui \
    cplayshow.ui

RESOURCES = images.qrc
