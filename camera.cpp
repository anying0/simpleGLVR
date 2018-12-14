#include "camera.h"


camera::camera()
{
    //Ĭ�ϳ�ʼ������
    x = 0;
    y = 0;
    z = 0;
    ang = 0;
    attAng = AL(45);
    scaleK = 1;
}

void camera::init(float xi, float yi, float zi, float angi, float attAngi)//XYZ���꣬����Ƕȣ�����Ƕ�
{
    //���������������������
    x = xi;
    y = yi;
    z = zi;
    ang = angi;
    attAng = attAngi;
    rollAng = 0;//��תֵĬ����0
    scaleK = 1;//����ֵ��1
    at = VERTEX(1,0,0);
    up = VERTEX(0,0,1);
    angVertexFlag = 0;
}

void camera::setRange(float minX, float maxX, float minY, float maxY)
{
    //������������ƶ���Χ
    minx = minX;
    miny = minY;
    maxx = maxX;
    maxy = maxY;
}

void camera::move(char in, float value)
{
    //mAng���ƶ��ĽǶȣ�����������ĽǶȼ����������ҵĽǶ�
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
        value = 0;//������벻�Ϸ���������Ͳ�����

    //�ı�x��y���ƶ������
    x += value*cos(mAng);
    y += value*sin(mAng);

    //����λ�ü��������������λ�ã���ֹ������ƶ�������
    if(x<minx)x=minx;
    if(y<miny)y=miny;
    if(x>maxx)x=maxx;
    if(y>maxy)y=maxy;
}

void camera::spin(char in, float value)//��ת�����
{
    if(in == CAMERA_ANG)//����ı���Ǻ���Ƕȣ���ֱ�Ӹı�
    {
        ang += value;
    }
    else if(in == CAMERA_ATT)//����ı������������ǣ�����Ҫ���ƹ��Ƿ�Χ����0��60��֮�䡣
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

void camera::scale(float value)//ֱ������
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

void camera::play(float li, float angi)//������������
{
    glMatrixMode(GL_PROJECTION);//����Ϊ���������ģʽ
    glLoadIdentity();//�ָ�Ϊ��λ����
    //�����൱��L��ͫ�ף����ţ��ߴ磬ʹ��scale�����Ըı����ű���
    float l = li;//ͫ�׳ߴ�
    float d = (l*0.5)/tan(angi*0.5);//���ݽǶ��������������
    if(d<0)return ;//�������˳�
    glFrustum(-l/scaleK,l/scaleK,-l/scaleK,l/scaleK,d,15000);//���������ͶӰ������ʹ��ӵ������Ч��
    //��ͫ�ף����ţ�λ��Ϊ��׼��ת
    if(angVertexFlag == 0)
    {
        vertex up;//ͷ����������
        vertexTransform trans;//����һ�����ױ任
        trans.init();//��ʼ��
        trans.rotate(VERTEX(0,0,1),-ang);//��Z��ת��
        trans.rotate(VERTEX(0,1,0),-attAng);//��Y��ת��
        trans.rotate(VERTEX(1,0,0),rollAng);//��X��ת��
        up = trans.get(VERTEX(0,0,1));//�õ���ǰ�����µ�(0,0,1)����ָ��ͷ��������

        vertex pos,to;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        to = trans.get(VERTEX(-d,0,0));//�õ���ǰ�����µĳ��������ķ���
        pos = MGLTools::add(pos,to);//��λ����ӵõ�����������λ��
        gluLookAt(pos.x,pos.y,pos.z,x,y,z,up.x,up.y,up.z);//����������Ƕ�


    }
    else
    {
        vertex pos,to;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        to = MGLTools::mult(at,-1*d);
        pos = MGLTools::add(pos,to);//��λ����ӵõ�����������λ��
        gluLookAt(pos.x,pos.y,pos.z,x,y,z,up.x,up.y,up.z);//����������Ƕ�
    }
    glMatrixMode(GL_MODELVIEW);//����Ϊģ�ͻ���ģʽ
    glLoadIdentity();//���뵥λ����
}

void camera::setPos(float xi, float yi, float zi)//ֱ������λ��XYZ
{
    x = xi;
    y = yi;
    z = zi;
}

void camera::setAng(float atti, float angi, float rolli)//ֱ�����ýǶȺ���Ƕȡ�����Ƕȡ���ת�Ƕ�
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
