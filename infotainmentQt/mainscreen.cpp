#include "mainscreen.h"
#include "ui_mainscreen.h"
#include "systeminterface.h"

#include "availabletargets.h"
#include "targetconfiguration.h"


/* System Macros */
#define COMMAND_SUCCEDED    0

/* Stack widget pages for main screen */
#define HOME_PAGE_INDEX 0
#define VIDEO_PAGE_INDEX 1
#define MUSIC_PAGE_INDEX 2
#define BLUETOOTH_PAGE_INDEX 3
#define SETTINGS_PAGE_INDEX 4
#define VIDEO_SHOW_PAGE 5

/* Multimedia Macros */
#define NO_FLASH_DETECTED   0
#define FLASH_DETECTED      1
#define MAX_VOLUME          100
#define MIN_VOLUME          0


/* Bluetooth Macros */
#define BLUETOOTH_CONNECTED 1
#define BLUETOOTH_NOT_CONNECTED 2

/* General variables */
int currentVolume = 50;

/* Multimedia variables */
int volumeStep = 10;
int flashStatus = NO_FLASH_DETECTED;
string usbName, usbPath;

/* Music variables */
string songsExt = ".mp3";
int playingSongFlag = 0;

/* Video variables */
string videosExt = ".mp4";
int playingVideoFlag = 0;
long long int videoStep = 10000;

/* Bluetooth variables */
int bluetoothDeviceConnected = BLUETOOTH_NOT_CONNECTED;

/* System Functions */
int mountUSB (void);
int unmountUSB(void);
void initialCommands(void);
string extractUsb (string devices);



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


    /* Enable required initial daemons */
    initialCommands();

    /* Setting home page as default page */
    ui->pagesSwitch->setCurrentIndex(HOME_PAGE_INDEX);

    /* Connecting home buttons to goBackHomeFunction */
    connect(ui->homeButtonVd,SIGNAL(clicked()),SLOT(goBackHome()));
    connect(ui->homeButtonMs,SIGNAL(clicked()),SLOT(goBackHome()));
    connect(ui->homeButtonBl,SIGNAL(clicked()),SLOT(goBackHome()));
    connect(ui->homeButtonSt,SIGNAL(clicked()),SLOT(goBackHome()));

    /* Setting home buttons icons */

    ui->videoButton->setIcon(QIcon(":/mainIcons/media/video_icon.png"));
    ui->videoButton->setIconSize(QSize(100,100));

    ui->musicButton->setIcon(QIcon(":/mainIcons/media/music_icon.png"));
    ui->musicButton->setIconSize(QSize(100,100));

    ui->bluetoothButton->setIcon(QIcon(":/mainIcons/media/bluetooth_icon.png"));
    ui->bluetoothButton->setIconSize(QSize(100,100));

    ui->settingsButton->setIcon(QIcon(":/mainIcons/media/settings_icon.png"));
    ui->settingsButton->setIconSize(QSize(100,100));

    ui->homeButtonVd->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonVd->setIconSize(QSize(50,50));
    ui->homeButtonMs->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonMs->setIconSize(QSize(50,50));
    ui->homeButtonBl->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonBl->setIconSize(QSize(50,50));
    ui->homeButtonSt->setIcon(QIcon(":/mainIcons/media/home_icon.png"));
    ui->homeButtonSt->setIconSize(QSize(50,50));


    /* Setting default music pages */
    ui->runningMusicGroup->hide();
    ui->currentRunningMediaGroup->hide();


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
    connect(ui->runningSongsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onSongListItemClicked(QListWidgetItem*)));
    connect(playList,SIGNAL(currentIndexChanged(int)),SLOT(onSongChange()));

    /* Setting default video pages */
    ui->runningVideoGroup->hide();
    connect(ui->runningVideosList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onVideoListItemClicked(QListWidgetItem*)));

    /* Video display setup */
    videoPlayer->setVideoOutput(videoGraphicsItem);
    videoScene->addItem(videoGraphicsItem);
    ui->videoView->setScene(videoScene);
    videoGraphicsItem->setSize(QSize(900,380));

    ui->playVideoButton->setIcon(QIcon(":/musicControl/media/play_icon.png"));
    ui->playVideoButton->setIconSize(QSize(40,40));
    ui->forwardVideoButton->setIcon(QIcon(":/musicControl/media/forward_icon.png"));
    ui->forwardVideoButton->setIconSize(QSize(40,40));
    ui->backwardVideoButton->setIcon(QIcon(":/musicControl/media/backward_icon.png"));
    ui->backwardVideoButton->setIconSize(QSize(40,40));
    ui->stopVideoButton->setIcon(QIcon(":/musicControl/media/stop_icon.png"));
    ui->stopVideoButton->setIconSize(QSize(40,40));
    ui->volumeUpVideoButton->setIcon(QIcon(":/musicControl/media/volumeUp_icon.png"));
    ui->volumeUpVideoButton->setIconSize(QSize(30,30));
    ui->volumeDownVideoButton->setIcon(QIcon(":/musicControl/media/volumeDown_icon.png"));
    ui->volumeDownVideoButton->setIconSize(QSize(30,30));
    ui->goTovideosListButton->setIcon(QIcon(":/videoIcons/media/back_icon.png"));
    ui->goTovideosListButton->setIconSize(QSize(30,30));


    /* Settings buttons icons */
    ui->darkThemeButton->setIcon(QIcon(":/settingsIcons/media/darkThemeOff_icon.png"));
    ui->darkThemeButton->setIconSize(QSize(70,34));
    ui->doneTimeDateButton->setIcon(QIcon(":/settingsIcons/media/done_icon.png"));
    ui->doneTimeDateButton->setIconSize(QSize(50,50));
    ui->cancelTimeDateButton->setIcon(QIcon(":/settingsIcons/media/cancel_icon.png"));
    ui->cancelTimeDateButton->setIconSize(QSize(50,50));

    ui->setTimeDateGroup->hide();
    ui->settingsMainGroup->show();

    /* Bluetooth setup */
    ui->bluetoothIconLabel->hide();
    enableBluetooth();

    /* Check bluetooth connections */
    bluetoothDetectionTimer = new QTimer(this);
    connect(bluetoothDetectionTimer,SIGNAL(timeout()),this,SLOT(updatBluetoothDevices()));
    bluetoothDetectionTimer->start();

}



