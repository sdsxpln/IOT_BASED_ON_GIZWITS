 /**
 ******************************************************************************
  * @file       bsp_htsensor.c
  * @author     yearnext
  * @version    1.0.0
  * @date       2016年9月17日
  * @brief      温湿度传感器 源文件
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
 * @defgroup 温湿度传感器模块
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_dht11.h"
#include <string.h>
#include "bsp_htsensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef DHT11_DATA_t ht_sensor_t;

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HT_SENSOR_TICK_CMD   (0x00)
#define HT_SENSOR_REPORT_CMD (0x01)
#define HT_SENSOR_READ_CMD   (0x02)

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       初始化温湿度传感器
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void ht_sensor_init(void)
{
	hal_dht11_init();
}

/**
 *******************************************************************************
 * @brief       上报温湿度数据
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void report_ht_sensor_data( void )
{
	ht_sensor_t ht_sensor_data = dht11_rd_data();
	MYPROTOCOL_USER_DATA user_data;
	
	memcpy(&user_data.data,&ht_sensor_data,sizeof(ht_sensor_data));
	user_data.len = sizeof(ht_sensor_data);
	user_data.cmd = HT_SENSOR_REPORT_CMD;
	
	MYPROTOCO_S2H_MSG_SEND(create_d2w_wait_packet,&user_data);
}

/**
 *******************************************************************************
 * @brief       温湿度传感器模块命令解析
 * @param       [in/out]  *data    命令数据
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
bool ht_sensor_cmd_resolve(MYPROTOCOL_USER_DATA *data)
{
	switch(data->cmd)
	{
        case HT_SENSOR_READ_CMD:
            report_ht_sensor_data();
            return true;
			break;
		default:
			break;
	}
	
	return false;
}

/** @}*/     /* 温湿度传感器模块 */

/**********************************END OF FILE*********************************/