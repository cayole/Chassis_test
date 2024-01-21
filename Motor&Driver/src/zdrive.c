#include "zdrive.h"


ZdriveTypedef Zdrive[8];
ZdriveParamTypedef zdriveParam;
ZdriveLimitTypedef zdriveLimit;

void Zdrive_Init(void)
{
	zdriveParam.GearRadio=1.f;
	zdriveLimit.IfReleaseWhenStuck=true;
	zdriveLimit.Stuck_IfDect=true;
	zdriveLimit.Timeout_IfDect=true;
	zdriveLimit.MAX_Current=18.f; // A
	zdriveLimit.MAX_Speed=30.f; // rad/s
	
	for(int i=0;i<8;i++)
	{
			Zdrive[i].errClearFlag=false;
			Zdrive[i].limit=zdriveLimit;
			Zdrive[i].Param=zdriveParam;
			Zdrive[i].enable=false;
			Zdrive[i].begin=false;
			Zdrive[i].mode=Zdrive_Disable;
			Zdrive[i].valueSet.speed=0;
			Zdrive[i].Status.err=Zdrive_Well;
	}
};

u32  GetZdriveCanID(u32 operationid , u32 id){ return ((id<<8) | operationid);};

void Zdrive_SetMode(float mode , u8 id)
{
	  if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				memcpy(Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].Data,&mode,sizeof(float));
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x55,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
}

void Zdrive_SetCurrent(float current,u8 id)
{
	  if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				memcpy(Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].Data,&current,sizeof(float));
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x37,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_SetSpeed(float speed,u8 id)
{
	
	  if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				speed /=60.f; // rpm => r/s
				memcpy(Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].Data,&speed,sizeof(float));
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x39,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_SetPosition(float position,u8 id)
{
	  if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				position /= 360.f;
				memcpy(Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].Data,&position,sizeof(float));
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x3B,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_ErrClear(u8 id){
		if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x57,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_AskCurrent(u8 id){
	  if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x44,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_AskSpeed(u8 id){
 if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x50,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_AskMode(u8 id){
 if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x54,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
};

void Zdrive_AskErr(u8 id){
		if (ZdriveRear == Zdrive_SendQueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].IDE =CAN_ID_STD;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].ID =  GetZdriveCanID(0x56,id);
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].DLC = 8;
				Zdrive_SendQueue.Can_DataSend[Zdrive_SendQueue.Rear].InConGrpFlag = false;
				Zdrive_SendQueue.Rear = ZdriveRear;
		}
}

void Zdrive_IfTimeOut(u8 id){
		Zdrive[id].Argum.lastRxTim++;
		if ((Zdrive[id].enable == true) && ((Zdrive[id].Argum.lastRxTim) > 30))
		{
        Zdrive[id].Argum.timeoutTicks++;
        if (Zdrive[id].Argum.timeoutTicks > 10) // ??30*10=300Ms???
        {
            Zdrive[id].Argum.timeoutTicks = 0;

            Zdrive[id].Status.timeout = true;
            LedTask.Timeout = true;
            // B1p.error=1;
            Led8DisData(id + 1);
        }
        else
        {
            Zdrive[id].Status.timeout = 0;
        }
    }

    else if ((Zdrive[id].enable == true) && (Zdrive[id].Status.timeout))
    {
        Zdrive[id].Status.timeout = 0;
        B1p.error = 0;
    }
    else
        Zdrive[id].Argum.lastRxTim = 0;
};

void ZdriveFunction(void)
{
    for (int i = 0; i < 8; i++)
    {
//				if(Zdrive[i].Argum.erraskcnt++>10) // 500ms					
//				{
//						Zdrive[i].Argum.erraskcnt=0;
//						Zdrive_AskErr(i+1);
//				}
        if (Zdrive[i].enable)
        {
						if(Zdrive[i].modeRead != Zdrive[i].mode)
						{
							Zdrive_SetMode(Zdrive[i].mode , i+1);
							Zdrive_AskMode(i+1);
						}
//            if (Zdrive[i].limit.Stuck_IfDect)
//                Zdrive_IfStuck(i);
            if (Zdrive[i].limit.Timeout_IfDect)
                Zdrive_IfTimeOut(i);
            if (Zdrive[i].begin)
            {
                switch (Zdrive[i].mode)
                {
                case Zdrive_Current:
                    Zdrive_SetCurrent(Zdrive[i].valueSet.current,i+1);
										Zdrive_AskCurrent(i+1);
                    break;

                case Zdrive_Speed:
                    Zdrive_SetSpeed(Zdrive[i].valueSet.speed* Zdrive[i].Param.GearRadio,i+1);
								    Zdrive_AskSpeed(i+1);
                    break;

                case Zdrive_Postion:
                    Zdrive_SetPosition(Zdrive[i].valueSet.angle, i+1);
                    break;
								
								case Zdrive_ClearErr:
									Zdrive_AskErr(i+1);
									if(Zdrive[i].Status.err == Zdrive_InsufficientVoltage || Zdrive[i].errClearFlag)
											Zdrive_ErrClear(i+1);
                default:
                    break;
                }
            }
            else
						{
               switch (Zdrive[i].mode)
                {
                case Zdrive_Current:
                    Zdrive_SetCurrent(0,i+1);
										Zdrive_AskCurrent(i+1);
                    break;

                case Zdrive_Speed:
                    Zdrive_SetSpeed(0,i+1);
										Zdrive_AskSpeed(i+1);
                    break;

                default:
                    break;
                }
						}
        }
        else
				{
						if(Zdrive[i].mode != Zdrive_Disable)
							Zdrive_SetMode(Zdrive_Disable, i+1);
				}
		}
};

void ZdriveRecHandler(CanRxMsg rx_msg)
{
	u8 controlid=((rx_msg.StdId & 0xf00)>> 8 );
	u32 operationid= (rx_msg.StdId & 0x0ff);
	
	if( controlid >8)
		return;

	switch(operationid)
	{
		case 0x35:
		{
			memcpy(&(Zdrive[controlid-1].valueReal.angle),&rx_msg.Data,sizeof(float));
			Zdrive[controlid-1].valueReal.angle*=360;
			break;
		}
		case 0x45:
		{
			memcpy(&(Zdrive[controlid-1].valueReal.current),&rx_msg.Data,sizeof(float));
			break;
		}
		case 0x51:
		{
			memcpy(&(Zdrive[controlid-1].valueReal.speed),&rx_msg.Data,sizeof(float));
			Zdrive[controlid-1].valueReal.speed *= 60;
			break;
		}
		case 0x55:
		{
			float tempmode;
			memcpy(&tempmode,&rx_msg.Data,sizeof(float));
			Zdrive[controlid-1].modeRead=(Zdrive_Mode)tempmode;
			break;
		}
		case 0x57:
		{
			float temperr;
			memcpy(&temperr,&rx_msg.Data,sizeof(float));
			Zdrive[controlid-1].Status.err=(Zdrive_Err)temperr;
			break;
		}
		default:
			return;
	}
	Zdrive[controlid-1].Argum.lastRxTim = 0;
};