mainScreen::~mainScreen()
{
    delete ui;
}


/*****************************************************************************************************************************/
/************************************************    General Methods    ******************************************************/
/*****************************************************************************************************************************/


void initialCommands()
{
    const char *cmd;

    string enablePulseAudioCMD = "pulseaudio -D 2> /dev/null";
    cmd = enablePulseAudioCMD.c_str();
    system(cmd);

    string unblockBluetoothCMD = "rfkill unblock bluetooth";
    cmd = unblockBluetoothCMD.c_str();
    system(cmd);
}

int mountUSB ()
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
    string umountCMD, forceUnmountCMD;
    int systemStatus;

    umountCMD = "umount /media/" + usbName;
    forceUnmountCMD = "umount -l /media/" + usbName;

    cmd = umountCMD.c_str();
    systemStatus = system(cmd);
    if (systemStatus != COMMAND_SUCCEDED)
    {
        cmd = forceUnmountCMD.c_str();
        systemStatus = system(cmd);
    }
    return systemStatus;
}

string extractUsb(string devices)
{
    string dev,devType;
    string checkDevTypeCMD;
    int start, devNameLen;

    devNameLen = 4;
    start = 0;

    while (start<devices.length())
    {
        dev = devices.substr(start,devNameLen);

        checkDevTypeCMD = "udevadm info --query=all --name=" +dev + " | grep usb";
        devType = GetStdoutFromCommand(checkDevTypeCMD);
        if (devType.length()!=0)
        {
            return dev;
        }

        start = start + devNameLen;
    }
    return devType;
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

    ui->setTimeDateButton->setText(time + ", " + date);

}

/* Mutual home button in every page */
void mainScreen::goBackHome (void)
{
    ui->pagesSwitch->setCurrentIndex(HOME_PAGE_INDEX);
}

/*****************************************************************************************************************************/
/**********************************************    Multimedia Methods    *****************************************************/
/*****************************************************************************************************************************/


