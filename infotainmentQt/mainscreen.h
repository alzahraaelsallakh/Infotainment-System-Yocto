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

#include <unistd.h>


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

    QDate currentDate;
    QString date;
    QTime currentTime;
    QString time;

    QMediaPlaylist *playList = new QMediaPlaylist;
    QMediaPlayer *musicPlayer = new QMediaPlayer;

    ~mainScreen();

private slots:
    void on_phoneButton_clicked();

    void on_fmButton_clicked();

    void on_musicButton_clicked();

    void on_gpsButton_clicked();

    void on_bluetoothButton_clicked();

    void on_settingsButton_clicked();

    void goBackHome();

    void updateTime();

    void updateFlashStatus();

    void updateSongsList();

    void on_playButton_clicked();

    void on_forwardButton_clicked();

    void on_backwardButton_clicked();

    void on_repeatButton_clicked();

    void on_shuffleButton_clicked();

    void on_stopButton_clicked();

    void playSong();

    void playToggle();

    void onListMailItemClicked(QListWidgetItem* item);

    void onSongChange();

    void on_volumeDownButton_clicked();

    void on_volumeUpButton_clicked();

    void on_darkThemeButton_clicked();

private:
    Ui::mainScreen *ui;
};

#endif // MAINSCREEN_H
