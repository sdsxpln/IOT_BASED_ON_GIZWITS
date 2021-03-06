 /**
 ******************************************************************************
  * @file       hal_ds1302.c
  * @author     yearnext
  * @version    1.0.0
  * @date       2016年9月17日
  * @brief      ds1302 驱动源文件
  * @par        工作平台                                  
  *                  CC2530
  * @par        工作频率                                  
  *                  32MHz
  * @par        编译平台									                         
  * 				 IAR
 ******************************************************************************
  * @note
  * 1.XXXXX                  						                     
 ******************************************************************************
 */

/**
 * @defgroup ds1302驱动模块
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_ds1302.h"
#include <ioCC2530.h>
#include <string.h>
#include "hal_board_cfg.h"
#include "OnBoard.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if USE_HAL_DS1302
// SCK端口功能配置
#define DS1302_SCK_WrMode() ( DS1302_SCK_SEL &= ~DS1302_SCK_BV, DS1302_SCK_DIR |=  DS1302_SCK_BV )
#define DS1302_SCK_RdMode() ( DS1302_SCK_SEL &= ~DS1302_SCK_BV, DS1302_SCK_DIR &= ~DS1302_SCK_BV )
#define SET_DS1302_SCK()    ( DS1302_SCK_PORT = DS1302_SCK_POLARITY(1) )
#define RST_DS1302_SCK()    ( DS1302_SCK_PORT = DS1302_SCK_POLARITY(0) )
#define Read_DS1302_SCK()   ( DS1302_SCK_POLARITY(DS1302_SCK_PORT) )
// SDA端口功能配置
#define DS1302_SDA_WrMode() ( DS1302_SDA_SEL &= ~DS1302_SCK_BV, DS1302_SDA_DIR |=  DS1302_SDA_BV )
#define DS1302_SDA_RdMode() ( DS1302_SDA_SEL &= ~DS1302_SCK_BV, DS1302_SDA_DIR &= ~DS1302_SDA_BV )
#define SET_DS1302_SDA()    ( DS1302_SDA_PORT = DS1302_SDA_POLARITY(1) )
#define RST_DS1302_SDA()    ( DS1302_SDA_PORT = DS1302_SDA_POLARITY(0) )
#define Read_DS1302_SDA()   ( DS1302_SDA_POLARITY(DS1302_SDA_PORT) )
// RST端口功能配置
#define DS1302_RST_WrMode() ( DS1302_RST_SEL &= ~DS1302_SCK_BV, DS1302_RST_DIR |=  DS1302_RST_BV )
#define DS1302_RST_RdMode() ( DS1302_RST_SEL &= ~DS1302_SCK_BV, DS1302_RST_DIR &= ~DS1302_RST_BV )
#define SET_DS1302_RST()    ( DS1302_RST_PORT = DS1302_RST_POLARITY(1) )
#define RST_DS1302_RST()    ( DS1302_RST_PORT = DS1302_RST_POLARITY(0) )
#define Read_DS1302_RST()   ( DS1302_RST_POLARITY(DS1302_RST_PORT) )

// 延时功能定义
#define DS1302_DELAY()      Onboard_wait(2)

// DS1302 内部寄存器操作功能
// DS1302 写保护功能
#define DS1302_ENABLE_WP()    ds1302_wr_data(DS1302_CONTROL_REG_ADDR,0x80)
#define DS1302_DISABLE_WP()   ds1302_wr_data(DS1302_CONTROL_REG_ADDR,0x00)
#define DS1302_WP_STATE()     ((ds1302_rd_data(DS1302_CONTROL_REG_ADDR)) ? (1) : (0))
// DS1302 涓流充电配置
#define DS1302_TRICK_CONFIG() ds1302_wr_data(DS1302_TRICK_REG_ADDR,0xA5)
// DS1302 BCD与HEX互相转换
#define DS1302_HEX2BCD(hex)   ((uint8)(((((hex)/10)<<4) | ((hex)%10))))
#define DS1302_BCD2HEX(bcd)   ((uint8)((((bcd)>>4)*10) + ((bcd)&0x0F)))

#endif
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if USE_HAL_DS1302
/**
 *******************************************************************************
 * @brief       DS1302初始化程函数
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void hal_ds1302_init( void )
{
    DS1302_RST_WrMode();
    RST_DS1302_RST();
    
    DS1302_SCK_WrMode();
    SET_DS1302_SCK();
    
    DS1302_SDA_WrMode();
    SET_DS1302_SDA();
    DS1302_SDA_RdMode();
    
//    DS1302_TRICK_CONFIG();
}

/**
 *******************************************************************************
 * @brief       DS1302读取一字节
 * @param       [in/out]  void
 * @return      [in/out]  rd_data    读取到的数据
 * @note        None
 *******************************************************************************
 */
static uint8 ds1302_rd_byte( void )
{
    uint8 rd_data = 0;
    uint8 i = 0, j = 0;
    
    DS1302_SDA_RdMode();
    
    for( i=0; i<8; i++ )
    {
        j = (uint8)Read_DS1302_SDA();
        rd_data |= j << i;

        DS1302_DELAY();
        
        SET_DS1302_SCK();
        DS1302_DELAY();
        
        RST_DS1302_SCK();
        DS1302_DELAY();
    }
    
    return rd_data;
}

