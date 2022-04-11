#ifndef __ISoarLoopRunner_H_
#define __ISoarLoopRunner_H_
class ISoarLoopRunner
{
public:
	ISoarLoopRunner(void){}
	virtual ~ISoarLoopRunner(void){}
	virtual bool RunLoop(float t)=0;
};
#endif