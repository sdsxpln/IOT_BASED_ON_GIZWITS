 /**
 ******************************************************************************
  * @file       bsp_light.c
  * @author     yearnext
  * @version    1.0.0
  * @date       2016年9月17日
  * @brief      智能电灯配置源文件
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
 * @defgroup 智能电灯配置模块
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#if (SMART_DEVICE_TYPE) == (MYPROTOCOL_DEVICE_LIGHT)
#include "bsp_light.h"
#include "timer_config.h"
#include <string.h>
#include "app_save.h"
#include "app_timer.h"
#include "myprotocol.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// 灯的亮度与PWM占空比之间转换
#define Light_Brightness_Conversion(n) ( 0xFF - (n) )
#define Get_Light_Brightness()         ( Get_TIM4_CH0_Duty() )

/* Private typedef -----------------------------------------------------------*/
// 电灯控制命令
typedef enum
{
    RD_LIGHT_BRIGHTNESS   = 0x10,
    WR_LIGHT_BRIGHTNESS   = 0x11,
    RD_LIGHT_SINGLE_TIMER = 0x12,
    WR_LIGHT_SINGLE_TIMER = 0x13,
    RD_LIGHT_CIRCUL_TIMER = 0x14,
    WR_LIGHT_CIRCUL_TIMER = 0x15,
    RD_LIGHT_LOAD_SET     = 0x16,
    WR_LIGHT_LOAD_SET     = 0x17,
}DEVICE_LIGHT_CMD;

typedef struct
{
    uint8 brightness;
}LIGHT_BRIGHTNESS_CMD_DATA;

/* Private variables ---------------------------------------------------------*/
static DEVICE_LIGHT_SAVE_DATA light;

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *******************************************************************************
 * @brief       上报电灯当前的亮度信息
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
static void report_brightness_data( void )
{
    MYPROTOCOL_USER_DATA user_data;
    memset(&user_data,0,sizeof(MYPROTOCOL_USER_DATA));
    
    user_data.cmd = RD_LIGHT_BRIGHTNESS;
    user_data.data[0] = light.status.now;
    user_data.len = 1;
    MYPROTOCO_S2H_MSG_SEND(create_d2w_wait_packet,&user_data);
}

/**
 *******************************************************************************
 * @brief       上报电灯定时器的数据
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
static void report_timer_data( uint8 timer )
{
    MYPROTOCOL_USER_DATA user_data;
    memset(&user_data,0,sizeof(MYPROTOCOL_USER_DATA));
    
    if( timer == 0 )
    {
        user_data.cmd = RD_LIGHT_SINGLE_TIMER;
    }
    else if( timer == 1 )
    {
        user_data.cmd = RD_LIGHT_CIRCUL_TIMER;
    }
    else
    {
        return;
    }
    
    memcpy(&user_data.data,&light.timer[timer],sizeof(DEVICE_TIMER));
    user_data.len = sizeof(DEVICE_TIMER);
    
    MYPROTOCO_S2H_MSG_SEND(create_d2w_wait_packet,&user_data);
}

/**
 *******************************************************************************
 * @brief       电灯初始化函数
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void bsp_light_init( void )
{
    Timer4_PWM_Init( TIM4_CH0_PORT_P2_0 );
    set_light_brightness(Light_OFF_Brightness);
    
    // FLASH 数据初始化
    if( Device_FirstWrite_Check() == false \
        || osal_nv_read(DEVICE_LIGHT_SAVE_ID,0,DEVICE_LIGHT_DATA_SIZE,(void *)&light) != SUCCESS )
    {
        memset(&light,0,sizeof(light));
        light.status.now = Light_OFF_Brightness;
        light.status.last = Light_ON_Brightness;
        
        osal_nv_item_init(DEVICE_LIGHT_SAVE_ID,DEVICE_LIGHT_DATA_SIZE,NULL);
        osal_nv_write(DEVICE_LIGHT_SAVE_ID,0,DEVICE_LIGHT_DATA_SIZE,(void *)&light);
    }
    
    set_light_brightness(light.status.now);
}

/**
 *******************************************************************************
 * @brief       电灯亮度设置函数
 * @param       [in/out]  uint8    电灯的亮度
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void set_light_brightness( uint8 brightness )
{
    TIM4_CH0_UpdateDuty( Light_Brightness_Conversion(light.status.now) );
}

/**
 *******************************************************************************
 * @brief       读取电灯亮度函数
 * @param       [in/out]  void
 * @return      [in/out]  uint8    电灯的亮度
 * @note        None
 *******************************************************************************
 */
