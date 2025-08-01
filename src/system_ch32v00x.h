/********************************** (C) COPYRIGHT *******************************
 * File Name          : system_ch32v00x.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : CH32V00x Device Peripheral Access Layer System Header File.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __SYSTEM_CH32V00x_H
#define __SYSTEM_CH32V00x_H

#ifdef __cplusplus
extern "C" {
#endif 

#define FCPU    48000000
//#define USE_HSE

#define USE_SPL

/* System_Exported_Functions */  
extern void SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /*__CH32V00x_SYSTEM_H */
