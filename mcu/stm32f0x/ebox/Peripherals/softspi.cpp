/**
  ******************************************************************************
  * @file    softspi.cpp
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
/**
 * Modification History:
 * -LQM (2016/9/5)
 *      *��ֲ��STM32F0,����HAL��LL��
 */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

SoftSpi::SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi)
{
    this->sck_pin =  sck;
    this->miso_pin = miso;
    this->mosi_pin = mosi;

}
void SoftSpi::begin(SPI_CONFIG_TYPE *spi_config)
{

    sck_pin->mode(OUTPUT_PP);
    miso_pin->mode(INPUT);
    mosi_pin->mode(OUTPUT_PP);

    config(spi_config);
    switch(mode)
    {
    case SPI_MODE0:
        sck_pin->reset();
        break;
    case SPI_MODE1:
        sck_pin->reset();
        break;
    case SPI_MODE2:
        sck_pin->set();
        break;
    case SPI_MODE3:
        sck_pin->set();
        break;
    }
}
void SoftSpi::config(SPI_CONFIG_TYPE *spi_config)
{
    current_dev_num = spi_config->dev_num;
    mode = spi_config->mode;
    bit_order = spi_config->bit_order;
    switch(spi_config->prescaler)
    {
    case LL_SPI_BAUDRATEPRESCALER_DIV2:
        spi_delay = 0;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV4:
        spi_delay = 1;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV8:
        spi_delay = 2;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV16:
        spi_delay = 4;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV32:
        spi_delay = 8;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV64:
        spi_delay = 16;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV128:
        spi_delay = 32;
        break;
    case LL_SPI_BAUDRATEPRESCALER_DIV256:
        spi_delay = 64;
        break;
    default:
        spi_delay = spi_config->prescaler;
        break;
    }
}
uint8_t SoftSpi::read_config(void)
{
    return current_dev_num;
}

uint8_t SoftSpi::transfer0(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //ʱ�ӿ��������0��
    //��һ���������أ���ȡ���ݣ�
    //�ڶ������½��أ�������ݣ�
    for (i = 0; i < 8; i++)
    { 
        if (bit_order == LL_SPI_LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);
        sck_pin->set();
        delay_us(spi_delay);
        sck_pin->reset();
    }
    return RcvData;
}
uint8_t SoftSpi::transfer1(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //ʱ�ӿ��������0��
    //��һ���������أ�������ݣ�
    //�ڶ������½��أ���ȡ���ݣ�
    for (i = 0; i < 8; i++)
    {
        ///////////////////���������//////////
        if (bit_order == LL_SPI_LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);
        sck_pin->set();
        /////////////////�½��ز���////////////
        delay_us(spi_delay);
        sck_pin->reset();
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
    }

    return RcvData;
}
uint8_t SoftSpi::transfer2(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //ʱ�ӿ��������1��
    //��һ�����½��أ���ȡ���ݣ�
    //�ڶ����������أ�������ݣ�
    for (i = 0; i < 8; i++)
    {
        sck_pin->reset();
        delay_us(spi_delay);
        if (bit_order == LL_SPI_LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);
        sck_pin->set();
    }

    return RcvData;
}
uint8_t SoftSpi::transfer3(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //ʱ�ӿ��������1��
    //��һ�����½��أ�������ݣ�
    //�ڶ����������أ���ȡ���ݣ�
    for (i = 0; i < 8; i++)
    {
        ///////////////////�½��������
        sck_pin->reset();
        delay_us(spi_delay);
        if (bit_order == LL_SPI_LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        /////////////////�����ز���////////////
        sck_pin->set();
        delay_us(spi_delay);
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
    }

    return RcvData;
}
uint8_t SoftSpi::transfer(uint8_t data)
{
    uint8_t RcvData = 0 ;
    switch(mode)
    {
    case SPI_MODE0:
        RcvData = transfer0(data);
        break;
    case SPI_MODE1:
        RcvData = transfer1(data);
        break;
    case SPI_MODE2:
        RcvData = transfer2(data);
        break;
    case SPI_MODE3:
        RcvData = transfer3(data);
        break;
    default :
        //return 0xff;
        break;
    }
    return RcvData;

}

int8_t  SoftSpi::write(uint8_t data)
{
    transfer(data);
    return 0;
}
int8_t  SoftSpi::write(uint8_t *data, uint16_t data_length)
{
    if(data_length == 0)
        return -1;
    while(data_length--)
    {
        transfer(*data++);
    }
    return 0;
}

uint8_t SoftSpi::read()
{
    return transfer(0xff);
}
int8_t  SoftSpi::read(uint8_t *data)
{
    *data = transfer(0xff);
    return 0;

}
int8_t  SoftSpi::read(uint8_t *rcvdata, uint16_t data_length)
{
    if(data_length == 0)
        return -1;
    while(data_length--)
    {
        *rcvdata++ = transfer(0xff);
    }
    return 0;
}

int8_t SoftSpi::take_spi_right(SPI_CONFIG_TYPE *spi_config)
{
    while((busy == 1) && (spi_config->dev_num != read_config()))
        delay_ms(1);
    if(spi_config->dev_num == read_config())
    {
        busy = 1;
        return 0;
    }
    config(spi_config);
    busy = 1;
    return 0;
}
int8_t SoftSpi::release_spi_right(void)
{
    busy = 0;
    return 0;
}



