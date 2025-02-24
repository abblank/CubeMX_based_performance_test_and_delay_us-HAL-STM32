/****************************************************************************************
 * MIT License
 * 
 * Copyright (c) 2025 Ziang Jiang
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ****************************************************************************************
 * This Lib is appropriate for projects from CubeMX
 * Attention: To use this Lib, you should chose GNU extension first in Keil
 * Contact author: 2078801653@qq.com
 ****************************************************************************************
 */
 
#ifndef _TIME_H_
#define _TIME_H_

#pragma diag_suppress 550
#pragma diag_suppress 167
#pragma diag_suppress 47
#pragma diag_suppress 1

#include "main.h"

/*useful variables*/
#define DURATION 	pass_time_us														//代码段的执行时间测试结果
#define RUN_TIME 	_CONNECT2(_static_, __LINE__)						//测试次数

/**************************************************************************************/
/*User Controled Macros*/
//To customize the actions for different number of get_time() arguments, uncomment and define them
//Remember to use #undef before define them outside the .h file if required

//#define USER_FUNC_WITH_0ARG() 													/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_1ARG(ARG_NAME1)  								/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_2ARG(ARG_NAME1, ARG_NAME2)				/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_3ARG(_1, _2, _3) 								/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_4ARG(_1, _2, _3, _4)  						/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_5ARG(_1, _2, _3, _4, _5)  				/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_6ARG(_1, _2, _3, _4, _5, _6)  		/*...USER FUNCTIONS*/

#define GT_ERROR_HANDLER ;

//是否启用get_time() ##调试时可以开启，非调试时设为0即可，无需删除get_time()代码
#ifndef _ENABLE_GET_TIME_
	#define _ENABLE_GET_TIME_ 1
#endif

//HAL库的SystemCoreClock在预处理时不确定，故需手动指定值。_FREQ_MHz_即STM32时钟频率（单位为MHz）
#ifndef _FREQ_MHz_
	#define _FREQ_MHz_  80
#endif
/**************************************************************************************/
/*Core Functions*/
#ifndef USER_FUNC_WITH_0ARG
	#define USER_FUNC_WITH_0ARG()  printf("GT: %d us      ", DURATION);
#endif
#ifndef USER_FUNC_WITH_1ARG
	#define USER_FUNC_WITH_1ARG(_TIMES)\
	if (RUN_TIME >= _TIMES) {\
		printf("GT: %d us      ", DURATION);\
		RUN_TIME = 0;\
	}
#endif
#ifndef USER_FUNC_WITH_2ARG
	#define USER_FUNC_WITH_2ARG(_TIMES, _STR)\
	if (RUN_TIME >= _TIMES) {\
		printf("%s: %d us      ", _STR, DURATION);\
		RUN_TIME = 0;\
	}
