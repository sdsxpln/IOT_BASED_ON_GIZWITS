/**
 ******************************************************************************
  * @file       bsp_htsensor.h
  * @author     yearnext
  * @version    1.0.0
  * @date       2016��9��17��
  * @brief      ��ʪ�ȴ����� ͷ�ļ�
  * @par        ����ƽ̨                                  
  *                  CC2530
  * @par        ����Ƶ��                                  
  *                  32MHz
  * @par        ����ƽ̨									                          
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
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void ht_sensor_init(void);
extern void report_ht_sensor_data( void );
extern bool ht_sensor_cmd_resolve(MYPROTOCOL_USER_DATA *data);

#endif      /* __SMART_DEVICE_HT_SENSOR_H__ */

/**********************************END OF FILE*********************************/