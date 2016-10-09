/**
 ******************************************************************************
  * @file       gizwits.h
  * @author     yearnext
  * @version    1.0.0
  * @date       2016��9��17��
  * @brief      gizwits ͷ�ļ�
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

#if defined ( USE_GIZWITS_MOD )

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_GIZWITS_H__
#define __APP_GIZWITS_H__

/* Includes ------------------------------------------------------------------*/
#include "ZComDef.h"
#include "gizwits_product.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void app_gizwits_init( void );

extern uint16 Gizwits_ProcessEven( uint8 task_id, uint16 events );
#endif      /* __APP_GIZWITS_H__ */

#endif

/**********************************END OF FILE*********************************/