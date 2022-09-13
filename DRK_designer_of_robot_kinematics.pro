QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS    +=  -lopengl32
LIBS    +=  -lglu32

SOURCES += \
    basecentraldock.cpp \
    basedock.cpp \
    centralwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    messageWindows/newprojectconstructorwidget.cpp \
    openGL/glvisualizatorwidget.cpp \
    openGL/projectvisualizator.cpp \
    projectCore/baseprojectcontroller.cpp \
    projectCore/initAvailableControllers.cpp \
    projectCore/projectsmanager.cpp \
    serialManipulator/serialManipulatorController.cpp \
    serialManipulator/centralDocks/glcentraldock.cpp \
    serialManipulator/docks/manybuttonsdock.cpp

HEADERS += \
    basecentraldock.h \
    basedock.h \
    centralwindow.h \
    mainwindow.h \
    messageWindows/newprojectconstructorwidget.h \
    openGL/glvisualizatorwidget.h \
    openGL/projectvisualizator.h \
    projectCore/baseprojectcontroller.h \
    projectCore/projectType.h \
    projectCore/projectsmanager.h \
    serialManipulator/serialManipulatorController.h \
    serialManipulator/centralDocks/glcentraldock.h \
    serialManipulator/docks/manybuttonsdock.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
