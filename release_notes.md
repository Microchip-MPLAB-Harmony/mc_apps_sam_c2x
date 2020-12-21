﻿---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes
## Microchip MPLAB® Harmony 3 Motor Control Application Examples for SAMC2x family v3.0.0


### Applications

Applications migrated from motor_control repository to this application repository for SAMC2x family. 

| Development Board | Number of Applications | 
| --- | --- | 
|[dsPICDEM™ MCLV-2 Low Voltage Development Board](https://www.microchip.com/DevelopmentTools/ProductDetails/DM330021-2) | 2 |
|[dsPICDEM™ MCHV-3 High Voltage Development Board](https://www.microchip.com/developmenttools/ProductDetails/dm330023-3) | 3 |
|||


### Required MPLAB Harmony v3 Modules
* csp v3.8.3
* x2c v1.1.3
* motor_control v3.6.0
* dev_packs v3.8.0
* mhc v3.6.5

### Known Issues

* Isolated EDBG Card 
   * Programming or debugging PIC32CM, SAM C/D2x or SAM D/E5x MCU, using Isolated EDBG Card (board revision #02-10824-R1) on dsPICDEM™ MCHV-3 High Voltage Development Board may inhibit MCU from executing instructions if the MCU is reset by pressing on board 'Reset' switch or power cycling the board. Refer to the [Isolated EDBG Debugger Product Change Notice](https://www.microchip.com/DevelopmentTools/ProductDetails/AC320202) for details of hardware modification needed to resolve this issue.

  * If programming failure occurs with message "java.lang.RuntimeException:RDDI_DAP_OPERATION_FAILED", then reset the Isolated EDBG Card's configuration by Go to File -> Project Properties -> EDBG -> Reset 

* For any demos running on SAMC21 Motor Control PIM, if any failures are observed while trying to use X2CScope, these failures may occur due to shortage of CPU computation bandwidth. In such cases, enable "RAM_EXECUTE" mode which speeds up execution by executing certain functions from RAM memory instead of Flash memory.


### Development Tools

* [MPLAB X IDE v5.45](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers)
* MPLAB X IDE plug-ins:
  * MPLAB Harmony Configurator (MHC) v3.6.2
  * X2CScope v1.3.0.
