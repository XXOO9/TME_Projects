#include "CSignPainterCommandDefin.h"

CSignPainterCmd::CSignPainterCmd()
{

}

QStringList CSignPainterCmd::controlCmdList()
{
    QStringList cmdList;
    cmdList << SIGN_CMD_POSTION << SIGN_CMD_DISPLAY << SIGN_CMD_HIDE << SIGN_CMD_CLEAR << SIGN_CMD_SAVE << SIGN_CMD_BLANK;
    return cmdList;
}