/**
 *******************************************************************************
 * @brief       DS1302写入一字节
 * @param       [in/out]  wr_data    写入的数据
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
static void ds1302_wr_byte( uint8 wr_data )
{
    uint8 i = 0;
    
    DS1302_SDA_WrMode();
    
    for( i=0; i<8; i++ )
    {        
        (wr_data & BV(i)) ? (SET_DS1302_SDA()) : (RST_DS1302_SDA());

        DS1302_DELAY();
        
        SET_DS1302_SCK();
        DS1302_DELAY();
        
        RST_DS1302_SCK();
        DS1302_DELAY();
    }

    DS1302_SDA_RdMode();
}

///**
// *******************************************************************************
// * @brief       BCD码转16进制
// * @param       [in/out]  bcd    bcd码
// * @return      [in/out]  hex    转换好的数据
// * @note        None
// *******************************************************************************
// */
//uint8 ds1302_bcd2hex( uint8 bcd )
//{
//    uint8 hex = 0;
//    
//    hex += (bcd >> 4)*10;
//    hex += bcd & 0x0F;
//    
//    return hex;
//}
//
///**
// *******************************************************************************
// * @brief       16进制转BCD码
// * @param       [in/out]  hex    hex数据
// * @return      [in/out]  bcd    转换好的数据
// * @note        None
// *******************************************************************************
// */
//uint8 ds1302_hex2bcd( uint8 hex )
//{
//    uint8 bcd = 0;
//    
//    bcd |= (hex/10) << 4;
//    bcd |= hex % 10;
//    
//    return bcd;
//}

/**
 *******************************************************************************
 * @brief       DS1302写入数据
 * @param       [in/out]  wr_addr    写入的地址
 * @param       [in/out]  wr_data    写入的数据
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void ds1302_wr_data( uint8 wr_addr, uint8 wr_data )
{
#if USE_DS1302_DEBUG
    uint8 cmd_data = wr_addr;
#else
    uint8 cmd_data = 0x80 | ( wr_addr << 1 );
#endif
    RST_DS1302_RST();
    RST_DS1302_SCK();
    SET_DS1302_RST();
    
    DS1302_DELAY();
    ds1302_wr_byte(cmd_data);
    ds1302_wr_byte(wr_data);
    
    SET_DS1302_SCK();
    RST_DS1302_RST();
}

/**
 *******************************************************************************
 * @brief       DS1302读取数据
 * @param       [in/out]  wr_addr    写入的地址
 * @return      [in/out]  rd_data    读取到的数据
 * @note        None
 *******************************************************************************
 */
uint8 ds1302_rd_data( uint8 rd_addr )
{
#if USE_DS1302_DEBUG
    uint8 cmd_data = rd_addr;
#else 
    uint8 cmd_data = 0x81 | ( rd_addr << 1 );
#endif
    uint8 rd_data = 0;
    
    RST_DS1302_RST();
    RST_DS1302_SCK();
    SET_DS1302_RST();
    
    DS1302_DELAY();
    ds1302_wr_byte(cmd_data);
    rd_data = ds1302_rd_byte();
    
    SET_DS1302_SCK();
    RST_DS1302_RST();
    
    return rd_data;
}

/**
 *******************************************************************************
 * @brief       从DS1302中读取时间
 * @param       [in/out]  void
 * @return      [in/out]  DS1302_TIME   读取到的时间数据
 * @note        None
 *******************************************************************************
 */
void ds1302_rd_time( void *time )
{
#if USE_DS1302_DEBUG
    uint8 data[7];
#else
    uint8 *data = (uint8 *)time;
#endif

#if USE_DS1302_DEBUG
    uint8 i = 0;
    
    data[0] = ds1302_rd_data(0x81);
    data[1] = ds1302_rd_data(0x83);
    data[2] = ds1302_rd_data(0x85);
    data[3] = ds1302_rd_data(0x87);
    data[4] = ds1302_rd_data(0x89);
    data[5] = ds1302_rd_data(0x8B);
    data[6] = ds1302_rd_data(0x8D);
    
    for( i=0; i<7; i++ )
    {
        data[i] = DS1302_BCD2HEX(data[i]);
    }
#else
    uint8 i = 0;
    
    for( i=0; i<7; i++ )
    {
        data[i] = DS1302_BCD2HEX(ds1302_rd_data(DS1302_RD_TIME_ADDR+i));
    }
#endif
    
#if USE_DS1302_DEBUG
    memcpy(time, &data, 7);
#endif
}

/**
 *******************************************************************************
 * @brief       设置DS1302时间
 * @param       [in/out]  newtime       设置的时间
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void ds1302_wr_time( void *time )
{
#if USE_DS1302_DEBUG
    uint8 data[7];
    memcpy(&data, time, 7);
#else
    uint8 *data = (uint8 *)time;
#endif
    
#if USE_DS1302_DEBUG
    if( ds1302_rd_data(0x81) & 0x80 )
    {
        ds1302_wr_data(0x8E,0x00);
        
        ds1302_wr_data(0x80,DS1302_HEX2BCD(data[0]));
        ds1302_wr_data(0x82,DS1302_HEX2BCD(data[1]));
        ds1302_wr_data(0x84,DS1302_HEX2BCD(data[2]));
        ds1302_wr_data(0x86,DS1302_HEX2BCD(data[3]));
        ds1302_wr_data(0x88,DS1302_HEX2BCD(data[4]));
        ds1302_wr_data(0x8A,DS1302_HEX2BCD(data[5]));
        ds1302_wr_data(0x8C,DS1302_HEX2BCD(data[6]));
        
        ds1302_wr_data(0x90,0xA5);
        ds1302_wr_data(0x8E,0x80);
    }
    else
    {
        ds1302_wr_data(0x80,0x80);
    }
#else
    uint8 i = 0;
    
    DS1302_DISABLE_WP();
    
    for( i=0; i<7; i++ )
    {
        ds1302_wr_data(DS1302_RD_TIME_ADDR+i,DS1302_HEX2BCD(data[i]));
    }
    
    DS1302_ENABLE_WP();
#endif
}

#endif

/** @}*/     /* ds1302驱动模块 */

/**********************************END OF FILE*********************************/