uint8 get_light_brightness( void )
{
    return Light_Brightness_Conversion( Get_Light_Brightness() );
}

/**
 *******************************************************************************
 * @brief       电灯亮度设置函数
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void light_control_handler( uint8 brightness )
{
    if( brightness != get_light_brightness() )
    {
        if( light.status.now == 0xFE )
        {
            light.status.now = 0xFF;
        }
        
        light.status.last = light.status.now;
        light.status.now = brightness;
        
        set_light_brightness( light.status.now );
        
        osal_nv_write(DEVICE_LIGHT_SAVE_ID,\
                      (uint16)(&((DEVICE_LIGHT_SAVE_DATA *)0)->status),\
                      sizeof(light.status),\
                      (void *)&light.status);
        
        if( light.status.now == 0xFF )
        {
            light.status.now = 0xFE;
        }
        
    }
}

/**
 *******************************************************************************
 * @brief       电灯按键处理
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void light_switch_handler( void )
{
    uint8 temp = 0;
    
    if( light.status.now == 0xFE )
    {
        light.status.now = 0xFF;
    }
    
    if( light.status.now == Light_OFF_Brightness )
    {
        if( light.status.last != light.status.now )
        {
            temp = light.status.now;
            light.status.now = light.status.last;
            light.status.last = temp;
        }
        else
        {
            light.status.last = light.status.now;
            light.status.now = Light_ON_Brightness;
        }
    }
    else
    {
        light.status.last = light.status.now;
        light.status.now = Light_OFF_Brightness;
    }
    
    set_light_brightness( light.status.now );
    
    osal_nv_write(DEVICE_LIGHT_SAVE_ID,\
              ((uint16)&light.status - (uint16)&light),\
              sizeof(light.status),\
              (void *)&light.status); 
    
    if( light.status.now == 0xFF )
    {
        light.status.now = 0xFE;
    }
    
    report_brightness_data();
}

/**
 *******************************************************************************
 * @brief       电灯工作处理函数
 * @param       [in/out]  void
 * @return      [in/out]  void
 * @note        None
 *******************************************************************************
 */
void light_working_handler( void )
{
    uint8 i;
    
    for( i=0; i<SIMPLE_DEVICE_TIMER_NUM; i++ )
    {
        device_timer_handler((DEVICE_TIMER*)&light.timer[i],light_control_handler);
    }
}

/**
 *******************************************************************************
 * @brief       解析电灯命令数据
 * @param       [in/out]  *data    控制命令数据
 * @return      [in/out]  bool     解析状态
 * @note        None
 *******************************************************************************
 */
bool light_cmd_resolve( MYPROTOCOL_USER_DATA *data )
{    
    switch( data->cmd )
    {
        case DEVICE_TICK:
            break;
        case DEVICE_RESET:
            break;
        case DEVICE_REBOOT:
            break;
        case RD_LIGHT_BRIGHTNESS:
        {
            report_brightness_data();
            break;
        }
        case WR_LIGHT_BRIGHTNESS:
        {
            light_control_handler(data->data[0]);
            report_brightness_data();
            break;
        }
        case RD_LIGHT_SINGLE_TIMER:
        {
            report_timer_data(0);
            break;
        }
        case WR_LIGHT_SINGLE_TIMER:
        {
            memcpy(&light.timer[0],data->data,sizeof(light.timer[0]));
            report_timer_data(0);
            break;
        }
        case RD_LIGHT_CIRCUL_TIMER:
        {
            report_timer_data(1);
            break;
        }
        case WR_LIGHT_CIRCUL_TIMER:
        {
            memcpy(&light.timer[1],data->data,sizeof(light.timer[1]));
            report_timer_data(1);
            break;
        }
        case RD_LIGHT_LOAD_SET:
            break;
        case WR_LIGHT_LOAD_SET:
            break;
        default:
            return false;
            break;
    }
    return true;
}

#endif

/** @}*/     /* 智能电灯配置模块 */

/**********************************END OF FILE*********************************/
