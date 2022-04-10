//任务10		制作用数码管显示的秒表(数码管动态扫描显示)
#include	<reg51.h>				//1 将特殊功能寄存器定义文件reg51.h包含进来
#define	DCOUNT	8					//2 宏定义：DCOUNT代表常数2(数码管的个数)
#define	portled_S		P0			//3 宏定义：portled_S代表P1(数码管的段选口)
#define	portled_B		P2			//4 宏定义：portled_B代表P2(数码管的位选口)
#define	uchar	unsigned	char	//5 宏定义：uchar代表unsigned char
uchar	data	wcnt,timcnt,second,minute,hour;	//6 在data区定义全局变量wcnt、timcnt、second
uchar	code	led[]={0x3f,0x06,0x5b,	//7 在code区建立数码管的笔型码表led
0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	//8 笔型码表续(C51的语句可以分多行书写)
uchar	code	ledctrl[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//9 在code区建立数码管的显示控制码表
void	display(uchar,uchar,uchar);				//10 display函数说明
void	main(void)					//11 main函数
{	second=0;						//12 软件初始化：秒计数器赋初值0
	timcnt=0;						//13 10ms中断次数计数器赋初值0
	wcnt=0;							//14 数码管显示位置计数器赋初值0
	TMOD=0x10;						//15 设置T1的运行模式(定时)、工作方式(方式1)
	TL1=(65536-9216)%256;			//16 T1的计数器赋初值：10ms
	TH1=(65536-9216)/256;			//17
	ET1=1;							//18 开T1中断
	EA=1;							//19 开全局中断
	TR1=1;							//20 启动T1
	while(1)						//21 死循环，语句22为循环体
	{
  display(second,minute,hour);				//33 调用display函数显示秒计数值		
	
	}			//22 睡眠CPU
}									//23 main函数结束
//--------定时中断T1服务程序---------
void	tim1()	interrupt	3	using	2	//24 T1中断服务函数，中断号为3
{	TL1=(65536-9216)%256;			//25 重置计数初值:10ms
	TH1=(65536-9216)/256;			//26
	timcnt++;						//27 10ms中断次数计数值加1
	if(timcnt>=100)					//28 计满1秒吗？
	{	timcnt=0;					//29 满1秒，10ms中断次数计数值回0
		second++;					//30 秒计数加1
		if(second>=60)	
		{
			second=0;	//31 若计满1分，则秒计数回0
			minute++;
			if(minute>=60)
			{
				minute=0;
				hour++;
				if(hour>=24)
					hour=0;
			}
		}
	}								//32 计满1秒处理结束
	
}									//34 中断服务函数结束
//-------------显示子程序------------
void	display(uchar	second,uchar minute,uchar hour)		//35 display函数
{	portled_S=0;					//36段选口消隐输出
	portled_B=ledctrl[wcnt];		//37查表获点亮当前数码管的控制码
	switch(wcnt)					//38查询当前点亮数码管的编号
	{case	0:						//39是0号管
		portled_S=led[second%10];		//40段选口输出秒的个位笔型码(显示秒个位)
		break;						//41结束switch语句
	case	1:						//42是1号管
		portled_S=led[second/10];		//43段选口输出秒的十位笔型码(显示秒十位)
	  break;
	case  2:
		portled_S=0x40;
	  break;
	case 3:
		portled_S=led[minute%10];		
	  break;
	case 4:
		portled_S=led[minute/10];	
    break;	
	case  5:
		portled_S=0x40;
	  break;
	case  6:
		portled_S=led[hour%10];	
	  break;
	case  7:
		portled_S=led[hour/10];	
	  break;
	  
	}								//44 switch语句结束
	wcnt++;							//45 位置计数值加1
	wcnt=wcnt%DCOUNT;				//46 超界处理
}									//47 中断服务函数结束
//--------------------------------------------------------
//显示函数放在主函数中,减少中断所需时间


////计数器达到模值时计数值归0  标志位置1 挂入中断标志位置0  计数赋初值继续计数