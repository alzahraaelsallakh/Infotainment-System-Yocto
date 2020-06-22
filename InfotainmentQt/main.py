# This Python file uses the following encoding: utf-8

#import python libraries
import sys

#importing PySide libraries
from PySide.QtCore import *
from PySide.QtGui import *

#importing Qt GUI files
import mainScreenUi

#importing Qt resources files
import background_rc
import icons_rc


# Stack Widgets
HOME_PAGE_INDEX = 0
PHONE_PAGE_INDEX = 1
FM_PAGE_INDEX = 2
MUSIC_PAGE_INDEX = 3
GPS_PAGE_INDEX = 4
BLUETOOTH_PAGE_INDEX = 5
SETTINGS_PAGE_INDEX = 6


class MainScreen (QDialog, mainScreenUi.Ui_mainScreen):
    def __init__(self, parent=None):
        super(MainScreen, self).__init__(parent)
        self.setupUi(self)

        #Setting timer to update time and date continuosly
        timer = QTimer(self)
        timer.timeout.connect(self.updateTime)
        timer.start()

        ############################
        #Editing home page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(HOME_PAGE_INDEX)

        #Replacing buttons with icons

        callsIcon = QIcon()
        callsIcon.addPixmap(QPixmap(':/mainIcons/media/calls_icon.png'))
        self.phoneButton.setIcon(callsIcon)
        self.phoneButton.setIconSize(QSize(100, 100))

        fmIcon = QIcon()
        fmIcon.addPixmap(QPixmap(':/mainIcons/media/fm_icon.png'))
        self.fmButton.setIcon(fmIcon)
        self.fmButton.setIconSize(QSize(100, 100))

        musicIcon = QIcon()
        musicIcon.addPixmap(QPixmap(':/mainIcons/media/music_icon.png'))
        self.musicButton.setIcon(musicIcon)
        self.musicButton.setIconSize(QSize(100, 100))

        gpsIcon = QIcon()
        gpsIcon.addPixmap(QPixmap(':/mainIcons/media/gps_icon.png'))
        self.gpsButton.setIcon(gpsIcon)
        self.gpsButton.setIconSize(QSize(100, 100))


        bluetoothIcon = QIcon()
        bluetoothIcon.addPixmap(QPixmap(':/mainIcons/media/bluetooth_icon.png'))
        self.bluetoothButton.setIcon(bluetoothIcon)
        self.bluetoothButton.setIconSize(QSize(100, 100))

        settingsIcon = QIcon()
        settingsIcon.addPixmap(QPixmap(':/mainIcons/media/settings_icon.png'))
        self.settingsButton.setIcon(settingsIcon)
        self.settingsButton.setIconSize(QSize(100, 100))


        #Connecting icons to their pages
        self.connect(self.phoneButton, SIGNAL("clicked()"),self.phonePageDisp)
        self.connect(self.fmButton, SIGNAL("clicked()"),self.fmPageDisp)
        self.connect(self.musicButton, SIGNAL("clicked()"),self.musicPageDisp)
        self.connect(self.gpsButton, SIGNAL("clicked()"),self.gpsPageDisp)
        self.connect(self.bluetoothButton, SIGNAL("clicked()"),self.bluetoothPageDisp)
        self.connect(self.settingsButton, SIGNAL("clicked()"),self.settingsPageDisp)


        ############################
        #Editing phone page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(PHONE_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonPh,SIGNAL("clicked()"),self.homePageDisp)

        #Replacing buttons with icons

        #Applying mutual settings
        self.setMutualSettings()


        ############################
        #Editing FM page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(FM_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonFm,SIGNAL("clicked()"),self.homePageDisp)

        #Applying mutual settings
        self.setMutualSettings()


        ############################
        #Editing music page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(MUSIC_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonMs,SIGNAL("clicked()"),self.homePageDisp)

        #Applying mutual settings
        self.setMutualSettings()

        ############################
        #Editing GPS page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(GPS_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonGp,SIGNAL("clicked()"),self.homePageDisp)

        #Applying mutual settings
        self.setMutualSettings()


        ############################
        #Editing Bluetooth page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(BLUETOOTH_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonBl,SIGNAL("clicked()"),self.homePageDisp)

        #Applying mutual settings
        self.setMutualSettings()


        ############################
        #Editing settings page settings#
        ############################

        self.pagesSwitch.setCurrentIndex(SETTINGS_PAGE_INDEX)

        #Connecting home icon to home pages
        self.connect(self.homeButtonSt,SIGNAL("clicked()"),self.homePageDisp)

        #Applying mutual settings
        self.setMutualSettings()

        ############################
        #Setting home page as default page#
        ############################

        self.pagesSwitch.setCurrentIndex(HOME_PAGE_INDEX)


    #Update time and date method
    def updateTime(self):
        self.dateText.setText(QDate.currentDate().toString(Qt.DefaultLocaleLongDate))
        self.dateText.setAlignment(Qt.AlignRight)
        self.timeText.setText(QTime.currentTime().toString('hh:mm AP'))
        self.timeText.setAlignment(Qt.AlignRight)

    #Displaying phone page method
    def phonePageDisp(self):
        self.pagesSwitch.setCurrentIndex(PHONE_PAGE_INDEX)

    #Displaying home page method
    def homePageDisp(self):
        self.pagesSwitch.setCurrentIndex(HOME_PAGE_INDEX)

    #Displaying FM page method
    def fmPageDisp(self):
        self.pagesSwitch.setCurrentIndex(FM_PAGE_INDEX)

    #Displaying music page method
    def musicPageDisp(self):
        self.pagesSwitch.setCurrentIndex(MUSIC_PAGE_INDEX)

    #Displaying gps page method
    def gpsPageDisp(self):
        self.pagesSwitch.setCurrentIndex(GPS_PAGE_INDEX)

    #Displaying bluetooth page method
    def bluetoothPageDisp(self):
        self.pagesSwitch.setCurrentIndex(BLUETOOTH_PAGE_INDEX)

    #Displaying settings page method
    def settingsPageDisp(self):
        self.pagesSwitch.setCurrentIndex(SETTINGS_PAGE_INDEX)

    def setMutualSettings(self):

        homeIcon = QIcon()
        homeIcon.addPixmap(QPixmap(':/mainIcons/media/home_icon.png'))

        if ( self.pagesSwitch.currentIndex()) == FM_PAGE_INDEX :
            self.homeButtonFm.setIcon(homeIcon)
            self.homeButtonFm.setIconSize(QSize(70, 70))
        elif ( self.pagesSwitch.currentIndex()) == PHONE_PAGE_INDEX :
            self.homeButtonPh.setIcon(homeIcon)
            self.homeButtonPh.setIconSize(QSize(70, 70))
        elif ( self.pagesSwitch.currentIndex()) == MUSIC_PAGE_INDEX :
            self.homeButtonMs.setIcon(homeIcon)
            self.homeButtonMs.setIconSize(QSize(70, 70))
        elif ( self.pagesSwitch.currentIndex()) == GPS_PAGE_INDEX :
            self.homeButtonGp.setIcon(homeIcon)
            self.homeButtonGp.setIconSize(QSize(70, 70))
        elif ( self.pagesSwitch.currentIndex()) == BLUETOOTH_PAGE_INDEX :
            self.homeButtonBl.setIcon(homeIcon)
            self.homeButtonBl.setIconSize(QSize(70, 70))
        elif ( self.pagesSwitch.currentIndex()) == SETTINGS_PAGE_INDEX :
            self.homeButtonSt.setIcon(homeIcon)
            self.homeButtonSt.setIconSize(QSize(70, 70))

if __name__ == "__main__":

    try:
        print ("System is running\n")
        app = QApplication(sys.argv)
        form  = MainScreen()
        form.show()
        app.exec_()

    except:
        print ("System is not working properly\n")
