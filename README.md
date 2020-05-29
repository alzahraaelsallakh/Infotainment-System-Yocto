# Infotainment-System-Yocto

This repo if for ITI-Intake 40 graduation project

## What is a car infotainment system?
An in-vehicle infotainment system is a combination of systems that deliver entertainment and information to the driver and passengers. They do it using audio and video interfaces, touch screen displays, button panels, voice commands, and many other features. [check more!](https://concisesoftware.com/car-infotainment-system-guide/)

<p align="center">
  <img width="700" height="346" src="../media/info-sys-demo.jpg">
</p>

## Technologies to be implemented
- Touch screen  
- Smartphone pairing  
- Multi-standard radio reception  
- Multimedia support  


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
Note: for my steps, both poky and meta-raspberrypi repos are in the same path   
  
3. Source “oe-init-build-env” script 
```
$ source poky/oe-init-build-env
```
4. Edit build/bblayers.conf and add meta-raspberrypi path to BBLAYERS variable  
```
BBLAYERS ?= " \
/ABSOLUTE/PATH/poky/meta \
/ABSOLUTE/PATH/poky/meta-poky \
/ABSOLUTE/PATH/poky/meta-yocto-bsp \
/ABSOLUTE/PATH/meta-raspberrypi \
"
```  
5. Edit build/local.conf by changing 
``` MACHINE ??= "qemux86-64"``` to be ```MACHINE ?= "raspberrypi3-64" ```   

6. Build the poky image using the build engine **BitBake**
```
$ bitbake rpi-basic-image
```