/* Flash status updating method */
void mainScreen::updateFlashStatus()
{
    int systemStatus;
    string listDevicesCMD, mkdirCMD, rmdirCMD;
    string devicesConnected;
    const char *cmd;

    /* Check if any flash usb is connected */

    listDevicesCMD = "ls /dev/ | grep sd[a-z][0-9]";

    devicesConnected = GetStdoutFromCommand(listDevicesCMD);

    devicesConnected = extractUsb(devicesConnected);

    /* New usb is connected */
    if (!devicesConnected.empty() && flashStatus == NO_FLASH_DETECTED)
    {
        flashStatus = FLASH_DETECTED;
        usbName = devicesConnected;


        usbPath = "/media/" + usbName + "/";

        /* Mounting usb device */

        mkdirCMD = "mkdir /media/" + usbName;
        cmd = mkdirCMD.c_str();
        systemStatus = system(cmd);

        if (mountUSB() != COMMAND_SUCCEDED)
        {
            unmountUSB();
            rmdirCMD = "rmdir /media/" + usbName;
            cmd = rmdirCMD.c_str();
            systemStatus = system (cmd);
            mountUSB();
        }

        ui->noFlashGroupMusic->hide();
        ui->noFlashGroupVideo->hide();
        ui->runningMusicGroup->show();
        ui->runningVideoGroup->show();
        updateSongsList();
        updateVideosList();


    }
    /* Current usb is removed */
    else if (devicesConnected.empty() && flashStatus == FLASH_DETECTED)
    {
        flashStatus = NO_FLASH_DETECTED;

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
                ui->noFlashGroupMusic->show();
                updateSongsList();
                updateVideosList();
            }
        }


    }
    /* No usb is connected */
    else if (devicesConnected.empty() && flashStatus == NO_FLASH_DETECTED)
    {
        ui->runningMusicGroup->hide();
        ui->runningVideoGroup->hide();
        ui->noFlashGroupMusic->show();
        ui->noFlashGroupVideo->show();
    }


    if (playingSongFlag == 1)
    {
        ui->currentRunningMediaGroup->show();
        ui->runningMediaLabel->setStyleSheet("image: url(:/mainIcons/media/music_icon.png)");
    }
    else if (playingVideoFlag == 1)
    {
        ui->currentRunningMediaGroup->show();
        ui->runningMediaLabel->setStyleSheet("image: url(:/mainIcons/media/video_icon.png)");
    }
    else
    {
        ui->currentRunningMediaGroup->hide();
    }

}


/*****************************************************************************************************************************/
/*************************************************    Music Methods    *******************************************************/
/*****************************************************************************************************************************/

void mainScreen::on_musicButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(MUSIC_PAGE_INDEX);
}

/* Updating songs list method */
void mainScreen::updateSongsList()
{
    int currentSongsNumber, start,end;
    string songsListCount, songsList, songPath,songName;
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
        songsListCount = GetStdoutFromCommand(listingSongsNumCMD);


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

            listingSongsCMD = "find " + usbPath + " -type f -name '*" + songsExt + "'";
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

void mainScreen::onSongListItemClicked(QListWidgetItem* item)
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
        ui->runningSongsList->scrollToItem(ui->runningSongsList->item(songIndex),QAbstractItemView::EnsureVisible);
        ui->currentRunningMediaLabel->setText(ui->runningSongsList->item(playList->currentIndex())->text());
    }


}

void mainScreen::playSong()
{
    playingSongFlag = 1;

    ui->currentRunningMediaLabel->setText(ui->runningSongsList->item(playList->currentIndex())->text());

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
/************************************************    Video Methods    *****************************************************/
/*****************************************************************************************************************************/

void mainScreen::on_videoButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(VIDEO_PAGE_INDEX);

}

