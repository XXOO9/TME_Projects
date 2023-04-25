#ifndef PROJECTMETAENUMS_H
#define PROJECTMETAENUMS_H

#include <QObject>

class ProjectMetaEnums : public QObject
{
    Q_OBJECT
public:

    enum cardDetectState{
        NO_Card = 1,
        Exist_Card_UnBind,
        Exist_Card_Bind,
    };
    Q_ENUM( cardDetectState )

    enum cardUserBindState{
       UserBind = 1,
       UserUnBind,
    };
    Q_ENUM( cardUserBindState )

    enum cardClassBindState{
       ClassBind = 1,
       ClassUnBind,
    };
    Q_ENUM( cardClassBindState )

    enum bindBtnState{
        BindBtn_Disable = 1,
        BindBtn_Ready_Bind,
        BindBtn_Ready_DisBind
    };
    Q_ENUM( bindBtnState )

    enum PageIndex{
        Page_User_Bind = 0,
        Page_Class_Bind,
        Page_Login,
    };
    Q_ENUM( PageIndex )

    enum InputType{
        STUDENTID = 0,
        STUDENTNAME,
        PASSWD,
    };
    Q_ENUM( InputType )

    enum tableType{
        User_Bind_Table = 0,
        Class_Bind_Table,
    };
    Q_ENUM( tableType )

    enum CardType{
        School_Card = 0,
        Persional_Card,
    };
    Q_ENUM( CardType )



    explicit ProjectMetaEnums(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PROJECTMETAENUMS_H
