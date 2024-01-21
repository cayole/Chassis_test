#include "vesc.h"

VESCParamTypedef VESC_U10, VESC_U8, VESC_U5,VESC_5060,VESC_4255;
VESCLimitTypedef VESCLimit;
VESCMotorTypedef VESCmotor[8];



void VESC_Init(void)
{
    VESC_U10.Motor_PolePairs = 21;
    VESC_U8.Motor_PolePairs = 21;
    VESC_U5.Motor_PolePairs = 7;
	  VESC_5060.Motor_PolePairs = 14;
		VESC_4255.Motor_PolePairs = 14;
	
    VESCLimit.Stuck_IfDect = true;
    VESCLimit.Timeout_IfDect = true;
    VESCLimit.IfReleaseWhenStuck = true;
    VESCLimit.MAX_Current = 40.0f;

    // TODO: 初始化底盘4个本杰明
    for (int i = 0; i < USEVESC; i++)
    {
        VESCmotor[i].limit = VESCLimit;
        VESCmotor[i].Param = VESC_4255;

        VESCmotor[i].enable = false;
        VESCmotor[i].begin = false;
        VESCmotor[i].mode = VESCRPM;
        VESCmotor[i].valueSet.speed = 0;
        VESCmotor[i].valueSet.duty = 0.1;
        VESCmotor[i].valueSet.Break_Current = 10;
        VESCmotor[i].valueSet.HandBreak_Current = 10;
    }
}

void VESC_ReceiveData_CAN2(CanRxMsg rx_message)
{
			if ((rx_message.IDE == CAN_ID_EXT) && (rx_message.RTR == CAN_RTR_DATA))
			{
					int32_t ind = 0;
					u8 id = (rx_message.ExtId & 0x0f) - 1;
					// if (id < 4)
					if(id==0)
					{
							if ((rx_message.ExtId >> 8) == CAN_PACKET_STATUS)
							{
									VESCmotor[id].valueReal.speed = buffer_32_to_float(rx_message.Data, 1e0, &ind) / VESCmotor[id].Param.Motor_PolePairs; // 放在第0 1 2 3个字节
									VESCmotor[id].valueReal.current = buffer_16_to_float(rx_message.Data, 1e1, &ind);
							}
							if ((rx_message.ExtId >> 8) == CAN_PACKET_STATUS_4)
							{
									ind += 6;
									VESCmotor[id].valueReal.angle = buffer_16_to_float(rx_message.Data, 5e1, &ind);
							}
					}
					else
					{
							if ((rx_message.ExtId >> 8) == CAN_PACKET_STATUS)
							{
									// 这里的速度有问题
									VESCmotor[id].valueReal.speed = buffer_32_to_float(rx_message.Data, 1e0, &ind) / VESCmotor[id].Param.Motor_PolePairs; // 放在第0 1 2 3个字节
									VESCmotor[id].valueReal.current = buffer_16_to_float(rx_message.Data, 1e1, &ind);                                     // 第4 5字节  得到的电流除以10
									VESCmotor[id].valueReal.angle = buffer_16_to_float(rx_message.Data, 1e1, &ind);                                       // 第6 7字节			得到的角度除以10		只供阅读，为0~360间的值

									ChangeData(&rx_message.Data[6], &rx_message.Data[7]);
									DecodeU16Data(&VESCmotor[id].Argum.angleNow, &rx_message.Data[6]);
									VESCmotor[id].Argum.distance = VESCmotor[id].Argum.angleNow - VESCmotor[id].Argum.anglePrv;
									VESCmotor[id].Argum.anglePrv = VESCmotor[id].Argum.angleNow;
									if (ABS(VESCmotor[id].Argum.distance) > 1800)
									{
											VESCmotor[id].Argum.distance -= 3600;
									}
									VESCmotor[id].Argum.position += VESCmotor[id].Argum.distance;
									VESCmotor[id].valueReal.Angle_ABS = VESCmotor[id].Argum.position / 10.0;
							}
							
					}
					VESCmotor[id].Argum.lastRxTim = 0;
			}
	}


/**
 * @brief 设定VESC角度
 *
 * @param controller_ID（ID号）
 * @param pos（角度）
 * @param InConGrpFlag
 */
void VESC_Set_Pos(u8 controller_ID, float pos, u8 InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else // 入队
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(pos * 1e6f), &send_index);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_POS << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    VESC_SendQueue.Rear = VESCRear; // 循环队尾加一 利用取余
}
/**
 * @brief 设置VESC占空比
 *
 */
void VESC_Set_Duty_Cycle(uint8_t controller_ID, float duty_cycle, uint8_t InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(duty_cycle * 100000), &send_index);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_DUTY << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }

    VESC_SendQueue.Rear = VESCRear;
}

/**
 * @brief 设定VESC转速
 *
 */
void VESC_Set_Speed(uint8_t controller_ID, float speed, uint8_t InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, speed, &send_index);
        // send_index = 0.即将格式为int32的speed放到数组buffer中，number的高位对应buffer的低字节
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_RPM << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    VESC_SendQueue.Rear = VESCRear; // Rear=Rear+1
}

/**
 * @brief 设定VESC电流
 *
 */
