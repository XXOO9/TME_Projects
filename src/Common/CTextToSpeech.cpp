#include "CFilePath.h"
#include "CTextToSpeech.h"
#include "commondefin.h"
#include <QtMultimedia/QSound>
#include <QTime>
#include <QCoreApplication>

QMutex CTextToSpeech::s_mutexInstance_Safe;
CTextToSpeech * CTextToSpeech::s_pInstance = nullptr;

CTextToSpeech::CTextToSpeech(QObject *parent)
    : QObject(parent)
{
    m_pTts = new QTextToSpeech();

    initTTS(m_pTts);
    connect(m_pTts, &QTextToSpeech::stateChanged, this, &CTextToSpeech::onSpeechStateChange, Qt::DirectConnection);
}

CTextToSpeech::~CTextToSpeech()
{
    if(m_pTts != nullptr)
    {
        disconnect(m_pTts, &QTextToSpeech::stateChanged, this, &CTextToSpeech::onSpeechStateChange);
        delete m_pTts;
    }
}

CTextToSpeech &CTextToSpeech::GetInstance()
{
    /// 1.--------------获取单实例唯一指针---------------
    if( s_pInstance == nullptr)
    {
        /// 1.1--------------多线程同时调用时，线程安全锁---------------
        s_mutexInstance_Safe.lock();

        /// 1.2--------------新建CConfig_Manager对象--------------------------
        if(s_pInstance == nullptr)
        {
            s_pInstance = new CTextToSpeech();
        }

        /// 1.3--------------多程程同步锁，解锁-------------------------
        s_mutexInstance_Safe.unlock();
    }

    /// 2.--------------返回对象指针---------------------
    return *s_pInstance;
}

QTextToSpeech::State CTextToSpeech::state() const
{
    if(m_pTts == nullptr)
        return QTextToSpeech::BackendError;

    return m_pTts->state();
}

void CTextToSpeech::textToSpeech(const QString &speakContent)
{
    if(m_pTts == nullptr || speakContent.isEmpty())
        return;

    //if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }
    m_pTts->say( speakContent );
}

void CTextToSpeech::textToSpeech(const QString &speakContent, int waitTimeMsec)
{
    if(m_pTts == nullptr || speakContent.isEmpty())
        return;

    //if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }

    emit signalSpeakStateChange(true);

    m_pTts->say( speakContent );

    if(waitTimeMsec > 0)
    {

        QTime timeTarget = QTime::currentTime().addMSecs(waitTimeMsec);
        //非阻塞循环,等待准备语音完成,同时设置超时等待（5s）
        while(QTime::currentTime() < timeTarget)
        {
            //获取当前时间
            QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
        }
    }

    emit signalSpeakStateChange(false);
}

void CTextToSpeech::textToSpeechBlock(const QString &speakContent)
{
    if(m_pTts == nullptr || speakContent.isEmpty())
        return;

    emit signalSpeakStateChange(true);

    //if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }
    m_pTts->say( speakContent );

    //设置最长等待时间为一分钟
    QTime timeTarget = QTime::currentTime().addMSecs(60000);
    setSpeakState(true);

    while(m_bSpeakState && QTime::currentTime() < timeTarget)
    {
        //获取当前时间
        QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    emit signalSpeakStateChange(false);
}

//使用 默认路径 及 默认等待时间
void CTextToSpeech::playStartSpeech(const QString &qsVoiceName/* = ""*/, const int &waitTimeMsec/* = 6500*/)
{
    //0.安全保护，避免语音重叠
    if(m_pTts == nullptr)
    {
        return;
    }

    emit signalSpeakStateChange(true);

    if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }

    //1.当路径为空时，则使用默认路径
    string strFilePath = CFilePath::getAppDirPath() + "/" + "voice/";
    QString qsPath;
    if(qsVoiceName == "")
    {
        qsPath = QString::fromStdString(strFilePath) + "StartVoice.wav";
    }
    else
    {
        qsPath = QString::fromStdString(strFilePath) + qsVoiceName;
    }

    QSound::play(qsPath);

    //2.设置阻塞等待时间
    if(waitTimeMsec > 0)
    {
        QTime timeTarget = QTime::currentTime().addMSecs(waitTimeMsec);
        //非阻塞循环,等待准备语音完成,同时设置超时等待（5s）
        while(QTime::currentTime() < timeTarget)
        {
            //获取当前时间
            QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
        }
    }

    emit signalSpeakStateChange(false);
}

