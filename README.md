# Infotainment-System-Yocto

This repo if for ITI-Intake 40 graduation project

## What is a car infotainment system?
An in-vehicle infotainment system is a combination of systems that deliver entertainment and information to the driver and passengers. They do it using audio and video interfaces, touch screen displays, button panels, voice commands, and many other features. [check more!](https://concisesoftware.com/car-infotainment-system-guide/)

<p align="center">
  <img width="700" height="346" src="../media/info-sys-demo.jpg">
</p>

---
# Getting Started

## Building the Image using Yocto

1. Download the Poky build system (zeus branch)  
```
$ git clone -b zeus git://git.yoctoproject.org/poky
``` 
2. Download RPI BSP (zeus branch) 
```
$ git clone -b zeus https://github.com/agherzan/meta-raspberrypi.git 
``` 
3. Download openembedded (zeus branch)
```
$ git clone -b zeus https://github.com/openembedded/meta-openembedded.git
```
Note: for my steps, both poky, meta-raspberrypi and meta-openembedded repos are in the same path   
  
4. Source “oe-init-build-env” script 
```
$ source poky/oe-init-build-env rpi-build
```
5. Edit rpi-build/bblayers.conf and add layers to BBLAYERS variable  
```
BBLAYERS ?= " \
/ABSOLUTE/PATH/poky/meta \
/ABSOLUTE/PATH/poky/meta-poky \
/ABSOLUTE/PATH/poky/meta-yocto-bsp \
/ABSOLUTE/PATH/meta-raspberrypi \
/ABSOLUTE/PATH/meta-openembedded/meta-oe \
/ABSOLUTE/PATH/meta-openembedded/meta-python \
/ABSOLUTE/PATH/meta-openembedded/meta-networking \
/ABSOLUTE/PATH/meta-openembedded/meta-multimedia \
"
```  
6. Edit rpi-build/local.conf by changing ```MACHINE ??= "qemux86-64"``` to be ```MACHINE ?= "raspberrypi3-64"```  

7. Edit rpi-build/local.conf and add the following line  
```
LICENSE_FLAGS_WHITELIST_append = " commercial_faad2 commercial_gstreamer1.0-plugins-ugly "
```  

## Adding VNC server

Edit rpi-build/local.conf and add x11vnc to  IMAGE_INSTALL_append variable  
```
IMAGE_INSTALL_append = " \
	x11vnc"
```  

## Adding Qt 

1. Download the qt5 layer (zeus branch)  
```
$ git clone -b zeus https://github.com/meta-qt5/meta-qt5
``` 
2. Edit rpi-build/bblayers.conf and add the layer to BBLAYERS variable  
```
BBLAYERS ?= " \
....
/ABSOLUTE/PATH/meta-qt5 \
"
```  
3. Install generic SDK tool chain    
```
$ bitbake meta-toolchain
$ cd tmp/deploy/sdk
$ ./poky-glibc-x86_64-meta-toolchain-aarch64-raspberrypi3-64-toolchain-3.0.2.sh poky-glibc-x86_64-meta-toolchain-aarch64-raspberrypi3-64-toolchain-3.0.2.sh
$ source /opt/poky/3.0.2/environment-setup-aarch64-poky-linux
```
4. Install Qt5 tool chain for cross compilation  
```
$ bitbake meta-toolchain-qt5  
$ cd tmp/deploy/sdk
$ ./poky-glibc-x86_64-meta-toolchain-qt5-aarch64-raspberrypi3-64-toolchain-3.0.2.sh 
$ source yes/environment-setup-aarch64-poky-linux
```

## Baking and flashing the image 

1. Build the image using the build engine **BitBake**  
It may take many hours to finish the build process
```
$ bitbake core-image-sato
```  
**core-image-sato** is selected as it supports X11 and a GUI server is required  
Note: The halt function in sato image seems as it has bug, rebooting/restarting or shutting down interrupts the image every time. Temporary solution is to cut the power off (still under work)  

2. If the build process was successful, the raspberry pi image will be under ```rpi-build/tmp/deploy/images/raspberrypi3-64/core-image-sato-raspberrypi3-64.rpi-sdimg```  

3. Flash the image on the SD card and make sure that it's formatted as free space  
my SD card is /dev/mmcblk0  
```
$ sudo dd if=tmp/deploy/images/raspberrypi3-64/core-image-sato-raspberrypi3-64.rpi-sdimg of=/dev/mmcblk0 status=progress conv=fsync bs=4M
```
4. After the image is ready, connect RPI with HDMI having the next interface  
<p align="center">
  <img  src="../media/desktop.png">
</p>


## Configuring network settings (Wifi)  

1. Edit /etc/wpa_supplicant.conf file to input WiFi access point related information  
```
network={
	ssid="NETWORK_NAME"
	psk="NETWORK_PASSWORD"
}
```  
2. Edit /etc/network/interfaces to set the RPI static IP address and wifi gateway address   
```
auto wlan0

allow-hotplug wlan0
iface wlan0 inet static
	address 192.168.1.2  
	netmask 255.255.255.0  
	gateway 192.168.1.1  
	wpa-conf /etc/wpa_supplicant.conf  
iface default inet dhcp
```  
3. Edit /etc/profile to start x11vnc server at booting time  
```
x11vnc &
```

---
# Creating UI  

## Setting up environment

1. Install PySide, following [PySide](https://pypi.org/project/PySide/) steps maybe helpful. Python-pip and qt5-default are required, you may need to install them first  
```
$ sudo apt install python-pip
$ sudo apt-get install qt5-default
$ sudo pip install wheel
$ pip install PySide
```
You may face problems due to your OS version, your pip wheel... etc.  

2. Install PySide tools for Qt integration  
```
$ sudo apt install pyside-tools
```  
3. Install Qt5 Creator/Designer from [Get Qt](https://www.qt.io/download)  

## Starting with Qt Creator
1. Create new project with Qt Creator -> **Application (Qt for Python -> Empty)**   
This will create 2 files (main.pyproject, main.py)
<p align="center">
  <img  width="700" height="500" src="../media/Qt2.png">
</p>

2. Right click on the project to add new **Qt Designer Form**, to design from scratch select the **Widget** template
<p align="center">
  <img  width="700" height="500" src="../media/Qt5.png">
</p>    

3. To add images and any resources, right click on the project to add new **Qt Resource File** then add prefix and files you need  
<p align="center">
  <img   width="700" height="500" src="../media/Qt7.png">
</p>
<p align="center">
  <img  width="700" height="500" src="../media/Qt8.png">
</p>

## Qt for Python  

1. Convert your UI file to python file 
```
$ pyside-uic mainScreen.ui -o mainScreenUi.py
```
2. Convert your resource file to python file  
```
$ pyside-rcc icons.qrc -o icons_rc.py
```  
3. Add the python files to your project then import them in your main.py file  
```
#importing Qt GUI files
import mainScreenUi

#importing Qt resources files
import background_rc
import icons_rc
```
4. Create new classes/execute the Qt appliaction in your main.py  
5. Build and Run, This is the main screen demo 
<p align="center">
  <img src="../media/mainScreenGui.gif">
</p>
