#include "CSyncClassInfoRet.h"

CSyncClassInfoRet::CSyncClassInfoRet()
{

}

CSyncClassInfoRet::~CSyncClassInfoRet()
{

}

QString CSyncClassInfoRet::getClassName(QString classId)
{
    for( int i = 0; i < size(); i++ ){
        QMap<QString, QString> classInfo = getRetMapByIndex( i );
        if( classId == classInfo.value( ClassInfoKeys::id ) ){
            QString className =  classInfo.value( ClassInfoKeys::className );
            return className;
        }
    }
    return "";
}
