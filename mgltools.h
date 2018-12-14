#ifndef GLTOOLS_H
#define GLTOOLS_H
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <gl/GL.h>
#include <GL/glut.h>
#include <gl/GLU.h>
#include <vector>
#include <math.h>
using namespace std;

//�������������OPENGL���ֹ��ܣ�ʹ������Ӽ�
//�ṹ�������˹�Դ�����ʡ���ͼ��ع���
//�����˵㣨������������ؼ���ĺ���
//�Գ������������˺궨��
//���ߣ�yinan Miao from Beihang Univ.
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645

//����AL(X)Ϊ�ӽǶ�ת��λ���ȵĺ�
//����VERTEX(X,Y,Z)Ϊ���ٴ����㣨�������ĺ�
//����FLATTENV(X)Ϊչ��x,y,z��д�����ĺ�
//����RA(X)��Ϊ���ƽǶ���-Pi��+Pi֮��ĺꡣ����ı�Ƕ�ֵ��ֻ�ǰ���2Pi������
#define AL(X) 3.14*X/180.0
#define WIDTH 1200
#define HEIGHT 400
#define LENGTH 1200
#define VERTEX(X,Y,Z) MGLTools::newV(X,Y,Z)
#define FLATTENV(A) A.x,A.y,A.z
#define RA(X) MGLTools::RA(X)


typedef struct vetexStruct{
    float x, y, z;
} vertex;//�㣨������

typedef struct modelStruct{
    vector<vertex> facet;//������
    vector<vertex> p1, p2, p3;//�������γ�һ����
} model;//.stl��ʽģ�ʹ��棬�������漰�䷨����

typedef struct ptstrct{
    float x,y;
} pt;//��


typedef struct picStruct{
    GLint imagewidth;
    GLint imageheight;
    GLint pixellength;
    GLubyte* pixeldata;
}pic;//���ڴ���bmp��ʽλͼ������ֻ�ܴ���24λ��λͼ


typedef struct materialStruct{
    GLfloat ambient[4];//�����ⷴ��
    GLfloat diffuse[4];//������
    GLfloat specular[4];//���淴��
    GLfloat shininess;//����
} materialStruct;//���ʽṹ


typedef struct lightStruct{
    GLfloat ambient[4];//�����ⷴ��
    GLfloat diffuse[4];//������
    GLfloat specular[4];//���淴��
    GLfloat positon[4];//λ��
} lightStruct;//��Դ�ṹ




class MGLTools
{
public:
    MGLTools();
    static vertex newV(float x,float y,float z);//�½�������һ���½�������
    static vertex crossProduct(vertex a,vertex b);//����������ˣ�������һ��������
    static float dot(vertex a,vertex b);//�����ڻ�������ˡ�����һ��ֵ��
    static vertex norm(vertex a);//��һ�����������򲻱䣬���ȱ�Ϊ1.
    static vertex add(vertex a,vertex b);//���
    static vertex mult(vertex a,float b);//�˷�����������һ������
    static vertex rotate(vertex a,vertex b,float ang);//������ת��bΪת�ᣬangΪ��ת�Ƕ�

    static float angBtPoints(float x1,float y1,float x2,float y2);//���������Ƕ�
    static float disBtPoints(float x1,float y1,float x2,float y2);//�����������

    static float RA(float ang);//���ƽǶ���-أ��+أ֮�䡣

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


    static void initGL();//��ʼ��openGL�ĺ���
    static void setMaterials(materialStruct *m);//���ò���Ϊָ�����ʣ���Ҫʹ�ÿ��Գ���������ڴ�����
    static void enableTexture();//������ͼģʽ
    static GLuint loadTexture(pic &texture,GLuint *tex);//��һ��ͼƬ�󶨵���ͼ�����ҷ���һ����ͼID��
    static void disableTexture();//��ͼģʽ����
    static void setLight(lightStruct *l,int light);//���ù�Դ��*lΪ������lightΪ��Դ��š���Ҫʹ��glEnable��������Դ��������á�

    static void QUADQuick(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3,float x4,float y4,float z4);//���ٻ�һ������ͼ���ı���
    static void CylinderGLUTQuick(float x1,float y1,float z1,float x2,float y2,float z2,float r,int s,int l);//������֮������һ��Բ���塣

    static void readPic(const char fileName[],pic &unit);//�ú������ڶ�ȡ��ͼ���ڴ�֮��
    static void readStl(const char fileName[],model &mod);//�ú������ڶ�ȡ.stl��ʽ��ģ�͵��ڴ�֮��
    static void drawStl(model &mod,float scale);//����stlģ��
};

#endif // GLTOOLS_H
