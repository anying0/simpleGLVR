#include "camera.h"


camera::camera()
{
    //默认初始化变量
    x = 0;
    y = 0;
    z = 0;
    ang = 0;
    attAng = AL(45);
    scaleK = 1;
}

void camera::init(float xi, float yi, float zi, float angi, float attAngi)//XYZ坐标，横向角度，纵向角度
{
    //按照输入设置摄像机参数
    x = xi;
    y = yi;
    z = zi;
    ang = angi;
    attAng = attAngi;
    rollAng = 0;//滚转值默认是0
    scaleK = 1;//缩放值是1
    at = VERTEX(1,0,0);
    up = VERTEX(0,0,1);
    angVertexFlag = 0;
}

void camera::setRange(float minX, float maxX, float minY, float maxY)
{
    //设置摄像机的移动范围
    minx = minX;
    miny = minY;
    maxx = maxX;
    maxy = maxY;
}

void camera::move(char in, float value)
{
    //mAng是移动的角度，就是摄像机的角度加上上下左右的角度
    float mAng;
    if(in=='w')
        mAng = ang;
    else if(in == 's')
        mAng = ang + PI5;
    else if(in == 'a')
        mAng = ang + PI5/2;
    else if(in == 'd')
        mAng = ang - PI5/2;
    else
        value = 0;//如果输入不合法，摄像机就不动了

    //改变x和y，移动摄像机
    x += value*cos(mAng);
    y += value*sin(mAng);

    //根据位置极限跳调整摄像机位置，防止摄像机移动出区域
    if(x<minx)x=minx;
    if(y<miny)y=miny;
    if(x>maxx)x=maxx;
    if(y>maxy)y=maxy;
}

void camera::spin(char in, float value)//旋转摄像机
{
    if(in == CAMERA_ANG)//如果改变的是横向角度，就直接改变
    {
        ang += value;
    }
    else if(in == CAMERA_ATT)//如果改变的是摄像机攻角，则需要限制攻角范围，在0到60度之间。
    {
        attAng += value;
        if(attAng > PI5)attAng  -= 2*PI5;
        if(attAng < -PI5)attAng += 2*PI5;
        //if(attAng > PI5/3)attAng = PI5/3;
        //if(attAng < 0)attAng = 0;
    }
    else if(in == CAMERA_ROL)
    {
        rollAng += value;
        if(rollAng > PI5)rollAng -= 2*PI5;
        if(rollAng < - PI5)rollAng += 2*PI5;
    }
    angVertexFlag = 0;
}

void camera::scale(float value)//直接缩放
{
    scaleK *= value;
}
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

void camera::play(float li, float angi)//绘制摄像机相关
{
    glMatrixMode(GL_PROJECTION);//设置为摄像机矩阵模式
    glLoadIdentity();//恢复为单位矩阵
    //这里相当于L是瞳孔（快门）尺寸，使用scale调节以改变缩放比例
    float l = li;//瞳孔尺寸
    float d = (l*0.5)/tan(angi*0.5);//根据角度设置摄像机焦距
    if(d<0)return ;//不合理即退出
    glFrustum(-l/scaleK,l/scaleK,-l/scaleK,l/scaleK,d,15000);//设置摄像机投影参数，使其拥有立体效果
    //以瞳孔（快门）位置为基准旋转
    if(angVertexFlag == 0)
    {
        vertex up;//头顶方向向量
        vertexTransform trans;//声明一个基底变换
        trans.init();//初始化
        trans.rotate(VERTEX(0,0,1),-ang);//延Z轴转动
        trans.rotate(VERTEX(0,1,0),-attAng);//延Y轴转动
        trans.rotate(VERTEX(1,0,0),rollAng);//延X轴转动
        up = trans.get(VERTEX(0,0,1));//得到当前基底下的(0,0,1)，即指向头顶的向量

        vertex pos,to;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        to = trans.get(VERTEX(-d,0,0));//得到当前基底下的朝向向量的反向
        pos = MGLTools::add(pos,to);//与位置相加得到摄像机焦点的位置
        gluLookAt(pos.x,pos.y,pos.z,x,y,z,up.x,up.y,up.z);//调整摄像机角度


    }
    else
    {
        vertex pos,to;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        to = MGLTools::mult(at,-1*d);
        pos = MGLTools::add(pos,to);//与位置相加得到摄像机焦点的位置
        gluLookAt(pos.x,pos.y,pos.z,x,y,z,up.x,up.y,up.z);//调整摄像机角度
    }
    glMatrixMode(GL_MODELVIEW);//调整为模型绘制模式
    glLoadIdentity();//载入单位矩阵
}

void camera::setPos(float xi, float yi, float zi)//直接设置位置XYZ
{
    x = xi;
    y = yi;
    z = zi;
}

void camera::setAng(float atti, float angi, float rolli)//直接设置角度横向角度、纵向角度、滚转角度
{
    attAng = atti;
    ang = angi;
    rollAng = rolli;
    angVertexFlag = 0;
}

void camera::setDirection(vertex ati, vertex upi)
{
    angVertexFlag = 1;
    up = upi;
    at = ati;
}
