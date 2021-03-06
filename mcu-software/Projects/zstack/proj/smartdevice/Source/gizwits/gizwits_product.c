/**
 ******************************************************************************
  * @file       gizwits_product.c
  * @author     Gizwits
  * @par        Modify
                    yearnext
  * @version    V03010101
  * @date       2016年12月1日
  * @brief      gizwits_product 源文件
  * @par        工作平台                                  
  *                  CC2530
  * @par        工作频率                                  
  *                  32MHz
  * @par        编译平台									                          
  * 				 IAR
  ******************************************************************************
  * @note
  * 机智云.只为智能硬件而生
  * Gizwits Smart Cloud  for Smart Products
  * 链接|增值ֵ|开放|中立|安全|自有|自由|生态
  * www.gizwits.com               						                      
  ******************************************************************************
 */

/**
 * @defgroup gizwits_product模块
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "gizwits_product.h"
#include "myprotocol.h"
#include "bsp_coord.h"
#include "devicelist.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if MYPROTOCOL_DEVICE_IS_COORD
/**
 *******************************************************************************
 * @brief       事件处理接口
 * @param       [in/out]  info    事件队列
 * @param       [in/out]  data    协议数据
 * @param       [in/out]  len     协议数据长度
 * @return      [in/out]  bool    返回状态
 * @note        None
 *******************************************************************************
 */
bool gizwitsEventProcess( eventInfo_t *info, uint8 *data, uint8 len )
{
  uint8 i = 0;
//  dataPoint_t *dataPointPtr = (dataPoint_t *)data;

  if((NULL == info) || (NULL == data))
  {
    return false;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_PACKET:
        GIZWITS_LOG("Evt: EVENT_PACKET\n");
        MyprotocolReceiveData((void *)data,deviceMessageHandler);
        break;
      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
        break;
      case WIFI_DISCON_ROUTER:
		break;
      case WIFI_CON_M2M:
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        break;
      case WIFI_CON_APP:
      {
        if( getGizwitsAPPStatus() == false )
        {
            MyprotocolSendData(NULL,NULL, createDeviceListChangePacket, MyprotocolD2WSendData);
            setGizwitsAPPStatus();
        }
        break;
      }
      case WIFI_DISCON_APP:
        break;
	  case WIFI_NTP:
//		gizUpdateTime((protocolUTT_t *)data);
	    break;
      case TRANSPARENT_DATA:
        break;

      default:
        break;
    }
  }

  return true;
}

#endif

/** @}*/     /** gizwits_product模块 */

/**********************************END OF FILE*********************************/
