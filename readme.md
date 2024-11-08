---
title: Microchip MPLAB® Harmony 3 Motor Control Application Examples for SAM C2x family
nav_order: 1
has_children: true
has_toc: false
---

# Microchip MPLAB® Harmony 3 Motor Control Application Examples for SAM C2x family

MPLAB Harmony 3 is an extension of the MPLAB® ecosystem for creating
embedded firmware solutions for Microchip 32-bit SAM and PIC32 microcontroller
and microprocessor devices.  Refer to the following links for more information.
 - [Microchip 32-bit MCUs for Motor Control Applications](https://www.microchip.com/design-centers/motor-control-and-drive/control-products/32-bit-solutions)
 - [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
 - [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
 - [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
 - [Microchip MPLAB Harmony](https://www.microchip.com/mplab/mplab-harmony)
 - [Microchip MPLAB Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 Motor Control application exmaples for SAMC2x family. Users can use these examples as a reference for
developing their own motor control applications. Refer to the following links for release
notes and licensing information.

 - [Release Notes](./release_notes.md)
 - [MPLAB Harmony License](mplab_harmony_license.md)

## Contents Summary

| Folder     | Description                                               |
|------------|-----------------------------------------------------------|
| apps       | Demonstration applications for Motor Control              |
|||

## Documentation

Click [here](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_INTRODUCTION&redirect=true) to view the online documentation of code examples hosted in this repository.

To view the documentation offline, follow these steps:
 - Download the publication as a zip file from [here](https://onlinedocs.microchip.com/download/GUID-686B08E8-FD18-4AAD-87B9-32B527406A88?type=webhelp).
 - Extract the zip file into a folder.
 - Navigate to the folder and open **index.html** in a web browser of your choice.
## Configurable Motor Control Examples (MCC with Harmony QSpin Motor Control )

The following applications are provided to demonstrate the usage of Harmony QSpin to generate motor control application with Microchip's 32-bit MCUs.


| Name | Description|Control Board|Inverter Board|
|:-----|:-----------|:------------|:-------------|
| [PMSM FOC using PLL Estimator](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_PMSM_FOC_PLL&redirect=true) | Sensorless Field Oriented Control of PMSM using PLL Estimator | [ATSAMC21 Motor Control DIM](https://www.microchip.com/en-us/development-tool/EV74G56A) | [MCLV-48V-300W Development Board](https://www.microchip.com/en-us/development-tool/ev18h47a) |
| [PMSM FOC using ROLO Estimator](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_PMSM_FOC_REDUCED_ORDER_LUENBERGER_OBSERVER_FIELD_WEAKENING_MTPA&redirect=true) | Sensorless Field Oriented Control of PMSM using ROLO Estimator | [ATSAMC21 Motor Control DIM](https://www.microchip.com/en-us/development-tool/EV74G56A) | [MCLV-48V-300W Development Board](https://www.microchip.com/en-us/development-tool/ev18h47a) |

## Static Motor Control Examples

These applications contain static algorithm code and peripherals are configured using MHC. Configurations can be changed in userparam.h file. 


| Name | Description|Control Board|Inverter Board|
|:-----|:-----------|:------------|:-------------|
| [ACIM Open Loop V/Hz](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_ACIM_VHZ_CONTROL&redirect=true) | Open Loop V/Hz Control of ACIM |[ATSAMC21 Motor Control Plugin Module](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/MA320206) |[dsPICDEM™ MCHV-3 Support](https://www.microchip.com/developmenttools/ProductDetails/dm330023-3)|
| [BLDC Block Commutation using Hall Sensors](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_BLDC_BLOCK_COMMUTATION_HALL_SENSOR&redirect=true) | Block Commutation Control of BLDC motors using Hall Sensors |[ATSAMC21 Motor Control Plugin Module](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/MA320206) |[dsPICDEM™ MCLV-2 Support](https://www.microchip.com/DevelopmentTools/ProductDetails/DM330021-2) |
|[PMSM FOC using Reduced Order Luenberger Observer with Single Shunt](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MC_APPS_SAM_C2X_PMSM_FOC_SINGLE_SHUNT_CURRENT_RECONSTRUCTION&redirect=true)| Sensorless Field Oriented Control of PMSM using Reduced Order Luenberger Observer with Single Shunt Current Sense | [ATSAMC21 Motor Control Plugin Module](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/MA320206)|[dsPICDEM™ MCLV-2 Support](https://www.microchip.com/DevelopmentTools/ProductDetails/DM330021-2)|


[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/mc/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg)](https://github.com/Microchip-MPLAB-Harmony/mc/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg)](https://github.com/Microchip-MPLAB-Harmony/mc/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg)](https://github.com/Microchip-MPLAB-Harmony/mc/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg)]()
____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/mc_apps_sam_c2x.svg?style=social)]()
