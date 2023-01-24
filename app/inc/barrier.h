
#ifndef __BARRIER_H__
#define __BARRIER_H__


typedef enum
{
  eBarrierStatusOpened,
	eBarrierStatusOpening,
	eBarrierStatusClosed,
	eBarrierStatusClosing
}eBarrierStatus;

typedef enum
{
  eBarrierCmdOpen,
	eBarrierCmdClose,
}eBarrierCmd;



void BarrierInit(eBarrierStatus status);

void BarrierSet(eBarrierCmd cmd);








#endif /* __BARRIER_H__ */
