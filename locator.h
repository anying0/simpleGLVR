#ifndef LOCATOR_H
#define LOCATOR_H
#include <recongnizer.h>
#include <thread>
#include <mutex>

//VR眼镜识别的位姿识别类
//可以异步高速读取图像对绿色的目标进行识别
//并且计算目标的位姿信息
//作者：yinan Miao from Beihang Univ.
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）
//QQ:1131042645

class locator
{
private:
    recongnizer *rec;//识别器
    CvCapture *capture;//数据流指针
    thread *capThread;//图像读取线程指针
    IplImage *pSrcImg;//中转图像所在指针
    mutex *mut;//线程锁
    IplImage *getPic;//最终获取到的图像指针
    int *endFlag;//线程结束标志变量指针

    /*
    Copyright (c) 2018, yinan Miao;
    All rights reserved.
    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the name of the Beihang Univ. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    QQ:1131042645
    */

public:
    static void captureThread(CvCapture *capture,IplImage *pSrcImg,mutex *mut,int *end);//线程执行函数，异步传输视频流，传入capture,以及图像、线程锁以及停止标志的内存地址。
    locator();//构造函数

    void init();//初始化函数
    void run(float *ans,int &ansType);//运行一次，可以得到一次当前摄像头信息中图像并且识别位置信息，返回位置值以及返回类型
    //**  pos为位姿输出的数组指针，长度至少为6。                                                **
    //**  pos[0]为输出的水平角位置，代表以摄像头方向为正方向时，目标的水平位置偏角。向右偏为正。**
    //**  pos[1]为输出的竖直角位置，代表以伸向头方向为正方向时，目标的数值位置偏角。向上偏为正。**
    //**  pos[2]为输出的目标距离，代表摄像头与识别目标的距离。                                  **
    //**  pos[3]为目标的俯仰角，代表以目标正面朝向摄像头为正方向时，目标的俯仰角。上仰为正。    **
    //**  pos[4]为目标的侧滑角，代表以目标正面朝向摄像头为正方向时，目标的侧滑角。向右侧滑为正。**
    //**  pos[5]为目标的滚转角，代表以目标正面朝向摄像头为正方向时，目标的滚转角，顺时针为正。  **
    //**  一些解释：                                                                            **
    //**    水平角位置和竖直角位置，直接代表目标中心在摄像头中的位置（x,y），是根据目标的像素值 **
    //**  直接换算为角度值而来的。配合距离值，可以计算目标相对于自身的相对位置(x,y,z)。         **
    //**    俯仰角、侧滑角、滚转角反映了目标相对的角姿态。以目标正面朝向摄像头为正方向的意思 **
    //**  是设置无论目标在哪里时，只要目标完全正对着摄像头，并且没有滚转，即从摄像头中可以看到没**
    //**  有任何仿射变换的识别目标正面图像时，这时的俯仰角、侧滑角、滚转角皆为0。在此基础上，如 **
    //**  发生角姿态的变化，则识别为俯仰角、侧滑角、滚转角。配合目标的竖直角位置以及水平角位置，**
    //**  俯仰角、侧滑角、滚转角，可以换算为标准的相对俯仰角、相对侧滑角以及相对滚转角。        **
    //**	因此pos中的值完全反映了六自由度的姿态信息。                                         **
    void end();//关闭函数
};

#endif // LOCATOR_H
