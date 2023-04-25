#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H


#include "common_global.h"
#include <QObject>
#include <QTextToSpeech>
#include <QDebug>
#include <QMutex>

//在导出库中使用存在一定问题，在主工程中创建qtexttospeech之后放到设置到当前

//主要需要的功能有：等待某个语音自然完成
//               设置等待时间，时间结束本次语音结束
//               不等待直接进行播报语音
class COMMONSHARED_EXPORT CTextToSpeech : public QObject
{
    Q_OBJECT
private:
    explicit CTextToSpeech(QObject *parent = nullptr);
    ~CTextToSpeech();
    CTextToSpeech(const CTextToSpeech &manager);
    CTextToSpeech &operator = (const CTextToSpeech &manager);

    /**
    * 静态实例对象
    */
    static CTextToSpeech		*s_pInstance;
    static QMutex				s_mutexInstance_Safe;
public:
    static CTextToSpeech &GetInstance();

    //设置完成直接返回 不进行等待
    void textToSpeech(const QString &speakContent);

    //等待时间结束函数完成
    void textToSpeech(const QString &speakContent, int waitTimeMsec);

    //等待语音完成函数结束
    void textToSpeechBlock(const QString &speakContent);

    //播放开始语音
    void playStartSpeech(const QString &qsVoiceName = "", const int &waitTimeMsec = 6500);

    //播放结束语音
    void playEndSpeech(const QString &qsVoiceName = "", const int &waitTimeMsec = 1000);

    //阻塞倒计时语音(例:10 9 8 ...)
    void playCountdownSpeech(const unsigned int &iCountNum);

    void stop();

    QTextToSpeech::State state() const;

    void initTTS(QTextToSpeech *pTTS);

    bool isInSpeaking(){ return m_bSpeakState; }
protected:
    inline void setSpeakState(const bool &bSpeakState){m_bSpeakState = bSpeakState;}

private slots:
    void onSpeechStateChange(QTextToSpeech::State state);

signals:
    //
    void signalSpeakStateChange(bool bSpeak);
private:
    QTextToSpeech   *m_pTts{nullptr};
    bool m_bSpeakState{false};
};

#endif // TEXTTOSPEECH_H
