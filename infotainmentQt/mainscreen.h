#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>
#include <QTimer>

#include <QDate>
#include <QTime>

#include <QIcon>

namespace Ui {
class mainScreen;
}

class mainScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainScreen(QWidget *parent = 0);

    QTimer *timer;

    QDate currentDate;
    QString date;
    QTime currentTime;
    QString time;

    QIcon phoneIcon;

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

private:
    Ui::mainScreen *ui;
};

#endif // MAINSCREEN_H