void mainScreen::updateVideosList()
{
    int currentVideoNumber, start,end;
    string videosListCount, videosList, videoPath,videoName;
    string listingVideosNumCMD, listingVideosCMD;

    currentVideoNumber = ui->runningVideosList->count();

    if (flashStatus == NO_FLASH_DETECTED)
    {
        if (currentVideoNumber != 0)
        {
            ui->runningVideosList->clear();
            videoPlayList->clear();
        }
    }
    else
    {
        listingVideosNumCMD = "ls " + usbPath + " | grep " + videosExt + " | wc -l";
        videosListCount = GetStdoutFromCommand(listingVideosNumCMD);


        if (videosListCount == "0")
        {
            ui->runningVideosList->hide();
            ui->zeroVideosGroup->show();
        }
        else
        {
            ui->zeroVideosGroup->hide();
            ui->runningVideosList->show();

            /* Just to make sure that any old displayed sone is removed */
            ui->runningVideosList->clear();

            /* Appned songs to music playlist and songs list in display */

            listingVideosCMD = "find " + usbPath + " -type f -name '*" + videosExt + "'";
            videosList = GetStdoutFromCommand(listingVideosCMD);

            start = 0;
            while (start < videosList.length())
            {
                end = videosList.find(videosExt,start);
                videoPath = videosList.substr(start,(end-start));
                videoPath = videoPath + videosExt;

                videoName = videoPath.substr(videoPath.rfind('/')+1, videoPath.length());

                videoPlayList->addMedia(QUrl::fromLocalFile(QString::fromStdString(videoPath)));

                ui->runningVideosList->addItem( QString::fromStdString(videoName));
                ui->runningVideosList->setSpacing(7);

                start = end + videosExt.length();
            }

            videoPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

            videoPlayer->setPlaylist(videoPlayList);
        }
    }
}

void mainScreen::playVideo()
{

    /* Stop music first*/
    if (playingSongFlag == 1)
    {
        musicPlayer->stop();

        playToggle();
    }

    playingVideoFlag = 1;
    videoPlayer->play();
    ui->playVideoButton->setIcon(QIcon(":/musicControl/media/pause_icon.png"));

}

void mainScreen::stopVideo()
{
    playingVideoFlag = 0;
    videoPlayer->pause();
    ui->playVideoButton->setIcon(QIcon(":/musicControl/media/play_icon.png"));

}

void mainScreen::onVideoListItemClicked(QListWidgetItem* item)
{
    int chosenVideo = ui->runningVideosList->row(item);

    if (chosenVideo != videoPlayList->currentIndex())
    {
        videoPlayList->setCurrentIndex(chosenVideo);
    }

    ui->pagesSwitch->setCurrentIndex(VIDEO_SHOW_PAGE);

    ui->currentRunningMediaLabel->setText(ui->runningVideosList->item(chosenVideo)->text());

    playVideo();
}

void mainScreen::on_goTovideosListButton_clicked()
{
    /* Stop any running video first */

    stopVideo();

    ui->pagesSwitch->setCurrentIndex(VIDEO_PAGE_INDEX);
}

void mainScreen::on_playVideoButton_clicked()
{

    if (playingVideoFlag == 0)
    {
        playVideo();
    }
    else
    {
       stopVideo();
    }

}

void mainScreen::on_stopVideoButton_clicked()
{

    /* To keep the video preview instead of black screen that results of stop function */
    videoPlayList->setCurrentIndex(videoPlayList->currentIndex());

    stopVideo();
}

void mainScreen::on_backwardVideoButton_clicked()
{
    long long int pos = videoPlayer->position();

    pos = pos - videoStep;

    if (pos < 0)
    {
        pos = 0;
    }

    videoPlayer->setPosition(pos);

}

void mainScreen::on_forwardVideoButton_clicked()
{
    long long int pos = videoPlayer->position();

    pos = pos + videoStep;

    if (pos >= videoPlayer->duration())
    {
        videoPlayList->setCurrentIndex(videoPlayList->currentIndex());
        stopVideo();
    }
    else
    {
        videoPlayer->setPosition(pos);
    }

}

void mainScreen::on_volumeDownVideoButton_clicked()
{
    currentVolume -= volumeStep;
    if (currentVolume < MIN_VOLUME)
    {
        currentVolume = MIN_VOLUME;
    }

    videoPlayer->setVolume(currentVolume);
}

void mainScreen::on_volumeUpVideoButton_clicked()
{
    currentVolume += volumeStep;
    if (currentVolume > MAX_VOLUME)
    {
        currentVolume = MAX_VOLUME;
    }

    videoPlayer->setVolume(currentVolume);
}


/*****************************************************************************************************************************/
/************************************************    Bluetooth Methods    *****************************************************/
/*****************************************************************************************************************************/

void mainScreen::on_bluetoothButton_clicked()
{
    ui->pagesSwitch->setCurrentIndex(BLUETOOTH_PAGE_INDEX);

}

