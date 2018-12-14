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

//��������ڶ���Ϳ�������ͷ
//��openGL�Ļ�����ʹ�����ͳһ�Ľӿڶ�������ͷ��λ��
//�Զ�����ͨ��ֱ�۵�λ����Ϣ�������ֱ�۵�openGL�������
//���ռ���ʵ������ͷ�Ĺ��ܡ�
//���ߣ�yinan Miao from Beihang Univ.
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645

class camera
{

private:
    float x,y,z;//����ͷλ��
    float minx,maxx,miny,maxy;//λ���޶�
    float ang;//����ͷ����ǣ�����
    float attAng;//����ͷ���ǣ�����
    float scaleK;//���űȣ�������
    float rollAng;//����ͷ��ת��
    int angVertexFlag;//���Ϊ0�������ýǶ�ģʽ�����Ϊ1��������ģʽ��
    vertex at;//ָ������
    vertex up;//��������

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
    void init(float xi,float yi,float zi,float angi,float attAngi);//��ʼ�����趨����ͷλ�ã��Ƕȣ�����
    void setRange(float minX,float maxX,float minY,float maxY);//��������ͷ���ƶ���Χ
    void move(char in,float value);//�ƶ�����ͷ���趨�ƶ�ģʽ��ֵ
    void spin(char in,float value);//ת������ͷ���趨ת��ģʽ��ֵ
    void scale(float value);//��������ͷ
    void play(float li,float angi);//������������
    void setPos(float xi,float yi,float zi);//����λ��
    void setAng(float atti,float angi,float rolli);//���ýǶȣ���ʹģʽ��Ϊ�Ƕ�ģʽ��
    void setDirection(vertex ati,vertex upi);//���÷�����ʽ�ĽǶȣ���ʹģʽ��Ϊ����ģʽ��
};

#endif // CAMERA_H
