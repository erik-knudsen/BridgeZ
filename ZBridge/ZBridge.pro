#-------------------------------------------------
#
# Project created by QtCreator 2012-12-18T10:40:03
#
#-------------------------------------------------

QT       += core gui network help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZBridge
TEMPLATE = app
LIBS += c:\Projects\qt\Zbridge\ZBridge\dds.dll


SOURCES +=\
        cmainframe.cpp \
    czbridgedoc.cpp \
    czbridgeapp.cpp \
    caboutdlg.cpp \
    cbidoptionspropsheet.cpp \
    cdealoptionspropsheet.cpp \
    cdispoptionspropsheet.cpp \
    cgameoptionspropsheet.cpp \
    cstatusbar.cpp \
    cbiddialog.cpp \
    cseatconfiguration.cpp \
    cbidoptions.cpp \
    cnewconvention.cpp \
    cbidoptiondoc.cpp \
    cseatoptiondoc.cpp \
    cdealoptiondoc.cpp \
    cdisplayoptiondoc.cpp \
    cgameoptiondoc.cpp \
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
    crubberscoredialog.cpp \
    cgamesdoc.cpp \
    cpointscoredialog.cpp \
    crankscoredialog.cpp \
    cplaydialog.cpp \
    cplayshow.cpp \
    ctblmngrbase.cpp \
    cactorbase.cpp \
    cactorlocalauto.cpp \
    cactorremoteauto.cpp \
    ctblmngrserverauto.cpp \
    ctblmngrclientauto.cpp \
    mt19937ar.cpp \
    cddtable.cpp \
    clayoutcardsdialog.cpp \
    cauction.cpp \
    cauctions.cpp \
    cbiddb.cpp \
    cbiddesc.cpp \
    cbiddingsystemdialog.cpp \
    cbidpagedescdialog.cpp \
    cbids.cpp \
    cfeatures.cpp \
    crule.cpp \
    cruledialog.cpp \
    cbiddbdefine.cpp \
    cbidandplayengines.cpp \
    cpopup.cpp \
    chelpbrowser.cpp

HEADERS  += cmainframe.h \
    czbridgedoc.h \
    czbridgeapp.h \
    caboutdlg.h \
    Defines.h \
    cbidoptionspropsheet.h \
    cdealoptionspropsheet.h \
    cdispoptionspropsheet.h \
    cgameoptionspropsheet.h \
    cstatusbar.h \
    cbiddialog.h \
    cseatconfiguration.h \
    cbidoptions.h \
    cnewconvention.h \
    cbidoptiondoc.h \
    cseatoptiondoc.h \
    cdealoptiondoc.h \
    cdisplayoptiondoc.h \
    cgameoptiondoc.h \
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
    crubberscoredialog.h \
    cgamesdoc.h \
    cpointscoredialog.h \
    crankscoredialog.h \
    cplaydialog.h \
    cplayshow.h \
    ctblmngrbase.h \
    cactorbase.h \
    cactorlocalauto.h \
    cactorremoteauto.h \
    ctblmngrserverauto.h \
    ctblmngrclientauto.h \
    mt19937ar.h \
    cddtable.h \
    clayoutcardsdialog.h \
    cauction.h \
    cauctions.h \
    cbiddb.h \
    cbiddesc.h \
    cbiddingsystemdialog.h \
    cbidpagedescdialog.h \
    cbids.h \
    cfeatures.h \
    crule.h \
    cruledialog.h \
    cbiddbdefine.h \
    cbidandplayengines.h \
    cpopup.h \
    chelpbrowser.h

FORMS    += cmainframe.ui \
    caboutdlg.ui \
    cbidoptionspropsheet.ui \
    cdealoptionspropsheet.ui \
    cdispoptionspropsheet.ui \
    cgameoptionspropsheet.ui \
    cbiddialog.ui \
    cseatconfiguration.ui \
    cbidoptions.ui \
    cnewconvention.ui \
    cmainscoredialog.ui \
    crubberscoredialog.ui \
    cpointscoredialog.ui \
    cplaydialog.ui \
    crankscoredialog.ui \
    cplayshow.ui \
    cddtable.ui \
    clayoutcardsdialog.ui \
    cbiddingsystemdialog.ui \
    cbidpagedescdialog.ui \
    cruledialog.ui \
    cpopup.ui

RESOURCES = images.qrc