#endif
#ifndef USER_FUNC_WITH_3ARG
	#ifndef lcd_printf
	#define lcd_printf(LINE, FORMAT, ...)\
	do{\
		char lcd_buff[21];\
		snprintf(lcd_buff, 20, FORMAT, ##__VA_ARGS__);\
		LCD_DisplayStringLine(LINE, (uint8_t*)lcd_buff);\
	}while(0)
	#endif
	#define USER_FUNC_WITH_3ARG(_TIMES, _STR, _LINE)\
	if (++_CONNECT2(_static_, __LINE__) >= _TIMES) {\
		lcd_printf(_LINE, "%s: %d us      ", _STR, DURATION);\
		_CONNECT2(_static_, __LINE__) = 0;\
	}
#endif
#ifndef USER_FUNC_WITH_4ARG
	#define USER_FUNC_WITH_4ARG(_1, _2, _3, _4)  ;
#endif
#ifndef USER_FUNC_WITH_5ARG
	#define USER_FUNC_WITH_5ARG(_1, _2, _3, _4, _5)  ;
#endif
#ifndef USER_FUNC_WITH_6ARG
	#define USER_FUNC_WITH_6ARG(_1, _2, _3, _4, _5, _6)  ;
#endif

#define __GET_ARG_NUM_ANALYSIS(_0,_1,_2,_3,_4,_5,_6,_N,...)  _N
#define _GET_ARG_NUM(...)  __GET_ARG_NUM_ANALYSIS(-1,##__VA_ARGS__,6,5,4,3,2,1,0)

#define __CONNECT2_MID(_str1, _str2) _str1##_str2	
#define _CONNECT2(_str1, _str2) __CONNECT2_MID(_str1, _str2)

#define _SYSTICK_RELOAD_REG_  _CONNECT2(_FREQ_MHz_, 000)

#define _process(_on_enter_exp, _on_leave_exp, init_exp...)\
for (uint32_t init_exp, _CONNECT2(_process_, __LINE__) = 1;\
 (_on_enter_exp), _CONNECT2(_process_, __LINE__) == 1;\
 (_on_leave_exp), --_CONNECT2(_process_, __LINE__))

#define _leave_0()    USER_FUNC_WITH_0ARG()
#define _leave_1(_1)    USER_FUNC_WITH_1ARG(_1)
#define _leave_2(_1, _2)    USER_FUNC_WITH_2ARG(_1, _2)
#define _leave_3(_1, _2, _3)    USER_FUNC_WITH_3ARG(_1, _2, _3)
#define _leave_4(_1, _2, _3, _4)    USER_FUNC_WITH_4ARG(_1, _2, _3, _4)
#define _leave_5(_1, _2, _3, _4, _5)    USER_FUNC_WITH_5ARG(_1, _2, _3, _4, _5)
#define _leave_6(_1, _2, _3, _4, _5, _6)    USER_FUNC_WITH_6ARG(_1, _2, _3, _4, _5, _6)

#if (_ENABLE_GET_TIME_ == 1)
#define get_time(...)\
if(_GET_ARG_NUM(__VA_ARGS__) > 6) {GT_ERROR_HANDLER; while(1);}\
static uint16_t _CONNECT2(_static_, __LINE__);\
_process(\
	{\
		pri_time_ms = HAL_GetTick(); pri_time_reg = SysTick->VAL;\
	},\
	{\
		last_time_reg = SysTick->VAL;\
		pass_time_us = ((last_time_reg>pri_time_reg?\
		(HAL_GetTick()-pri_time_ms-1)*1000:\
		(HAL_GetTick()-pri_time_ms)*1000)+\
		(((_SYSTICK_RELOAD_REG_+pri_time_reg-last_time_reg)%\
			_SYSTICK_RELOAD_REG_)/_FREQ_MHz_));\
		++_CONNECT2(_static_, __LINE__);\
		_CONNECT2(_leave_, _GET_ARG_NUM(__VA_ARGS__))(__VA_ARGS__)\
	},\
		last_time_reg, pri_time_ms, pri_time_reg, pass_time_us\
)
#else
	#define get_time(...)  ;
#endif
	
/**
\brief      微秒级延时函数	delay_us
	\arg				xus:延时的时间us
\attention	稳定状态（不被中断打断）误差小于1%
\return			None
*/
#ifndef delay_us
	#define delay_us(xus)\
	do{\
		int start, target, last, val, cur_delay, rest = xus;\
		while(rest > 0){\
			cur_delay = rest>800? 800: rest;\
			start = SysTick->VAL;\
			target = cur_delay*_FREQ_MHz_;\
			if (start >= target) {\
				last = start - target;\
				do{\
					val = SysTick->VAL;\
				}while((val > last) && (val <= start));\
			} else {\
				last = _SYSTICK_RELOAD_REG_ + start - target;\
				do{\
					val = SysTick->VAL;\
				}while((val > last) || (val <= start));\
			}\
			rest -= cur_delay;\
		}\
	}while(0)
#endif	

#endif
	
