#pragma once

#include "spi.h"
#include "mathFunc.h"

u8 ReadAS5047pData(u16* rec);
/**
 * @brief 检查对as5047操作下，发送/反馈值是否合理
 * @param msg 接收到的值
 * @return u8 1为合理（偶数个1），0为不合理（奇数个1）
 */
u8	 AS5047Check(u16 msg);
