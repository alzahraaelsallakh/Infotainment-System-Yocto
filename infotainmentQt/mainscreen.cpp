#include "mainscreen.h"
#include "ui_mainscreen.h"
#include "systeminterface.h"
#include <bits/stdc++.h>


#include <iostream>
using namespace std;
#include <typeinfo>
#include <QDebug>


#define USING_PC    1
#define USING_PI    2
#define USING_DEV   USING_PC


/* System Macros */
#define COMMAND_SUCCEDED    0

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
#define MAX_VOLUME          100
#define MIN_VOLUME          0

/* Music variables */
int flashStatus = NO_FLASH_DETECTED;
string usbName, usbPath;
string songsExt = ".mp3";
int playingSongFlag = 0;
int currentVolume = 50;
int volumeStep = 10;

int mountUSB (void);
int unmountUSB(void);



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

    /* Setting home buttons icons */
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

    /* Setting default music pages */
    ui->runningMusicGroup->hide();

    /* Setting music control buttons icons */
    ui->playButton->setIcon(QIcon(":/musicControl/media/play_icon.png"));
    ui->playButton->setIconSize(QSize(40,40));
    ui->forwardButton->setIcon(QIcon(":/musicControl/media/forward_icon.png"));
    ui->forwardButton->setIconSize(QSize(40,40));
    ui->backwardButton->setIcon(QIcon(":/musicControl/media/backward_icon.png"));
    ui->backwardButton->setIconSize(QSize(40,40));
    ui->stopButton->setIcon(QIcon(":/musicControl/media/stop_icon.png"));
    ui->stopButton->setIconSize(QSize(40,40));
    ui->shuffleButton->setIcon(QIcon(":/musicControl/media/shuffle_icon.png"));
    ui->shuffleButton->setIconSize(QSize(40,40));
    ui->repeatButton->setIcon(QIcon(":/musicControl/media/repeat_icon.png"));
    ui->repeatButton->setIconSize(QSize(40,40));
    ui->volumeUpButton->setIcon(QIcon(":/musicControl/media/volumeUp_icon.png"));
    ui->volumeUpButton->setIconSize(QSize(30,30));
    ui->volumeDownButton->setIcon(QIcon(":/musicControl/media/volumeDown_icon.png"));
    ui->volumeDownButton->setIconSize(QSize(30,30));

    /* Music player signals connections */
    connect(ui->runningSongsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListMailItemClicked(QListWidgetItem*)));
    connect(playList,SIGNAL(currentIndexChanged(int)),SLOT(onSongChange()));
}



mainScreen::~mainScreen()
{
    delete ui;
}


/*****************************************************************************************************************************/
/************************************************    General Methods    ******************************************************/
/*****************************************************************************************************************************/


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

/* Mutual home button in every page */
void mainScreen::goBackHome (void)
{
    ui->pagesSwitch->setCurrentIndex(HOME_PAGE_INDEX);
}


/*****************************************************************************************************************************/
/*************************************************    Music Methods    *******************************************************/
/*****************************************************************************************************************************/

void mainScreen::on_musicButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(MUSIC_PAGE_INDEX);
}

int mountUSB (void)
{
    const char *cmd;
    string mountCMD;

    mountCMD = "mount /dev/" + usbName + " /media/" + usbName;

    cmd = mountCMD.c_str();
    return system (cmd);
}

int unmountUSB ()
{
    const char *cmd;
    string umountCMD;

    umountCMD = "umount /media/" + usbName;

    cmd = umountCMD.c_str();
    return system (cmd);
}

/* Flash status updating method */
void mainScreen::updateFlashStatus()
{
    int systemStatus;
    string listDevicesCMD, mkdirCMD, mountCMD,rmdirCMD,umountCMD;
    string devicesConnected;
    const char *cmd;

    /* Check if any flash usb is connected */
#if USING_DEV == USING_PC
    listDevicesCMD = "ls /dev/ | grep sdb[0-9]";
#endif
#if USING_DEV == USING_PI
    listDevicesCMD = "ls /dev/ | grep sd[a-z][0-9]";
#endif

    devicesConnected = GetStdoutFromCommand(listDevicesCMD);

    /* New usb is connected */
    if (!devicesConnected.empty() && flashStatus == NO_FLASH_DETECTED)
    {
        flashStatus = FLASH_DETECTED;
        usbName = devicesConnected;

#if USING_DEV == USING_PI

        usbPath = "/media/" + usbName + "/";

        /* Mounting usb device */

        mkdirCMD = "mkdir /media/" + usbName;
        cmd = mkdirCMD.c_str();
        systemStatus = system(cmd);

        if (mountUSB() != COMMAND_SUCCEDED)
        {
            unmountUSB();
            mountUSB();

        }
        ui->noFlashGroup->hide();
        ui->runningMusicGroup->show();
        updateSongsList();


#endif
#if USING_DEV == USING_PC
        usbPath = "/media/Stuff/" + usbName + "/" ;

        ui->noFlashGroup->hide();
        ui->runningMusicGroup->show();
        updateSongsList();
#endif

    }
    /* Current usb is removed */
    else if (devicesConnected.empty() && flashStatus == FLASH_DETECTED)
    {
        flashStatus = NO_FLASH_DETECTED;

#if USING_DEV == USING_PI
        /* Unmounting usb device */

        if (unmountUSB() == COMMAND_SUCCEDED)
        {
            rmdirCMD = "rmdir /media/" + usbName;
            cmd = rmdirCMD.c_str();
            systemStatus = system (cmd);
            if (systemStatus == COMMAND_SUCCEDED)
            {
                /* In case of device unmounted successfully then update songs */
                ui->runningMusicGroup->hide();
                ui->noFlashGroup->show();
                updateSongsList();
            }
        }

#endif
#if USING_DEV == USING_PC
        ui->runningMusicGroup->hide();
        ui->noFlashGroup->show();
        updateSongsList();
#endif

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
    string songsListCount, songsList, songName, songPath;
    string listingSongsNumCMD, listingSongsCMD;

    currentSongsNumber = ui->runningSongsList->count();

    if (flashStatus == NO_FLASH_DETECTED)
    {
        if (currentSongsNumber != 0)
        {
            ui->runningSongsList->clear();
            playList->clear();
        }
    }
    else
    {
        listingSongsNumCMD = "ls " + usbPath + " | grep " + songsExt + " | wc -l";
        songsListCount = GetStdoutFromCommand(listingSongsCMD);

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

            /* Appned songs to music playlist and songs list in display */

            listingSongsCMD = "find " + usbPath + " -type f -name '*.mp3'";
            songsList = GetStdoutFromCommand(listingSongsCMD);

            start = 0;
            while (start < songsList.length())
            {
                end = songsList.find(songsExt,start);
                songPath = songsList.substr(start,(end-start));
                songPath = songPath + songsExt;

                songName = songPath.substr(songPath.rfind('/')+1, songPath.length());

                playList->addMedia(QUrl::fromLocalFile(QString::fromStdString(songPath)));

                ui->runningSongsList->addItem( QString::fromStdString(songName));
                ui->runningSongsList->setSpacing(7);

                start = end + songsExt.length();
            }

            playList->setPlaybackMode(QMediaPlaylist::Sequential);

            musicPlayer->setPlaylist(playList);
        }
    }
}


