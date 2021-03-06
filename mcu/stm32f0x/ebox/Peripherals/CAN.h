/**
  ******************************************************************************
  * @file    can.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CAN_H
#define __CAN_H

#include "common.h"

typedef enum
{
    BSP_CAN_33KBPS = 0,              // 波特率 33K 模式
    BSP_CAN_50KBPS,                  // 波特率 50K 模式
    BSP_CAN_625KBPS,                 // 波特率 62.5K 模式
    BSP_CAN_96KBPS,                  // 波特率 96K 模式
    BSP_CAN_100KBPS,                 // 波特率 100K 模式
    BSP_CAN_125KBPS,                 // 波特率 125K 模式
    BSP_CAN_250KBPS,                 // 波特率 250K 模式
    BSP_CAN_500KBPS,                 // 波特率 500K 模式
    BSP_CAN_800KBPS,                 // 波特率 800K 模式
    BSP_CAN_1MBPS,                   // 波特率 1M模式
} BSP_CAN_BAUD;

class CAN
{
public:
    CAN(CAN_TypeDef *CANx, Gpio *p_pin_rx, Gpio *p_pin_tx);
    void begin(BSP_CAN_BAUD bps);
    void set_filter(u8 Fifo, u8 nCanType, u8 num, u32 ID, u32 Mask);
    void interrupt(FunctionalState enable);
    void attach_interrupt(void (*callback_fun)(void));

    u8   write(CanTxMsg *pCanMsg);
    u8   read(CanRxMsg *pCanMsg, u16 WaitTime);

private:
    void set_bps(BSP_CAN_BAUD);

private:
    CAN_TypeDef *_CANx;
    Gpio *pin_rx;           //arduino pin number
    Gpio *pin_tx;           //arduino pin number

    BSP_CAN_BAUD _bps;
};

#endif
