#include "main.h"
#include "i2c.h"

#define PRESS_W_ADDRESS              (0x6D<<1)|0x00
#define PRESS_R_ADDRESS              (0x6D<<1)|0x01
#define PRESS_CMD_REG                0x30
#define PRESS_DATA_REG               0x06
//#define SINGLE_MODE

uint8_t single_mode_cmd = 0xA0;
uint8_t circle_mode_cmd_short = 0x1B;    //62.5ms采集一次数据
uint8_t circle_mode_cmd_medium = 0x2B;   //125ms采集一次数据
uint8_t circle_mode_cmd_long = 0x7B;     //1s采集一次数据
uint8_t circle_mode_cmd_stop = 0x08;     
uint8_t press_data[3] = {0};

#ifdef SINGLE_MODE

/*为避免说明书上需要开启测量后延时20ms带来的不利影响，将start和get_data函数分开
前者可在每次闭环控制后再次调用，到下一次控制时已经过去超过20ms了*/
void press_measure_start(void)
{
    HAL_I2C_Mem_Write(&hi2c2, PRESS_W_ADDRESS, PRESS_CMD_REG,
                        I2C_MEMADD_SIZE_8BIT, &single_mode_cmd, 1, 10000);
}

void press_measure_stop()
{
    return;     //组合模式（即单次模式）下硬件自动将soc置1，不用手动停止
}

uint32_t get_press_data(void)
{
    static uint32_t pressure;
    static uint8_t wait_for_complete;
    static uint32_t timeout;
    timeout = 100000;
    do{
        HAL_I2C_Mem_Read(&hi2c2, PRESS_R_ADDRESS, PRESS_CMD_REG,
                        I2C_MEMADD_SIZE_8BIT, &wait_for_complete, 1, 10000);
        Timeout--;
        if(!Timeout)
            break;
    } while (wait_for_complete & 0x08 != 0);    //等待soc位 置一测量完成
    //HAL_Delay(20);                            //说明书上要延时20ms，但是不延时似乎不影响测量
    HAL_I2C_Mem_Read(&hi2c2, PRESS_R_ADDRESS, PRESS_DATA_REG,
                     I2C_MEMADD_SIZE_8BIT, press_data, 3, 10000);
    pressure = (press_data[0] << 16) | (press_data[1] << 8) | (press_data[2]);
    if((pressure&0x00800000)==0){
        return 0;                               //最高位为0表示测得正压力，直接计压力为0
    }
    return 16777216 - pressure;                 //这是负压力的绝对值
}

#else

void press_measure_start(void)
{
    HAL_I2C_Mem_Write(&hi2c2, PRESS_W_ADDRESS, PRESS_CMD_REG,
                        I2C_MEMADD_SIZE_8BIT, &circle_mode_cmd_medium, 1, 10000);
}

void press_measure_stop(void)
{
    HAL_I2C_Mem_Write(&hi2c2, PRESS_W_ADDRESS, PRESS_CMD_REG,
        I2C_MEMADD_SIZE_8BIT, &circle_mode_cmd_stop, 1, 10000);
        //休眠模式(即循环模式)下，需要软件自行将soc置1停止采集
}

uint32_t get_press_data(void)
{
    static uint32_t pressure;
    HAL_I2C_Mem_Read(&hi2c2, PRESS_R_ADDRESS, PRESS_DATA_REG,
                     I2C_MEMADD_SIZE_8BIT, press_data, 3, 10000);
    pressure = (press_data[0] << 16) | (press_data[1] << 8) | (press_data[2]);
    if((pressure&0x00800000)==0){
        return 0;                               //最高位为0表示测得正压力，直接计压力为0
    }
    return 16777216 - pressure;                 //这是负压力的绝对值
}

#endif
