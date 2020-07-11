# Infotainment-System-Yocto

This repo is for ITI-Intake 40 graduation project

## What is a car infotainment system?
An in-vehicle infotainment system is a combination of systems that deliver entertainment and information to the driver and passengers. They use audio and video interfaces, touch screen displays, button panels, voice commands, and many other features. [check more!](https://concisesoftware.com/car-infotainment-system-guide/)

<p align="center">
  <img width="700" height="346" src="../media/info-sys-demo.jpg">
</p>
 
 
## Table of Contents
1. [ Building the image using Yocto ](#gettingStarted)
	1. [ Setting up environment ](#settingEnvYocto)  
	2. [ Configuring network settings ](#networkSettings)
	3. [ Adding VNC server ](#addingVNC)  
	4. [ Adding Qt ](#addingQt)   
	5. [ Adding bluetooth ](#addingBluetooth)  
	6. [ Adding sound ](#enableSound)
	7. [ Video setup ](#enableVideo)
	8. [ Adding debugging tools ](#enableDebugging) 
	9. [ Baking and flashing the image ](#baking) 
	10. [ Known issues ](#knownIssues)
2. [ Creating UI  ](#creatingUI)  
	1. [ Setting up environment ](#settingEnv)  
	2. [ Configuring the cross compiling and remote deployment settings on Qt creator ](#qtCreatorDeploy)
	3. [ Creating Qt project with C++ ](#qtCreator)  
	4. [ Running multimedia Mp3 and Mp4 ](#multimedia)  
3. [ Conclusion ](#conclusion)
4. [ References ](#references)


## General setup  
**Host machine:** Ubuntu 18.04.4 LTS   
**Target machine:** Raspberry Pi 3B+  
 
---
<a name="gettingStarted"></a>
# Building the image using Yocto

<a name="settingEnvYocto"></a>
## Setting up the environment

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
LICENSE_FLAGS_WHITELIST_append = " commercial_faad2"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_append = " systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
```   
8. For developing you might need rootfs extra space, to add additional space as 5G edit rpi-build/local.conf and add the following line 
```
IMAGE_ROOTFS_EXTRA_SPACE = "5242880"
```

<a name="addingVNC"></a>
## Adding VNC server

Connect to your target through VNC server, then edit rpi-build/local.conf and add x11vnc to  IMAGE_INSTALL_append variable  
```
IMAGE_INSTALL_append = " x11vnc"
```  
There is an issue with x11vnc setup mentioned in the [Known issues](#knownIssues) section in issue no.1   

<a name="networkSettings"></a>
## Configuring network settings (Wifi)  

1. Create new custom layer ``` $ bitbake-layers create-layer meta-customInfotainment ``` 
2. Add the layer to BBLAYERS variable as previous  
3. Create a new folder for network configuration recipes, **recipes-network** will hold overlaying network files (.bbappend files)  
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
5. To enable WIFI and set static IP address: /etc/network/interfaces file is required, which is produced by init-ifupdown recipe  
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
6. In the case of X11VNC installed, you need to enable server at booting time: /etc/profile file is required, produced by base-files recipe. For an unknown reason, your wifi may be blocked by rfkill, so you need to unblock and enable it again  
``` 
$ echo 'FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"' >  init-ifupdown_%.bbappend  
```
  Copy the original profile file then add these to the end of the file   
```
rfkill unblock wifi

ifconfig wlan0 up
x11vnc &
```  
**Note:** To know the recipe producing the path, after sourcing run ``` $ oe-pkgdata-util find-path /etc/profile ```  The output will look like ```base-files: /etc/profile ```  
There is an issue in the netwrok setup mentioned in the [Known issues](#knownIssues) section in issue no.2  


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
3.  To support Qt5 on image, edit rpi-build/local.conf and add
``` 
IMAGE_INSTALL_append = " make cmake"
IMAGE_INSTALL_append = " qtbase-tools qtbase qtdeclarative qtimageformats qtmultimedia qtquickcontrols2 qtquickcontrols qtbase-plugins cinematicexperience liberation-fonts"
PACKAGECONFIG_FONTS_append_pn-qtbase = " fontconfig"
```  
4. To enable remote deployment on RPI using Qt platform, you need to add extra network configuration to rpi-build/local.conf  
```
IMAGE_INSTALL_append = " openssh-sftp-server rsync"
```  

<a name="addingBluetooth"></a>
## Adding Bluetooth  

   Edit rpi-build/local.conf to add required firmwares and bluez
```
MACHINE_FEATURES += " bluetooth"
CORE_IMAGE_EXTRA_INSTALL = " rsync "
DISTRO_FEATURES_append = " pi-bluetooth bluez5 bluetooth linux-firmware-bcm43430 linux-firmware-brcmfmac43430"
IMAGE_INSTALL_append = " pi-bluetooth bluez5 bluez5-testtools linux-firmware-bcm43430 i2c-tools hostapd dhcp-server udev-rules-rpi bridge-utils iptables linux-firmware-ralink linux-firmware-rtl8192ce linux-firmware-rtl8192cu linux-firmware-rtl8192su linux-firmware-rpidistro-bcm43430"
ENABLE_UART = "1"
```
There is an issue in the network setup mentioned in the [Known issues](#knownIssues) section in issue no.3   

<a name="enableSound"></a>
## Adding sound 

1. Edit rpi-build/local.conf and add the following to enable GStreamer or mgp123. Fot Qt Media modules GStreamer is required  
```
IMAGE_INSTALL_append = " gstreamer1.0-plugins-good gstreamer1.0-plugins-base gstreamer1.0-plugins-ugly"
LICENSE_FLAGS_WHITELIST_append = " commercial  commercial_gstreamer1.0-plugins-ugly commercial_gstreamer1.0-plugins-ugly"
PACKAGECONFIG_append_pn-qtmultimedia = " gstreamer alsa" 
```    
2. Pulseaudio is required to stream audio over Bluetooth  
```
DISTRO_FEATURES_append = " pulseaudio"
IMAGE_INSTALL_append = " pulseaudio pulseaudio-module-dbus-protocol pulseaudio-server pulseaudio-module-bluetooth-discover pulseaudio-module-bluetooth-policy pulseaudio-module-bluez5-device pulseaudio-module-bluez5-discover alsa-utils alsa-plugins"
```

<a name="enableVideo"></a>
## Video setup  
  
  For displaying video in a proper way you may need to set HDMI_MODE and HDMI_GROUP  in rpi-build/local.conf

<a name="enableDebugging"></a>
## Adding debugging tools  
  
  To enable debugging you will need to add strace in local.conf ``` IMAGE_INSTALL_append = " strace" ```  

<a name="baking"></a>
## Baking and flashing the image 

1. Build the image using the build engine **BitBake**  
It may take many hours to finish the build process
```
$ bitbake core-image-sato
```  
**core-image-sato** is selected as it supports X11 and a GUI server is required. But it has a bug mentioned in the [Known issues](#knownIssues) section in issues no.4 and no.5       

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

**Issue 1:** Running Qt5 will disable X11 then it will disable x11vnc   
**Workaround:** Enable x11vnc and run it on your host machine before you deploy with Qt   

**Issue 2:** Although network setup, the RPI doesn't connect automatically to wifi after deploying image  
**Workaround:**  I need to connect my RPI to HDMI and connect to my wifi manually only for the first time. HDMI is the best choice so far    

**Issue 3:** Bluetooth configurations enable Bluetooth connections and pairing with any device, but it seems that the bluetooth driver has a bug that fails in attaching serial to bluez stack and leads to block the audio streaming via Bluetooth   

**Issue 4:** The halt function in core-image-sato has a bug, where any restart/shutdown/reboot operation interrupts the image every time  
**Workaround:** Cut the power off temporarily each time 

**Issue 5:** Date and time cannot be set by default in core-image-sato  

---
<a name="creatingUI"></a>
# Creating UI  

<a name="settingEnv"></a>
## Setting up environment

1. Install Qt5 Creator command line launcher, my Qt version is 5.9.5  
```
$ sudo apt-get install qtcreator  
```
2. Install Qt5 toolchain for cross compilation. The installation path may differ, just check your terminal output  
```
$ bitbake meta-toolchain-qt5  
$ cd tmp/deploy/sdk
$ ./poky-glibc-x86_64-meta-toolchain-qt5-aarch64-raspberrypi3-64-toolchain-3.0.2.sh 
```   

<a name="qtCreatorDeploy"></a>
## Configuring the cross compiling and remote deployment settings on Qt creator

1. First, you need to source the SDK toolchain. The source path may differ depending on the output of your SDK installation  
```
$ source rpi-build/tmp/deploy/sdk/yes/environment-setup-aarch64-poky-linux  
```  
2. From the same terminal launch qtcreator 
```
$ qtcreator 
```  
3. After Qt creator launches, you need to configure the device from Tools -> Devices  
	1. Add new Generic Linux Device providing the name you want, the hostname/IP address of your device and the username  
	2. Make sure that the device is connected on the same ethernet network to your device and that you did step 4 in [Adding Qt](#addingQt)  
	3. Do the device Test, if anything goes wrong you need the review your network configurations  
	<p align="center">
  	<img  src="../media/qtDevice.png">
	</p>
4. Then configure your build and run options from Tools -> Build & Run
	1. Qt Versions -> add the path of the SDK qmake  `rpi-build/tmp/deploy/sdk/yes/sysroots/x86_64-pokysdk-linux/usr/bin/qmake` with any name  
		<p align="center">
  		<img  src="../media/qtVersion.png">
		</p>
	2. Compilers -> add the path of C and C++ compilers `rpi-build/tmp/deploy/sdk/yes/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++` with any name    
		<p align="center">
  		<img  src="../media/qtCompiler.png">
		</p>  
	3. Debuggers -> add the remote debugger path `rpi-build/tmp/deploy/sdk/yes/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gdb`  with any name  
		<p align="center">
  		<img  src="../media/qtDebugger.png">
		</p>  
	4. CMake -> it might be auto detected, in case if not add the path `rpi-build/tmp/deploy/sdk/yes/sysroots/x86_64-pokysdk-linux/usr/bin/cmake` 
		<p align="center">
		<img  src="../media/qtCmake.png">
		</p>  
	5. Kits -> add the previous setups with:  
		**Device type** -> Generic Linux Device  
		**sysroot** -> path `rpi-build/tmp/deploy/sdk/yes/sysroots/aarch64-poky-linux`  
		**QT mkspec** -> linux-oe-g++    
		<p align="center">
  		<img  src="../media/qtKits.png">
		</p> 	


<a name="qtCreator"></a>
## Creating Qt project with C++

1. Create new project as **Application** -> Qt Widgets Application  
2. When it comes to **Kit Selection** choose your pc and your device, if your pc is not listed add it from devices window  
3. Following the steps will create main.cpp which is the whole project main function, source/header and ui files for your main class and .pro file which configure your project  
4. The pro file is updated automatically on each file creation/deletion. You will only need to add the remote executable path where your application will be deployed in your image as  
```
	target.path = /home/root/app
	INSTALLS += target
```  
5. Executing system commands may require root permissions on pc such as mount, mkdir, date.. etc. You can launch your application as sudoer to avoid this problem in testing your application ```sudo qtcreator```    
6. In my Qt project there is a configuration file `targetconfiguration.h` to specify on each target the application is going to run because of some commands that don't run on both  

<a name="multimedia"></a>
## Running multimedia Mp3 and Mp4

1. Qt helpful classes for Mp3 are **QMediaPlayer** and **QMediaPlaylist**   
2. Qt helpful classes for Mp4 are **QGraphicsScene, QGraphicsVideoItem, QMediaPlayer** and **QMediaPlaylist**  
3. Using QMediaPlayer module requires some packages, make sure they are installed  
```
$ sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5
```  
4. Make sure that qmake selected in Qt version for your PC is Qt5 not Qt4  ``` /usr/lib/qt5/bin/qmake ```  
5. Clean your project, add ```QT += multimedia multimediawidgets``` to your .pro file and run qmake again (right click on your project)   
6. Qt media modules rely on GStreamer, make sure that it's enabled as mentioned in [Adding Sound](#enableSound) section   


---
<a name="conclusion"></a>
# Conclusion  

- A simple demo of the system running on both Ubuntu and Raspberry Pi:    

[![Infotainment System (Yocto & Qt)](https://github.com/alzahraaelsallakh/Infotainment-System-Yocto/blob/media/demoThumb.PNG)](https://www.youtube.com/watch?v=YtsgnpMfhsA&t=5s "Infotainment System (Yocto & Qt)")

- Applied features:  
   1. Mp3 Player (.mp3 files)  
   2. Mp4 Player (.mp4 files)   
   3. Bluetooth connection (Audio streaming)  
   4. Settings Panel  (Dark theme and date/time)  
	
- Audio streaming via Bluetooth and setting time/date are not working on RPI due to previously mentioned issues     
- The image selection may not be the best for the issues I faced, you can try another with the same configurations  
- I tried to go with rpi-basic-image but after building I got the following warning  
```
The image 'rpi-basic-image' is deprecated, please use 'core-image-base' instead
```

---
<a name="references"></a>
# References 

1. [Qt Documentation](https://doc.qt.io/qt-5/index.html)  
2. [Using Qt Creator to cross-compile and debug Raspberry Pi Qt5 apps ](https://jumpnowtek.com/rpi/Qt-Creator-Setup-for-RPi-cross-development.html) 
3. [Qt C++ GUI Tutorial for Beginners](https://www.youtube.com/playlist?list=PLS1QulWo1RIZiBcTr5urECberTITj7gjA)  
4. [Intel Edison audio output to Bluetooth speaker](https://tl00.wordpress.com/2015/01/28/11/)
