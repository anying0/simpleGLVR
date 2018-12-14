#ifndef CAMERA_H
#define CAMERA_H
#include <Windows.h>
#include <gl/GL.h>
#include <GL/glut.h>
#include <gl/GLU.h>
#include "math.h"
#include "mgltools.h"
#include <vertexTransform.h>
#define PI5 3.14159
#define CAMERA_ANG 0
#define CAMERA_ATT 1
#define CAMERA_ROL 2

//这个类用于定义和控制摄像头
//在openGL的环境下使用相对统一的接口定义摄像头的位姿
//自动化地通过直观的位姿信息计算出不直观的openGL仿射参数
//最终简便地实现摄像头的功能。
//作者：yinan Miao from Beihang Univ.
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）
//QQ:1131042645

class camera
{

private:
    float x,y,z;//摄像头位置
    float minx,maxx,miny,maxy;//位置限定
    float ang;//摄像头朝向角（横向）
    float attAng;//摄像头攻角（纵向）
    float scaleK;//缩放比（滚动）
    float rollAng;//摄像头滚转角
    int angVertexFlag;//如果为0，则启用角度模式。如果为1启动向量模式。
    vertex at;//指向向量
    vertex up;//顶向向量

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
    camera();
    void init(float xi,float yi,float zi,float angi,float attAngi);//初始化，设定摄像头位置，角度，攻角
    void setRange(float minX,float maxX,float minY,float maxY);//设置摄像头的移动范围
    void move(char in,float value);//移动摄像头，设定移动模式和值
    void spin(char in,float value);//转动摄像头，设定转动模式和值
    void scale(float value);//缩放摄像头
    void play(float li,float angi);//绘制摄像机相关
    void setPos(float xi,float yi,float zi);//设置位置
    void setAng(float atti,float angi,float rolli);//设置角度，会使模式变为角度模式。
    void setDirection(vertex ati,vertex upi);//设置方向形式的角度，会使模式变为向量模式。
};

#endif // CAMERA_H
