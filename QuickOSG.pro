QT += quick remoteobjects
CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=1
DEFINES += PATCH_VERSION=1

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    appglobal.h \
    fyeventadapter.h \
    quickosgearthrenderer.h \
    quickosgearthviewer.h \
    quickosgrenderer.h \
    quickosgviewer.h \
    viewerdatum.h \
    appworker.h

SOURCES += \
    fyeventadapter.cpp \
    main.cpp \
    appglobal.cpp \
    quickosgearthrenderer.cpp \
    quickosgearthviewer.cpp \
    quickosgrenderer.cpp \
    quickosgviewer.cpp \
    viewerdatum.cpp \
    appworker.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

LIBS += -L$$PWD/3rd/windows/OSG/lib/ -losgText -losgShadow -losg -losgDB -losgViewer -losgGA -lOpenThreads -losgUtil -losgManipulator
LIBS += -L$$PWD/3rd/windows/OSG/dll/

LIBS += -L$$PWD/3rd/windows/OSGEarth/lib/ -losgEarth -losgEarthUtil -losgEarthSymbology -losgEarthFeatures -losgEarthAnnotation
LIBS += -L$$PWD/3rd/windows/OSGEarth/dll/
LIBS+=-lopengl32


INCLUDEPATH += $$PWD/3rd/windows/OSG/include
DEPENDPATH += $$PWD/3rd/windows/OSG/include

INCLUDEPATH += $$PWD/3rd/windows/OSGEarth/include
DEPENDPATH += $$PWD/3rd/windows/OSGEarth/include

INCLUDEPATH += $$PWD/3rd/windows/OSG_3rd/include
DEPENDPATH += $$PWD/3rd/windows/OSG_3rd/include
