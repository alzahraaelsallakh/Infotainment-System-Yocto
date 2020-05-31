# Infotainment-System-Yocto

This repo if for ITI-Intake 40 graduation project

## What is a car infotainment system?
An in-vehicle infotainment system is a combination of systems that deliver entertainment and information to the driver and passengers. They do it using audio and video interfaces, touch screen displays, button panels, voice commands, and many other features. [check more!](https://concisesoftware.com/car-infotainment-system-guide/)

<p align="center">
  <img width="700" height="346" src="../media/info-sys-demo.jpg">
</p>

---
# Getting Started

## Building the System using Yocto

1. Download the Poky build system then checkout to zeus branch  
```
$ git clone git://git.yoctoproject.org/poky
$ git checkout zeus
``` 
2. Download RPI BSP then checkout to zeus branch 
```
$ git clone https://github.com/agherzan/meta-raspberrypi.git 
$ git checkout zeus
``` 
3. Download openembedded then checkout to zeus branch
```
$ git clone https://github.com/openembedded/meta-openembedded.git
$ git checkout zeus
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
8. Build the poky image using the build engine **BitBake**
```
$ bitbake core-image-sato
```  
Note: core-image-sato is selected as it supports X11 and a GUI server is required