void VESC_Set_Current(uint8_t controller_ID, float current, uint8_t InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(current * 1000), &send_index);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    VESC_SendQueue.Rear = VESCRear;
}

/**
 * @brief 设定VESC锁定电机电流
 *
 */
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(brake_current * 1000), &send_index);
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    VESC_SendQueue.Rear = VESCRear;
}

/**
 * @brief 设定VESC锁电机电流
 */
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag)
{
    int32_t send_index = 0;
    if (VESCRear == VESC_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else // 入队
    {
				VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].IDE= CAN_ID_EXT;
        buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(handbrake_current * 1000), &send_index);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8);
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
        VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    VESC_SendQueue.Rear = VESCRear; // 队尾加一
}

// 本杰明断连检测
void VESC_IfTimeout(uint8_t id)
{
    if (VESCmotor[id].enable && (VESCmotor[id].Argum.lastRxTim++) > 10)
    {
        VESCmotor[id].Argum.timeoutTicks++;
        if (VESCmotor[id].Argum.timeoutTicks > 50)
        {
            VESCmotor[id].Argum.timeoutTicks = 0;
						VESC_Feedback_TimeoutError(id+1, 0); // 反馈主控断连
            VESCmotor[id].Status.timeout = true;
            LedTask.Timeout = true;
            Led8DisData(id + 1);
            B1p.error = true;
        }
        else
        {
            VESCmotor[id].Status.timeout = false;
        }
    }
    else if (VESCmotor[id].enable && VESCmotor[id].Status.timeout)
    {
        VESCmotor[id].Status.timeout = false;
        B1p.error = false;
    }
    else
        VESCmotor[id].Argum.timeoutTicks = 0;
}

//本杰明堵转检测
void VESC_IfStuck(uint8_t id)
{
    if ((VESCmotor[id].valueReal.current > 48) && (VESCmotor[id].enable)) // 电机处于使能状态且电流非常大
    {
        VESCmotor[id].Argum.stuckcnt++;
        if (VESCmotor[id].Argum.stuckcnt > 500) // 500 * 20ms = 1s
        {
            VESCmotor[id].Status.stuck = true;
            LedTask.Stuck = true;
            B1p.error = true;
            VESC_Feedback_StuckError(id+1, 0);
            Led8DisData(id + 1);
            if (VESCmotor[id].limit.IfReleaseWhenStuck)//如果堵转时释放
                VESCmotor[id].enable = false;
        }
        else
        {
            VESCmotor[id].Status.stuck = false;
        }
    }
    else if ((VESCmotor[id].enable) && (VESCmotor[id].Status.stuck == true))
    {
        VESCmotor[id].Status.stuck = false;
        B1p.error = false;
    }
    else
        VESCmotor[id].Argum.stuckcnt = 0;
}

//本杰明反馈堵转
void VESC_Feedback_StuckError(uint8_t id, uint8_t InConGrpFlag)
{
    if (IncCAN1Rear == CAN1_SendQueue.Front)
    {
        flag.Can1SendqueueFULL++;
        return;
    }
    else
    {
				CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x020201EE;//本杰明反馈堵转;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0x02;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = id;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 2;

        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    CAN1_SendQueue.Rear = IncCAN1Rear;
}

//本杰明反馈断连
void VESC_Feedback_TimeoutError(uint8_t id, uint8_t InConGrpFlag)
{
    if (IncCAN1Rear == CAN1_SendQueue.Front)
    {
        flag.Can1SendqueueFULL++;
        return;
    }
    else
    {
				CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x020201EE;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0x02;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = id;
        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 1;

        CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = InConGrpFlag;
    }
    CAN1_SendQueue.Rear = IncCAN1Rear;
}

void VESCFunc(void)
{
    for (int i = 0; i < USEVESC; i++)
    {
        if (VESCmotor[i].enable)
        {
            if (VESCmotor[i].limit.Stuck_IfDect)
                VESC_IfStuck(i);
            if (VESCmotor[i].limit.Timeout_IfDect)
                VESC_IfTimeout(i);
            if (VESCmotor[i].begin)
            {
                switch (VESCmotor[i].mode)
                {
                case VESCCURRENT:
                    VESC_Set_Current(i + 1, VESCmotor[i].valueSet.current, 0);
                    break;

                case VESCRPM:
                    VESC_Set_Speed(i + 1, VESCmotor[i].valueSet.speed * VESCmotor[i].Param.Motor_PolePairs, 0);
                    break;

                case VESCPOSITION:
                    VESC_Set_Pos(i + 1, VESCmotor[i].valueSet.angle, 0);
                    break;

                case VESCDUTY:
                    VESC_Set_Duty_Cycle(i + 1, VESCmotor[i].valueSet.duty, 0);
                    break;

                case VESCHANDBREAK:
                    VESC_Set_Handbrake_Current(i + 1, VESCmotor[i].valueSet.HandBreak_Current, 0);
                    break;

                default:
                    break;
                }
            }
            else
                VESC_Set_Pos(i + 1, VESCmotor[i].valueSet.angle, 0);
        }
//        else
//            VESC_Set_Current(i + 1, 0.0, 0);
    }
}



