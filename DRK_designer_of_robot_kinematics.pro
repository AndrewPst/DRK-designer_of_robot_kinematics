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
    messageWindows/ConfirmActionDialog.cpp \
    messageWindows/newprojectconstructorwidget.cpp \
    serial6DofManipulator/centralDocks/glcentraldock.cpp \
    serial6DofManipulator/docks/dhtabledock.cpp \
    serial6DofManipulator/docks/kinematicsdock.cpp \
    serial6DofManipulator/docks/manybuttonsdock.cpp \
    serial6DofManipulator/logic/kinematics.cpp \
    serial6DofManipulator/logic/manipulatorcontroller.cpp \
    serial6DofManipulator/logic/models/joint_t.cpp \
    serial6DofManipulator/openGL/glvisualizatorwidget.cpp \
    serial6DofManipulator/openGL/projectvisualizator.cpp \
    serial6DofManipulator/serial6dofmanipulator.cpp \
    projectCore/baseprojectcontroller.cpp \
    projectCore/initAvailableControllers.cpp \
    projectCore/projectsmanager.cpp \
    version_t.cpp

HEADERS += \
    basecentraldock.h \
    basedock.h \
    centralwindow.h \
    mainwindow.h \
    messageWindows/ConfirmActionDialog.h \
    messageWindows/newprojectconstructorwidget.h \
    projectCore/projectGenerator.h \
    projectCore/projectMetadata.h \
    serial6DofManipulator/centralDocks/glcentraldock.h \
    serial6DofManipulator/docks/dhtabledock.h \
    serial6DofManipulator/docks/kinematicsdock.h \
    serial6DofManipulator/docks/manybuttonsdock.h \
    serial6DofManipulator/logic/kinematics.h \
    serial6DofManipulator/logic/manipulatorcontroller.h \
    serial6DofManipulator/logic/models/DHTable_t.h \
    serial6DofManipulator/logic/models/Effector_t.h \
    serial6DofManipulator/logic/models/joint_t.h \
    serial6DofManipulator/logic/models/matrix.h \
    serial6DofManipulator/logic/models/units_t.h \
    serial6DofManipulator/openGL/glvisualizatorwidget.h \
    serial6DofManipulator/openGL/projectvisualizator.h \
    serial6DofManipulator/serial6dofmanipulator.h \
    projectCore/baseprojectcontroller.h \
    projectCore/projectsmanager.h \
    version_t.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
