
#include "Queue.h"
#include <taskManager.h>

/********************************************************************************************************
*  @函数名   S_QueueEmpty						                                                           
*  @描述     清空一个队列								                                     
*  @参数     Head-队列头地址,  Tail-队列尾地址,   HBuff-队列缓存
*  @返回值   无   
*  @注意    无
********************************************************************************************************/
void S_QueueEmpty(unsigned char **Head, unsigned char **Tail, unsigned char *HBuff)
{
  *Head = HBuff;
  *Tail = HBuff;
}

/********************************************************************************************************
*  @函数名   S_QueueDataIn						                                                           
*  @描述     输入一个字节数据进队列								                                     
*  @参数     Head-队列头地址,  Tail-队列尾地址,   HBuff-队列缓存
*  @返回值   无   
*  @注意     无
********************************************************************************************************/
void S_QueueDataIn(unsigned char **Head, unsigned char **Tail, unsigned char *HBuff, unsigned short Len, unsigned char *HData, unsigned short DataLen)
{	
  unsigned short num;
  unsigned char IptStatus;

//  IptStatus = __get_interrupt_state();

//  disableInterrupts();
  __atomic_enter()

  for(num = 0; num < DataLen; num++, HData++)
  {
    **Tail = *HData;
    (*Tail)++;
    if(*Tail == HBuff+Len)
            *Tail = HBuff;
    if(*Tail == *Head)
    {
      if(++(*Head) == HBuff+Len)
         *Head = HBuff;		
    }
  }	
  __atomic_exit()
//  __set_interrupt_state(IptStatus);
}

/********************************************************************************************************
*  @函数名   S_QueueDataOut						                                                           
*  @描述     从队列里取出一个数据								                                     
*  @参数     Head-队列头地址,  Tail-队列尾地址,   HBuff-队列缓存
*  @返回值   取出的数据   
*  @注意     无
********************************************************************************************************/
unsigned char S_QueueDataOut(unsigned char **Head, unsigned char **Tail, unsigned char *HBuff, unsigned short Len, unsigned char *Data)
{					   
  unsigned char back = 0;
  unsigned char IptStatus;
//  IptStatus = __get_interrupt_state();

//  disableInterrupts();
  __atomic_enter()

  *Data = 0;
  if(*Tail != *Head)
  {
    *Data = **Head;
    back = 1; 				
    if(++(*Head) == HBuff+Len)
       *Head = HBuff;
  }
//  __set_interrupt_state(IptStatus);
  __atomic_exit()

  return back;	
}

/********************************************************************************************************
*  @函数名   S_QueueDataLen						                                                           
*  @描述     判断队列里数据的长度							                                     
*  @参数     Head-队列头地址,  Tail-队列尾地址,   HBuff-队列缓存
*  @返回值   队列里有数据个数
*  @注意     无
********************************************************************************************************/
unsigned short S_QueueDataLen(unsigned char **Head, unsigned char **Tail, unsigned short Len)
{
  if(*Tail > *Head)
     return *Tail-*Head;
  if(*Tail < *Head)
     return *Tail+Len-*Head;
  return 0;
}

