HEADERS += \
    qtglrenderres.h \
    ctrlwindow.h \
    qsliderspinbox.h \
    videohandler.h \
    videosurface.h \
    prewindow.h

SOURCES += \
    qtglrenderres.cpp \
    main.cpp \
    ctrlwindow.cpp \
    qsliderspinbox.cpp \
    videohandler.cpp \
    videosurface.cpp \
    prewindow.cpp

QT += multimedia multimediawidgets

 QT           += opengl

OTHER_FILES += \
    Basic.vsh \
    Basic.fsh

RESOURCES += \
    Shaders.qrc
