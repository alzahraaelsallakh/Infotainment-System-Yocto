#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>


/* Time & Date inclusions */
#include <QTimer>
#include <QDate>
#include <QTime>

/* Media inclusions */
#include <QIcon>

/* Music inclusions */
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDirIterator>
#include <QDir>
#include <QUrl>

/* MP4 inclusions */
#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>

#include <unistd.h>
#include <bits/stdc++.h>
#include <regex>

#include <iostream>
using namespace std;
#include <typeinfo>
#include <QDebug>


namespace Ui {
class mainScreen;
}

class mainScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainScreen(QWidget *parent = 0);

    QTimer *timer;

    QTimer *flashDetectionTimer;

    QTimer * bluetoothDetectionTimer;

    QDate currentDate;
    QString date;
    QTime currentTime;
    QString time;

    QMediaPlaylist *playList = new QMediaPlaylist;
    QMediaPlayer *musicPlayer = new QMediaPlayer;

    QMediaPlayer *videoPlayer = new QMediaPlayer;
    QMediaPlaylist *videoPlayList = new QMediaPlaylist;
    QGraphicsScene *videoScene = new QGraphicsScene;
    QGraphicsVideoItem *videoGraphicsItem = new QGraphicsVideoItem;

    ~mainScreen();

private slots:

    void on_videoButton_clicked();

    void on_musicButton_clicked();

    void on_bluetoothButton_clicked();

    void on_settingsButton_clicked();

    void goBackHome();

    void updateTime();

    void updateFlashStatus();

    void updatBluetoothDevices();

    void updateSongsList();

    void on_playButton_clicked();

    void on_forwardButton_clicked();

    void on_backwardButton_clicked();

    void on_repeatButton_clicked();

    void on_shuffleButton_clicked();

    void on_stopButton_clicked();

    void playSong();

    void playToggle();

    void onSongListItemClicked(QListWidgetItem* item);

    void onSongChange();

    void on_volumeDownButton_clicked();

    void on_volumeUpButton_clicked();

    void on_darkThemeButton_clicked();

     void updateVideosList();

    void on_goTovideosListButton_clicked();

    void onVideoListItemClicked(QListWidgetItem* item);

    void on_backwardVideoButton_clicked();

    void on_forwardVideoButton_clicked();

    void on_playVideoButton_clicked();

    void on_stopVideoButton_clicked();

    void on_volumeDownVideoButton_clicked();

    void on_volumeUpVideoButton_clicked();

    void playVideo();

    void stopVideo();

    void enableBluetooth();

    void on_setTimeDateButton_clicked();

    void on_doneTimeDateButton_clicked();

    void on_cancelTimeDateButton_clicked();

private:
    Ui::mainScreen *ui;
};



#endif // MAINSCREEN_H
