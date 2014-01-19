APP_NAME = GoldenRetriever

CONFIG += qt warn_on cascades10
INCLUDEPATH += ../src ../../canadainc/src/ ../../golden/src/
LIBS += -lbbutilityi18n -lbb -lbbdata -lbbsystem -lbbpim

CONFIG(release, debug|release) {
    DESTDIR = o.le-v7
    LIBS += -L../../canadainc/arm/o.le-v7 -lcanadainc -Bdynamic
    LIBS += -L../../golden/arm/o.le-v7 -lgolden -Bdynamic
}

CONFIG(debug, debug|release) {
    DESTDIR = o.le-v7-g
    LIBS += -L../../canadainc/arm/o.le-v7-g -lcanadainc -Bdynamic
    LIBS += -L../../golden/arm/o.le-v7-g -lgolden -Bdynamic
}

simulator {
CONFIG(release, debug|release) {
    DESTDIR = o
    LIBS += -Bstatic -L../../canadainc/x86/o-g/ -lcanadainc -Bdynamic
    LIBS += -Bstatic -L../../golden/x86/o-g/ -lgolden -Bdynamic     
}
CONFIG(debug, debug|release) {
    DESTDIR = o-g
    LIBS += -Bstatic -L../../canadainc/x86/o-g/ -lcanadainc -Bdynamic
    LIBS += -Bstatic -L../../golden/x86/o-g/ -lgolden -Bdynamic
}
}

include(config.pri)