void mainScreen::updatBluetoothDevices()
{
    string listConnectedDevicesCMD, getDeviceNameCMD;
    string listConnectedDevices, deviceAddress,deviceName, zeroConnectedDevices;
    int start = 0, end;
    regex macRegex("[a-fA-F0-9:]{17}|[a-fA-F0-9]{12}");
    smatch macMatch;


    listConnectedDevicesCMD = "hcitool con";

    listConnectedDevices = GetStdoutFromCommand (listConnectedDevicesCMD);

    end = listConnectedDevices.find(":",start);

    if (end == (listConnectedDevices.length()-1))
    {
        /* No bluetooth devices are connected */
        bluetoothDeviceConnected = BLUETOOTH_NOT_CONNECTED;

        zeroConnectedDevices = "Connect Your Device!";

        ui->connectedBluetoothDeviceLabel->setText(QString::fromStdString(zeroConnectedDevices));
        ui->bluetoothIconLabel->hide();
    }
    else if (end != (listConnectedDevices.length()-1) && bluetoothDeviceConnected == BLUETOOTH_NOT_CONNECTED)
    {
        bluetoothDeviceConnected = BLUETOOTH_CONNECTED;
        regex_search(listConnectedDevices, macMatch, macRegex);

        for (auto x : macMatch)
            deviceAddress = x ;

        getDeviceNameCMD = "hcitool name " + deviceAddress;
        deviceName = GetStdoutFromCommand(getDeviceNameCMD);
        deviceName = "Connected Device: " + deviceName;
        ui->connectedBluetoothDeviceLabel->setText(QString::fromStdString(deviceName));
        ui->bluetoothIconLabel->show();
    }
    else
    {
        /* Do nothing */
    }

}

void mainScreen::enableBluetooth()
{
    string enableBluetoothCMD;
    const char *cmd;
    int systemStatus;


#if TARGET_DEV == DEV_RPI
    enableBluetoothCMD = "bluetoothctl power on && bluetoothctl discoverable on && bluetoothctl pairable on && bluetoothctl agent NoInputNoOutput";
#endif
#if TARGET_DEV == DEV_PC
    enableBluetoothCMD = "service bluetooth start && bt-agent --capability=NoInputNoOutput > /dev/null &";
    cmd = enableBluetoothCMD.c_str();
    systemStatus = system (cmd);
#endif

}


/*****************************************************************************************************************************/
/************************************************    Settings Methods    *****************************************************/
/*****************************************************************************************************************************/

void mainScreen::on_settingsButton_clicked()
{
    ui->setTimeDateGroup->hide();
    ui->settingsMainGroup->show();

    ui->pagesSwitch->setCurrentIndex(SETTINGS_PAGE_INDEX);
}

void mainScreen::on_darkThemeButton_clicked()
{
    static int darkThemeFlag = 0;
    if (darkThemeFlag == 0)
    {
        darkThemeFlag = 1;

        ui->darkThemeButton->setIcon(QIcon(":/settingsIcons/media/darkThemeOn_icon.png"));
        mainScreen::setStyleSheet("background:url(:/mainBG/media/darkBackground.png)");
    }
    else
    {
        darkThemeFlag = 0;
        ui->darkThemeButton->setIcon(QIcon(":/settingsIcons/media/darkThemeOff_icon.png"));
        mainScreen::setStyleSheet("background:url(:/mainBG/media/colorfulBackground.jpeg)");

    }
}

void mainScreen::on_setTimeDateButton_clicked()
{
    ui->settingsMainGroup->hide();
    ui->setTimeDateGroup->show();
}

void mainScreen::on_doneTimeDateButton_clicked()
{
    string updatedTime, updatedDate;
    string updateTimeDateCMD;
    const char *cmd;


    updatedTime = ui->updatedTimeValue->time().toString().toStdString();
    updatedDate = ui->updatedDateValue->date().toString("d MMM yyyy").toStdString();

#if TARGET_DEV == DEV_PC
    updateTimeDateCMD = "date -s '" + updatedDate + " " + updatedTime + "'";
    cmd = updateTimeDateCMD.c_str();
    system(cmd);
#endif

    ui->setTimeDateGroup->hide();
    ui->settingsMainGroup->show();



}

void mainScreen::on_cancelTimeDateButton_clicked()
{
    ui->setTimeDateGroup->hide();
    ui->settingsMainGroup->show();
}
