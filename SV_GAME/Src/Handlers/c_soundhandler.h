#ifndef C_SOUNDHANDLER_H
#define C_SOUNDHANDLER_H

#include <QObject>
#include <QMediaPlayer>

/* Must be implemented with Str_Sounds */
enum E_SOUNDS{
    musicEntrance,
    musicGameFinished,
    screenOpen,
    draw,
    hover,
    cardSelection,
    kill,
    notify,
    soundsNumber
};

class C_SoundHandler : public QObject
{
    Q_OBJECT
public:
    C_SoundHandler(QObject *parent = nullptr);
    C_SoundHandler(C_SoundHandler &other) = delete;
    void operator=(const C_SoundHandler &) = delete;
    ~C_SoundHandler();

public:
    int getGeneralSound()const{ return 100*mGlobalVolume; };
    const bool &getEnableMusic()const{ return mEnableMusic; };
    const bool &getEnableSounds()const{ return mEnableSounds; };

public:
    void playSound(const E_SOUNDS &sound)const;
    void stopSound(const E_SOUNDS &sound)const;

private:
    void initialize();

public slots:
    void onSetGlobalVolume(int valuePourcent);
    void onEnableMusic(bool toggle);
    void onEnableSounds(bool toggle);

public:
    static C_SoundHandler *getInstance();

protected:
    static C_SoundHandler *mSoundHandler;

private:
    QMediaPlayer *mSounds[soundsNumber];
    qreal mGlobalVolume;
    bool mEnableMusic;
    bool mEnableSounds;
};

#endif // C_SOUNDHANDLER_H
