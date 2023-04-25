TEMPLATE = subdirs

SUBDIRS += \
    App \
    Common \
    Communication \
    Core \
#    STE-100-BindCard-New \
    SignPainter \
#    STE-100-BindCard       ####  弃用
    UnitTest

    Core.depends = Communication    Common
    App.depends = Communication    Common   Core
    SignPainter.depends =  Common
    UnitTest.depends =  Common  Communication   Core

