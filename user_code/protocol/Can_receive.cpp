#include "can_receive.h"

#include "cmsis_os.h"
#include "main.h"

#include "bsp_can.h"
#include "can.h"

#include "struct_typedef.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void Can_receive::init()
{
    can_filter_init();
}

void Can_receive::get_mine_motor_measure(uint8_t num, uint8_t data[8])
{
    mine_motive_motor[num].last_ecd = mine_motive_motor[num].ecd;
    mine_motive_motor[num].ecd = (uint16_t)(data[0] << 8 | data[1]);
    mine_motive_motor[num].speed_rpm = (uint16_t)(data[2] << 8 | data[3]);
    mine_motive_motor[num].given_current = (uint16_t)(data[4] << 8 | data[5]);
    mine_motive_motor[num].temperate = data[6];
}

/**
 * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
 * @param[in]      motor1: (0x201) 2006电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor2: (0x202) 2006电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
 * @retval         none
 */
void Can_receive::can_cmd_mine_motive_motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    mine_tx_message.StdId = CAN_GIMBAL_MOTIVE_ALL_ID;
    mine_tx_message.IDE = CAN_ID_STD;
    mine_tx_message.RTR = CAN_RTR_DATA;
    mine_tx_message.DLC = 0x08;
    mine_can_send_data[0] = motor1 >> 8;
    mine_can_send_data[1] = motor1;
    mine_can_send_data[2] = motor2 >> 8;
    mine_can_send_data[3] = motor2;
    mine_can_send_data[4] = motor3 >> 8;
    mine_can_send_data[5] = motor3;
    mine_can_send_data[6] = motor4 >> 8;
    mine_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &mine_tx_message, mine_can_send_data, &send_mail_box);
}

/**
 * @brief          返回拨矿电机 2006电机数据指针
 * @param[in]      i: 电机编号,范围[0,3]
 * @retval         电机数据指针
 */
const motor_measure_t *Can_receive::get_mine_motive_motor_measure_point(uint8_t i)
{
    return &mine_motive_motor[i];
}
