QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    wrnch.cpp

HEADERS += \
    Quaternion.hpp \
    dialog.h \
    json2csv.h \
    mainwindow.h \
    model.h \
    wrnch.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# dependencies

INCLUDEPATH += D:/yanzhang/wrnchAI-engine-1.13.0-win64/include \
    D:/yanzhang/wrnchAI-engine-1.13.0-win64/src/wrnchAI/wrSamples/cpp/include \
    D:/yanzhang/opencv/build/include \
    D:/yanzhang/project/wrnchGUI/rapidjson-1.1.0/include \
    D:/yanzhang/project/jsoncpp/include \
    #$$quote(D:/Program Files (x86)/Point Grey Research/Spinnaker/include)



win32: LIBS += -L$$PWD/../../wrnchAI-engine-1.13.0-win64/lib/ -lwrAPI

INCLUDEPATH += $$PWD/../../wrnchAI-engine-1.13.0-win64/lib
DEPENDPATH += $$PWD/../../wrnchAI-engine-1.13.0-win64/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world410
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world410d

INCLUDEPATH += $$PWD/../../opencv/build/x64/vc14
DEPENDPATH += $$PWD/../../opencv/build/x64/vc14


#win32:CONFIG(release, debug|release): LIBS += -L'D:/Program Files (x86)/Point Grey Research/Spinnaker/lib64/vs2015/' \
#    -lSpinnaker_v140 \
#    -lSpinVideo_v140
#else:win32:CONFIG(debug, debug|release): LIBS += -L'D:/Program Files (x86)/Point Grey Research/Spinnaker/lib64/vs2015/' \
#    -lSpinnakerd_v140 \
#    -lSpinVideod_v140

#INCLUDEPATH += 'D:/Program Files (x86)/Point Grey Research/Spinnaker/lib64/vs2015'
#DEPENDPATH += 'D:/Program Files (x86)/Point Grey Research/Spinnaker/lib64/vs2015'

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../jsoncpp/build/src/lib_json/release/ -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../jsoncpp/build/src/lib_json/debug/ -ljsoncpp

INCLUDEPATH += $$PWD/../jsoncpp/build/src/lib_json/Release
DEPENDPATH += $$PWD/../jsoncpp/build/src/lib_json/Release
