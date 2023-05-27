#include "c_soundhandler.h"
#include "../../Shared/debug.h"
#include "c_json.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

/* Must follow E_SOUNDS enum */
const QString Str_Sounds[] = {
    "Music_Entrance",
    "Music_GameFinished",
    "Sound_Clicked",
    "Sound_Draw",
    "Sound_Hover",
    "Sound_CardSelection",
    "Sound_Kill",
    "Sound_Notify",
    "Sound_Unlock",
    "Sound_EndGame"
};

C_SoundHandler *C_SoundHandler::mSoundHandler = nullptr;

C_SoundHandler::C_SoundHandler(QObject *parent)
    : QObject(parent)
    , mGlobalVolume(1.0)
    , mEnableMusic(true)
    , mEnableSounds(true)
{
    mSoundHandler = this;

    initialize();
}

C_SoundHandler::~C_SoundHandler()
{    
    C_Json::saveParameter("Enable music", mEnableMusic);
    C_Json::saveParameter("Enable sounds", mEnableSounds);
    C_Json::saveParameter("Volume", mGlobalVolume*100);

    for(int i = 0; i < soundsNumber; i++)
        delete mSounds[i];
}

void C_SoundHandler::playSound(const E_SOUNDS &sound) const
{
    if(!mEnableSounds && sound > musicGameFinished)
        return;

    if(!mEnableMusic && sound <= musicGameFinished)
        return;

    if(sound >= soundsNumber)
    {
        LOG_DBG("Sound can not be played - Out of bounds");
        return;
    }

    if(mSounds[sound]->state() == QMediaPlayer::State::PlayingState)
        mSounds[sound]->setPosition(0);
    else
        mSounds[sound]->play();
}

void C_SoundHandler::stopSound(const E_SOUNDS &sound) const
{
    if(sound >= soundsNumber)
    {
        LOG_DBG("Sound can not be played - Out of bounds");
        return;
    }

    mSounds[sound]->stop();
}

void C_SoundHandler::initialize()
{
    for(int i = 0; i < soundsNumber; i++)
    {
        mSounds[i] = new QMediaPlayer(this);
        mSounds[i]->setMedia(QUrl(QString("qrc:/sounds/%1.wav").arg(Str_Sounds[i])));
        mSounds[i]->setVolume(100);
    }

    C_Json::loadParameter("Volume", &mGlobalVolume);
    C_Json::loadParameter("Enable music", &mEnableMusic);
    C_Json::loadParameter("Enable sounds", &mEnableSounds);

    onSetGlobalVolume(mGlobalVolume);
}

void C_SoundHandler::onSetGlobalVolume(int valuePourcent)
{
    mGlobalVolume = valuePourcent/100.0;
    for(int i = 0; i < soundsNumber; i++)
    {
        switch(i)
        {
            case musicEntrance:
            case musicGameFinished:
                mSounds[i]->setVolume(10*mGlobalVolume);
                break;

            case hover:
                mSounds[i]->setVolume(20*mGlobalVolume);
                break;

            default:
                mSounds[i]->setVolume(100*mGlobalVolume);
                break;
        }
    }
}

void C_SoundHandler::onEnableMusic(bool toggle)
{
    mEnableMusic = toggle;
    if(!mEnableMusic)
    {
        stopSound(E_SOUNDS::musicEntrance);
        stopSound(E_SOUNDS::musicGameFinished);
    }
}

void C_SoundHandler::onEnableSounds(bool toggle)
{
    mEnableSounds = toggle;
};

C_SoundHandler *C_SoundHandler::getInstance()
{
    if(!mSoundHandler)
        return mSoundHandler = new C_SoundHandler();
    else
        return mSoundHandler;
}
