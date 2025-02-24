# CubeMX_based_performance_test_and_us_delay

*This Lib is appropriate for projects from CubeMX. Your project should be create at the beginning by CubeMX and it can make sure that the SysTick is configured in a correct way.*

*Contact author: 2078801653@qq.com or jza15767@gmail.com*

## This Lib is devided into several parts as follows:
### **useful variables**  
**User can use these macros variables to designed their "get_time()" function in order to display some performance information in an appropriate frequency.
```
#define DURATION 	pass_time_us						//代码段的执行时间测试结果  Result of test
#define RUN_TIME 	_CONNECT2(_static_, __LINE__)				//测试次数 Total Tests Conducted
```
### **User Controled Macros**
**To customize the actions for different numbers of get_time() arguments, uncomment and define them. get_time() will call the corresponding function each time the test completes**  
**Remember to use #undef before define them outside the .h file if required**
```
//#define USER_FUNC_WITH_0ARG() 						/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_1ARG(ARG_NAME1)  					/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_2ARG(ARG_NAME1, ARG_NAME2)				/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_3ARG(_1, _2, _3) 					/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_4ARG(_1, _2, _3, _4)  					/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_5ARG(_1, _2, _3, _4, _5)  				/*...USER FUNCTIONS*/
//#define USER_FUNC_WITH_6ARG(_1, _2, _3, _4, _5, _6)  				/*...USER FUNCTIONS*/

#define GT_ERROR_HANDLER ;

//是否启用get_time() ##调试时可以开启，非调试时设为0即可，无需删除get_time()代码
//whether to enable get_time() function
#ifndef _ENABLE_GET_TIME_
	#define _ENABLE_GET_TIME_ 1
#endif

//HAL库的SystemCoreClock在预处理时不确定，故需手动指定值。_FREQ_MHz_即STM32时钟频率（单位为MHz）
//modify the Frequency of MCU in MHz
#ifndef _FREQ_MHz_
	#define _FREQ_MHz_  80
#endif
```
### **Core Functions**
**The core implementation**

### Meanwhile, you can use delay_us(xus) function to delay in a blocking mode.  
```
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
```
