#include "mainscreen.h"
#include "ui_mainscreen.h"
#include "systeminterface.h"
#include <regex>

/* For debugging only */
#include <iostream>
using namespace std;
#include <typeinfo>
#include <QDebug>



/* Stack widget pages for main screen */
#define HOME_PAGE_INDEX 0
#define PHONE_PAGE_INDEX 1
#define FM_PAGE_INDEX 2
#define MUSIC_PAGE_INDEX 3
#define GPS_PAGE_INDEX 4
#define BLUETOOTH_PAGE_INDEX 5
#define SETTINGS_PAGE_INDEX 6

/* Music Macros */
#define NO_FLASH_DETECTED   0
#define FLASH_DETECTED      1

/* Music variables */
int flashStatus = NO_FLASH_DETECTED;
string usbName;

mainScreen::mainScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainScreen)
{
    ui->setupUi(this);

    /* Time and date setup */
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer->start();

    /* Flash USB detection */
    flashDetectionTimer = new QTimer(this);
    connect(flashDetectionTimer,SIGNAL(timeout()),this,SLOT(updateFlashStatus()));
    flashDetectionTimer->start();

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
    ui->homeButtonPh->setIconSize(QSize(50,50));
    ui->homeButtonFm->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonFm->setIconSize(QSize(50,50));
    ui->homeButtonMs->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonMs->setIconSize(QSize(50,50));
    ui->homeButtonGp->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonGp->setIconSize(QSize(50,50));
    ui->homeButtonBl->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonBl->setIconSize(QSize(50,50));
    ui->homeButtonSt->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonSt->setIconSize(QSize(50,50));

    /* Music control icons */
    ui->playButton->setIcon(QIcon(":/musicControl/media/play_icon.png"));
    ui->playButton->setIconSize(QSize(40,40));
    ui->forwardButton->setIcon(QIcon(":/musicControl/media/forward_icon.png"));
    ui->forwardButton->setIconSize(QSize(40,40));
    ui->backwardButton->setIcon(QIcon(":/musicControl/media/backward_icon.png"));
    ui->backwardButton->setIconSize(QSize(40,40));
    ui->pauseButton->setIcon(QIcon(":/musicControl/media/pause_icon.png"));
    ui->pauseButton->setIconSize(QSize(40,40));
    ui->shuffleButton->setIcon(QIcon(":/musicControl/media/shuffle_icon.png"));
    ui->shuffleButton->setIconSize(QSize(40,40));
    ui->repeatButton->setIcon(QIcon(":/musicControl/media/repeat_icon.png"));
    ui->repeatButton->setIconSize(QSize(40,40));
}



mainScreen::~mainScreen()
{
    delete ui;
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

    /*
    updateSongsList();
    */


    /* Monitor control buttons press and play songs */

    /* find command -> path -> to run music */
    /*
    listingSongsCMD = "find " + usbPath + " -type f -name '*.mp3'";
    songsList = GetStdoutFromCommand(listingSongsCMD);
    cout << "List: " << songsList<<endl;
    GetStdoutFromCommand ("mpg123 "+songsList + "&");
    */

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

/* Flash status updating method */
void mainScreen::updateFlashStatus()
{

    string listDevicesCMD, mkdirCMD, mountCMD,rmdirCMD,umountCMD;
    string devicesConnected;

    /* Check if any flash usb is connected */
    //    listDevicesCMD = "ls /dev/ | grep sd[a-z][0-9]";
    listDevicesCMD = "ls /dev/ | grep sdb[0-9]";
    devicesConnected = GetStdoutFromCommand(listDevicesCMD);

    /* New usb is connected */
    if (!devicesConnected.empty() && flashStatus == NO_FLASH_DETECTED)
    {
        flashStatus = FLASH_DETECTED;
        usbName = devicesConnected;

        ui->noFlashGroup->hide();
        ui->runningMusicGroup->show();
        updateSongsList();
    }
    /* Current usb is removed */
    else if (devicesConnected.empty() && flashStatus == FLASH_DETECTED)
    {
        flashStatus = NO_FLASH_DETECTED;

        ui->runningMusicGroup->hide();
        ui->noFlashGroup->show();
        updateSongsList();
    }
    /* No usb is connected */
    else if (devicesConnected.empty() && flashStatus == NO_FLASH_DETECTED)
    {
        ui->runningMusicGroup->hide();
        ui->noFlashGroup->show();
    }

}

/* Updating songs list method */
void mainScreen::updateSongsList()
{
    int currentSongsNumber, start,end;
    string usbPath, songsListCount, songsList, song, songsExt;
    string listingSongsNumCMD, listingSongsCMD;

    songsExt = ".mp3";

    //    usbPath = "/run/media/" + usbName + "/";
    usbPath = "/media/Stuff/" + usbName + "/" ;
    currentSongsNumber = ui->runningSongsList->count();

    if (flashStatus == NO_FLASH_DETECTED)
    {
        if (currentSongsNumber != 0)
        {
            ui->runningSongsList->clear();
        }
    }
    else
    {
        listingSongsNumCMD = "ls " + usbPath + " | grep " + songsExt + " | wc -l";
        try
        {
            songsListCount = GetStdoutFromCommand(listingSongsCMD);
        }
        catch (int e)
        {
            cout << "It's okay" << endl;
            return;
        }

        cout << songsListCount << endl;

        if (songsListCount == "0")
        {
            ui->runningSongsList->hide();
            ui->zeroSongsGroup->show();
        }
        else
        {
            ui->zeroSongsGroup->hide();
            ui->runningSongsList->show();

            /* Just to make sure that any old displayed sone is removed */
            ui->runningSongsList->clear();

            /* Add existing songs to list */
            listingSongsCMD = "ls " + usbPath + " | grep " + songsExt;
            cout << "CMD: " << listingSongsCMD;
            try
            {
                songsList = GetStdoutFromCommand(listingSongsCMD);
            }
            catch (int e)
            {
                cout << "It's okay" << endl;
                return;
            }

            start = 0;
            while (start < songsList.length())
            {
                end = songsList.find(songsExt,start);
                song = songsList.substr(start,(end-start));

                ui->runningSongsList->addItem( QString::fromStdString(song));
                ui->runningSongsList->setSpacing(7);

                start = end + songsExt.length();
            }



        }

    }

}




