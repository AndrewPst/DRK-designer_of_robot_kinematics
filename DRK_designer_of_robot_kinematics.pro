QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS    +=  -lopengl32
LIBS    +=  -lglu32

SOURCES += \
    CentralDocks/exemplecentraldock.cpp \
    CentralDocks/glcentraldock.cpp \
    CentralDocks/titleBars/splittertitlebar.cpp \
    basecentraldock.cpp \
    basedock.cpp \
    centralwindow.cpp \
    dockWidgets/manipulatoreditordock.cpp \
    dockWidgets/manybuttonsdock.cpp \
    main.cpp \
    mainwindow.cpp \
    openGL/baseglobject.cpp \
    openGL/glfield.cpp \
    openGL/globjectscontainer.cpp \
    openGL/glvisualizatorwidget.cpp \
    openGL/projectvisualizator.cpp \
    projectCore/projectStructure.cpp \
    projectCore/projectcore.cpp

HEADERS += \
    CentralDocks/CentralDockType.h \
    CentralDocks/exemplecentraldock.h \
    CentralDocks/glcentraldock.h \
    CentralDocks/titleBars/splittertitlebar.h \
    basecentraldock.h \
    basedock.h \
    centralwindow.h \
    dockWidgets/manipulatoreditordock.h \
    dockWidgets/manybuttonsdock.h \
    mainwindow.h \
    openGL/baseglobject.h \
    openGL/glfield.h \
    openGL/globjectscontainer.h \
    openGL/glvisualizatorwidget.h \
    openGL/projectvisualizator.h \
    projectCore/projectStructure.h \
    projectCore/projectcore.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
