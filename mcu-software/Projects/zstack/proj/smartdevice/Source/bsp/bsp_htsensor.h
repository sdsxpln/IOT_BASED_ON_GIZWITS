/**
 ******************************************************************************
  * @file       bsp_htsensor.h
  * @author     yearnext
  * @version    1.0.0
  * @date       2016年9月17日
  * @brief      温湿度传感器 头文件
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMART_DEVICE_HT_SENSOR_H__
#define __SMART_DEVICE_HT_SENSOR_H__

/* Includes ------------------------------------------------------------------*/
#include "comdef.h"
#include "myprotocol.h"

/* Exported macro ------------------------------------------------------------*/
#if MYPROTOCOL_DEVICE_IS_HT_SENSOR
#define bspDeviceInit() bspHTSensorInit()
#define deviceMessageHandler htsensorMessageHandler
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void bspHTSensorInit(void);
extern void reportHTSensorData( void );
extern void readHTSensorData( void );
extern bool htsensorMessageHandler(MYPROTOCOL_FORMAT_t *);

#endif      /* __SMART_DEVICE_HT_SENSOR_H__ */

/**********************************END OF FILE*********************************/
