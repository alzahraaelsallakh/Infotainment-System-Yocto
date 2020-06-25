# Infotainment-System-Yocto

This repo if for ITI-Intake 40 graduation project

## What is a car infotainment system?
An in-vehicle infotainment system is a combination of systems that deliver entertainment and information to the driver and passengers. They do it using audio and video interfaces, touch screen displays, button panels, voice commands, and many other features. [check more!](https://concisesoftware.com/car-infotainment-system-guide/)

<p align="center">
  <img width="700" height="346" src="../media/info-sys-demo.jpg">
</p>
 
 
## Table of Contents
1. [ Building the image using Yocto ](#gettingStarted)
	1. [ Setting up environment ](#settingEnvYocto)  
	2. [ Configuring network settings ](#networkSettings)
	3. [ Adding VNC server ](#addingVNC)  
	4. [ Adding Qt ](#addingQt)  
	5. [ Installing PySide for Qt ](#enablePySide)
	6. [ Enabling sound ](#enableSound)
	7. [ Baking and flashing the image ](#baking) 
	8. [ Known issues ](#knownIssues)
2. [ Creating UI  ](#creatingUI)  
	1. [ Setting up environment ](#settingEnv)  
	2. [ Starting with Qt Creator ](#qtCreator)  
	3. [ Developing main screen ](#qtMainScreen)
3. [ MP3 player](#mp3Player)  

## Hardware  
**Host machine:** Ubuntu 18.04.4 LTS   
**Target machine:** Raspberry Pi 3B+  
 
---
<a name="gettingStarted"></a>
# Building the image using Yocto

<a name="settingEnvYocto"></a>
## Setting up environment

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

<a name="networkSettings"></a>
## Configuring network settings (Wifi)  

1. Create new custom layer ``` $ bitbake-layers create-layer meta-customInfotainment ``` 
2. Add the layer to BBLAYERS variable as previuos  
3. Create new folder for network configuration recipes, **recipes-network** will hold overlaying network files (.bbappend files)  
```
$ cd meta-customInfotainment/recipes-network
```  
The tree should be as following  
```
recipes-network
    ├── base-files
    │   ├── base-files
    │   │   └── profile
    │   └── base-files_%.bbappend
    ├── init-ifupdown
    │   ├── init-ifupdown
    │   │   └── interfaces
    │   └── init-ifupdown_%.bbappend
    └── wpa-supplicant
        ├── wpa-supplicant
        │   ├── 0001-AP-Silently-ignore-management-frame-from-unexpected-.patch
        │   ├── 0001-replace-systemd-install-Alias-with-WantedBy.patch
        │   ├── 99_wpa_supplicant
        │   ├── defconfig
        │   ├── wpa_supplicant.conf
        │   ├── wpa_supplicant.conf-sane
        │   └── wpa-supplicant.sh
        └── wpa-supplicant_%.bbappend

```
4. To add input wifi access point: /etc/wpa_supplicant.conf file is required, which is produced by wpa-supplicant recipe  
``` 
$ echo 'FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"' >  wpa-supplicant_%.bbappend  
```
  Copy the original wpa_supplicant.conf-sane file then add your modifications. It should be as following  
```
ctrl_interface=/var/run/wpa_supplicant
ctrl_interface_group=0
update_config=1

network={
	ssid="NETWORK_NAME"
	psk="NETWORK_PASSWORD"
}

network={
        key_mgmt=NONE
}
```
5. To enable wifi and set static ip address: /etc/network/interfaces file is required, which is produced by init-ifupdown recipe  
``` 
$ echo 'FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"' >  init-ifupdown_%.bbappend  
```
  Copy the original interfaces file then add your modifications. It should be as following  
```
# Wireless interfaces
auto wlan0

allow-hotplug wlan0
iface wlan0 inet static
	address 192.168.1.2  
	netmask 255.255.255.0  
	gateway 192.168.1.1  
	wpa-conf /etc/wpa_supplicant.conf  
iface default inet dhcp
```
6. To enable x11vnc server at bootin time: /etc/profile file is required, which is produced by base-files recipe  
``` 
$ echo 'FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"' >  init-ifupdown_%.bbappend  
```
  Copy the original profile file then add this line to the end of the file   
```
x11vnc &
```  
**Note:** To know the recipe producing the path, after sourcing run ``` $ oe-pkgdata-util find-path /etc/profile ```  The output will look like ```base-files: /etc/profile ```

<a name="addingVNC"></a>
## Adding VNC server

Edit rpi-build/local.conf and add x11vnc to  IMAGE_INSTALL_append variable  
```
IMAGE_INSTALL_append = " \
	x11vnc"
```  
<a name="addingQt"></a>
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
3.  Edit rpi-build/local.conf and add 
``` 
IMAGE_INSTALL_append = " make cmake"
IMAGE_INSTALL_append = " qtbase qtdeclarative qtimageformats qtmultimedia qtquickcontrols2 qtquickcontrols "
```  
4. Install generic SDK tool chain    
```
$ bitbake meta-toolchain
$ cd tmp/deploy/sdk
$ ./poky-glibc-x86_64-meta-toolchain-aarch64-raspberrypi3-64-toolchain-3.0.2.sh poky-glibc-x86_64-meta-toolchain-aarch64-raspberrypi3-64-toolchain-3.0.2.sh
$ source /opt/poky/3.0.2/environment-setup-aarch64-poky-linux
```
5. Install Qt5 tool chain for cross compilation  
```
$ bitbake meta-toolchain-qt5  
$ cd tmp/deploy/sdk
$ ./poky-glibc-x86_64-meta-toolchain-qt5-aarch64-raspberrypi3-64-toolchain-3.0.2.sh 
$ source yes/environment-setup-aarch64-poky-linux
```  

<a name="enablePySide"></a>
## Installing PySide for Qt  

   Python2.7 is required, Edit rpi-build/local.conf and add ``` CORE_IMAGE_EXTRA_INSTALL = "python-core python-pip" ```  

<a name="enableSound"></a>
## Enabling sound 

   Edit rpi-build/local.conf and add the following to enable GStreamer and mgp123
```
IMAGE_INSTALL_append = " gstreamer1.0-plugins-good gstreamer1.0-plugins-base gstreamer1.0-plugins-ugly"
LICENSE_FLAGS_WHITELIST_append = " commercial commercial_mpg123 commercial_gstreamer1.0-plugins-ugly "

PACKAGECONFIG_append_pn-qtmultimedia = " gstreamer alsa"  
```  

<a name="baking"></a>
## Baking and flashing the image 

1. Build the image using the build engine **BitBake**  
It may take many hours to finish the build process
```
$ bitbake core-image-sato
```  
**core-image-sato** is selected as it supports X11 and a GUI server is required   

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


<a name="knownIssues"></a>
## Known issues

**Issue 1:** The halt function in core-image-sato has a bug, where any restart/shutdown/reboot operation interrupts the image every time  
**Workaround:** Cut the power off temporarly each time 

---
<a name="creatingUI"></a>
# Creating UI  

<a name="settingEnv"></a>
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

<a name="qtCreator"></a>
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

<a name="qtMainScreen"></a>
## Developing main screen  

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


---
<a name="mp3Player"></a>
# MP3 player  

1. To run any mp3 file using GStreamer run the following command  
```
$ gst-play-1.0 song.mp3
```
2. To run any mp3 file using mpg123 run the following command   
```
$ mpg123 song.mp3
```