void mainScreen::onListMailItemClicked(QListWidgetItem* item)
{
    int chosenSong = ui->runningSongsList->row(item);
    playList->setCurrentIndex(chosenSong);

    if (playingSongFlag == 0)
    {
        playSong();
    }
}

void mainScreen::onSongChange()
{
    int songIndex;
    songIndex = playList->currentIndex();
    if (songIndex >= 0 && songIndex < playList->mediaCount())
    {
        ui->runningSongsList->item(songIndex)->setSelected(true);
    }
    ui->runningSongsList->scrollToItem(ui->runningSongsList->item(songIndex),QAbstractItemView::PositionAtBottom);

}

void mainScreen::playSong()
{
    playingSongFlag = 1;

    musicPlayer->play();
    ui->playButton->setIcon(QIcon(":/musicControl/media/pause_icon.png"));
}

void mainScreen::playToggle()
{
    playingSongFlag = 0;

    ui->playButton->setIcon(QIcon(":/musicControl/media/play_icon.png"));
}


/* Music control buttons */
void mainScreen::on_playButton_clicked()
{
    if (playingSongFlag == 0)
    {
        playSong();
    }
    else
    {
        musicPlayer->pause();
        playToggle();
    }
}

void mainScreen::on_stopButton_clicked()
{

    if (playingSongFlag == 1)
    {
        musicPlayer->stop();

        playToggle();
    }
}

void mainScreen::on_forwardButton_clicked()
{
    playList->next();


    if (playList->currentIndex() < 0 && (playList->playbackMode() == QMediaPlaylist::Sequential))
    {
        playToggle();
    }
}

void mainScreen::on_backwardButton_clicked()
{
    playList->previous();

    if (playList->currentIndex() < 0 && (playList->playbackMode() == QMediaPlaylist::Sequential))
    {
        playToggle();
    }
}

void mainScreen::on_repeatButton_clicked()
{
    static int toggleRepeat = 0;
    /*
     * 0 -> Repeat all mode
     * 1 -> Repeat once
     * 2 -> No repeat
 */
    if (toggleRepeat == 0)
    {

        toggleRepeat = 1;

        playList->setPlaybackMode(QMediaPlaylist::Loop);
        ui->repeatButton->setIcon(QIcon(":/musicControl/media/repeatPressed_icon.png"));

    }
    else if (toggleRepeat == 1)
    {

        toggleRepeat = 2;
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->repeatButton->setIcon(QIcon(":/musicControl/media/repeatOnce_icon.png"));

    }
    else
    {
        toggleRepeat =0;
        playList->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->repeatButton->setIcon(QIcon(":/musicControl/media/repeat_icon.png"));


    }

}

void mainScreen::on_shuffleButton_clicked()
{
    static int toggleShuffle = 0;

    if (toggleShuffle == 0)
    {
        toggleShuffle = 1;

        playList->setPlaybackMode(QMediaPlaylist::Random);
        ui->shuffleButton->setIcon(QIcon(":/musicControl/media/shufflePressed_icon.png"));
    }
    else
    {
        toggleShuffle = 0;

        playList->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->shuffleButton->setIcon(QIcon(":/musicControl/media/shuffle_icon.png"));
    }


}

void mainScreen::on_volumeDownButton_clicked()
{
    currentVolume -= volumeStep;
    if (currentVolume < MIN_VOLUME)
    {
        currentVolume = MIN_VOLUME;
    }

    musicPlayer->setVolume(currentVolume);
}

void mainScreen::on_volumeUpButton_clicked()
{

   currentVolume += volumeStep;
   if (currentVolume > MAX_VOLUME)
   {
       currentVolume = MAX_VOLUME;
   }

   musicPlayer->setVolume(currentVolume);
}



/*****************************************************************************************************************************/
/*************************************************    Phone Methods    *******************************************************/
/*****************************************************************************************************************************/


void mainScreen::on_phoneButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(PHONE_PAGE_INDEX);
}

void mainScreen::on_fmButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(FM_PAGE_INDEX);
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



