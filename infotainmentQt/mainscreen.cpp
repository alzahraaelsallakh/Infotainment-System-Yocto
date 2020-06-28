#include "mainscreen.h"
#include "ui_mainscreen.h"
#include <iostream>
using namespace std;


/* Stack widget pages for main screen */
#define HOME_PAGE_INDEX 0
#define PHONE_PAGE_INDEX 1
#define FM_PAGE_INDEX 2
#define MUSIC_PAGE_INDEX 3
#define GPS_PAGE_INDEX 4
#define BLUETOOTH_PAGE_INDEX 5
#define SETTINGS_PAGE_INDEX 6


mainScreen::mainScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainScreen)
{
    ui->setupUi(this);

    /* Time and date setup */
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer->start();


    /* Setting home page as default page */
    ui->pagesSwitch->setCurrentIndex(HOME_PAGE_INDEX);

   /* Connecting home buttons to goBackHomeFunction */
   connect(ui->homeButtonPh,SIGNAL(clicked()),SLOT(goBackHome()));
   connect(ui->homeButtonFm,SIGNAL(clicked()),SLOT(goBackHome()));
   connect(ui->homeButtonMs,SIGNAL(clicked()),SLOT(goBackHome()));
   connect(ui->homeButtonGp,SIGNAL(clicked()),SLOT(goBackHome()));
   connect(ui->homeButtonBl,SIGNAL(clicked()),SLOT(goBackHome()));
   connect(ui->homeButtonSt,SIGNAL(clicked()),SLOT(goBackHome()));

   /* Setting buttons icons */
   ui->phoneButton->setIcon(QIcon(":/mainIcons/media/calls_icon.png"));
   ui->phoneButton->setIconSize(QSize(100,100));

   ui->fmButton->setIcon(QIcon(":/mainIcons/media/fm_icon.png"));
   ui->fmButton->setIconSize(QSize(100,100));

   ui->musicButton->setIcon(QIcon(":/mainIcons/media/music_icon.png"));
   ui->musicButton->setIconSize(QSize(100,100));

   ui->gpsButton->setIcon(QIcon(":/mainIcons/media/gps_icon.png"));
   ui->gpsButton->setIconSize(QSize(100,100));

   ui->bluetoothButton->setIcon(QIcon(":/mainIcons/media/bluetooth_icon.png"));
   ui->bluetoothButton->setIconSize(QSize(100,100));

   ui->settingsButton->setIcon(QIcon(":/mainIcons/media/settings_icon.png"));
   ui->settingsButton->setIconSize(QSize(100,100));

   ui->homeButtonPh->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonPh->setIconSize(QSize(70,70));
   ui->homeButtonFm->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonFm->setIconSize(QSize(70,70));
   ui->homeButtonMs->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonMs->setIconSize(QSize(70,70));
   ui->homeButtonGp->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonGp->setIconSize(QSize(70,70));
   ui->homeButtonBl->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonBl->setIconSize(QSize(70,70));
   ui->homeButtonSt->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
   ui->homeButtonSt->setIconSize(QSize(70,70));

}

mainScreen::~mainScreen()
{
    delete ui;
}

/* Time and date updating method */
void mainScreen::updateTime()
{
    currentDate = QDate::currentDate();
    date = currentDate.toString();
    ui->dateText->setText(date);
    ui->dateText->setAlignment(Qt::AlignRight);

    currentTime = QTime::currentTime();
    time = currentTime.toString("hh:mm AP");
    ui->timeText->setText(time);
    ui->timeText->setAlignment(Qt::AlignRight);

}

/* Home page buttons methos */
void mainScreen::on_phoneButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(PHONE_PAGE_INDEX);
}

void mainScreen::on_fmButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(FM_PAGE_INDEX);
}

void mainScreen::on_musicButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(MUSIC_PAGE_INDEX);
}

void mainScreen::on_gpsButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(GPS_PAGE_INDEX);
}

void mainScreen::on_bluetoothButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(BLUETOOTH_PAGE_INDEX);
}

void mainScreen::on_settingsButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(SETTINGS_PAGE_INDEX);
}

/* Mutual home button in every page */
void mainScreen::goBackHome (void)
{
    ui->pagesSwitch->setCurrentIndex(HOME_PAGE_INDEX);
}
