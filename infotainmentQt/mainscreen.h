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
#include <QThread>


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

private:
    Ui::mainScreen *ui;
};

#endif // MAINSCREEN_H