//使用 默认路径
void CTextToSpeech::playEndSpeech(const QString &qsVoiceName/* = ""*/, const int &waitTimeMsec/* = 1000*/)
{
    //0.安全保护，避免语音重叠
    if(m_pTts == nullptr)
    {
        return;
    }

    if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }

    //1.当路径为空时，则使用默认路径
    string strFilePath = CFilePath::getAppDirPath() + "/" + "voice/";
    QString qsPath;
    if(qsVoiceName == "")
    {
        qsPath = QString::fromStdString(strFilePath) + "EndVoice.wav";
    }
    else
    {
        qsPath = QString::fromStdString(strFilePath) + qsVoiceName;
    }

    QSound::play(qsPath);

    //2.设置阻塞等待时间
    if(waitTimeMsec > 0)
    {
        QTime timeTarget = QTime::currentTime().addMSecs(waitTimeMsec);
        //非阻塞循环,等待准备语音完成,同时设置超时等待（5s）
        while(QTime::currentTime() < timeTarget)
        {
            //获取当前时间
            QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
        }
    }
}

void CTextToSpeech::playCountdownSpeech(const unsigned int &iCountNum)
{
    //0.安全保护
    if(nullptr == m_pTts)
    {
        return;
    }

    emit signalSpeakStateChange(true);

    //1.停止其他语音
    if( m_pTts->state() == QTextToSpeech::Speaking )
    {
        m_pTts->stop();
    }

    //2.间隔播放语音
    int iCurrentNum = static_cast<int>(iCountNum);
    QString qsSpeakContent;
    QTime timeTarget;
    for (;iCurrentNum > 0; iCurrentNum--)
    {
        qsSpeakContent = QString::number(iCurrentNum);
        m_pTts->say( qsSpeakContent );
        timeTarget = QTime::currentTime().addMSecs(1000);
        while(QTime::currentTime() < timeTarget)
        {
            QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
        }
    }

    emit signalSpeakStateChange(false);
}

void CTextToSpeech::stop()
{
    if(m_pTts == nullptr)
        return;

    m_pTts->stop();
}

void CTextToSpeech::initTTS(QTextToSpeech *pTTS)
{
    if(pTTS == nullptr)
        return;

    QVector<QLocale> locales = pTTS->availableLocales();

    int nIndex = -1;
    for(int i = 0; i < locales.size(); i++)
    {
        nIndex = 0;
        if(locales[i].name().contains("zh_CN"))
        {
            nIndex = i;
            break;
        }
    }

    if(nIndex >= 0)
    {
        pTTS->setLocale( locales[nIndex] );
    }
    //设置语言环境之后
    nIndex = -1;
    auto voices = pTTS->availableVoices();
    for(int i = 0; i < voices.size(); i++)
    {
        nIndex = 0;
        if(voices[i].name().contains("hui"))
        {
            nIndex = i;
            break;
        }
    }

    if(nIndex >= 0)
    {
        pTTS->setVoice(voices[nIndex]);
    }

    //设置语速
    pTTS->setRate( 0.1 );

    //设置音高
    pTTS->setPitch( 1.0 );

    //设置音量
    pTTS->setVolume( 1.0 );
}

void CTextToSpeech::onSpeechStateChange(QTextToSpeech::State state)
{
    setSpeakState(state == QTextToSpeech::Speaking);
}
