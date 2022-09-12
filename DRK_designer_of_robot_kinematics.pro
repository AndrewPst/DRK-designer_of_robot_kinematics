QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS    +=  -lopengl32
LIBS    +=  -lglu32

SOURCES += \
    CentralDocks/glcentraldock.cpp \
    basecentraldock.cpp \
    basedock.cpp \
    centralwindow.cpp \
    dockWidgets/manybuttonsdock.cpp \
    main.cpp \
    mainwindow.cpp \
    openGL/baseglobject.cpp \
    openGL/glfield.cpp \
    openGL/globjectscontainer.cpp \
    openGL/glvisualizatorwidget.cpp \
    openGL/projectvisualizator.cpp \
    projectCore/baseprojectcontroller.cpp \
    projectCore/initAvailableControllers.cpp \
    projectCore/projectControllers/serialmanipulatorprojectcontroller.cpp \
    projectCore/projectsmanager.cpp

HEADERS += \
    CentralDocks/glcentraldock.h \
    basecentraldock.h \
    basedock.h \
    centralwindow.h \
    dockWidgets/manybuttonsdock.h \
    mainwindow.h \
    openGL/baseglobject.h \
    openGL/glfield.h \
    openGL/globjectscontainer.h \
    openGL/glvisualizatorwidget.h \
    openGL/projectvisualizator.h \
    projectCore/baseprojectcontroller.h \
    projectCore/projectControllers/serialmanipulatorprojectcontroller.h \
    projectCore/projectType.h \
    projectCore/projectsmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
