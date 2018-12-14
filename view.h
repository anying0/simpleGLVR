#ifndef VIEW_H
#define VIEW_H
#include <mgltools.h>
#include <vertexTransform.h>


class view
{
private:

public:


    view();
    void init(float w,float l,float h,int div);//初始化场景，设置长宽高以及分割数量
    void draw(int time);//绘制场景，输入当前的时间
    void end();//结束场景，释放内存
    void getBasicCam(int time,float *ans);//获得指定时刻的摄像机标准位置.
    int getTimeLength();//获得过场景整体时长

};

#endif // VIEW_H
