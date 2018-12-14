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

//这个类用于扩充OPENGL各种功能，使代码更加简单
//结构化定义了光源、材质、贴图相关功能
//定义了点（向量）及其相关计算的函数
//对常见操作进行了宏定义
//作者：yinan Miao from Beihang Univ.
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）
//QQ:1131042645

//定义AL(X)为从角度转换位弧度的宏
//定义VERTEX(X,Y,Z)为快速创建点（向量）的宏
//定义FLATTENV(X)为展开x,y,z填写函数的宏
//定义RA(X)，为限制角度在-Pi到+Pi之间的宏。不会改变角度值，只是按照2Pi调整。
#define AL(X) 3.14*X/180.0
#define WIDTH 1200
#define HEIGHT 400
#define LENGTH 1200
#define VERTEX(X,Y,Z) MGLTools::newV(X,Y,Z)
#define FLATTENV(A) A.x,A.y,A.z
#define RA(X) MGLTools::RA(X)


typedef struct vetexStruct{
    float x, y, z;
} vertex;//点（向量）

typedef struct modelStruct{
    vector<vertex> facet;//法向量
    vector<vertex> p1, p2, p3;//三个点形成一个面
} model;//.stl格式模型储存，三角形面及其法向量

typedef struct ptstrct{
    float x,y;
} pt;//点


typedef struct picStruct{
    GLint imagewidth;
    GLint imageheight;
    GLint pixellength;
    GLubyte* pixeldata;
}pic;//用于储存bmp格式位图，这里只能储存24位的位图


typedef struct materialStruct{
    GLfloat ambient[4];//环境光反射
    GLfloat diffuse[4];//漫反射
    GLfloat specular[4];//镜面反射
    GLfloat shininess;//明度
} materialStruct;//材质结构


typedef struct lightStruct{
    GLfloat ambient[4];//环境光反射
    GLfloat diffuse[4];//漫反射
    GLfloat specular[4];//镜面反射
    GLfloat positon[4];//位置
} lightStruct;//光源结构




class MGLTools
{
public:
    MGLTools();
    static vertex newV(float x,float y,float z);//新建并返回一个新建的向量
    static vertex crossProduct(vertex a,vertex b);//向量积（叉乘），返回一个向量。
    static float dot(vertex a,vertex b);//向量内积，即点乘。返回一个值。
    static vertex norm(vertex a);//归一化，向量方向不变，长度变为1.
    static vertex add(vertex a,vertex b);//求和
    static vertex mult(vertex a,float b);//乘法，向量乘以一个常数
    static vertex rotate(vertex a,vertex b,float ang);//向量旋转，b为转轴，ang为旋转角度

    static float angBtPoints(float x1,float y1,float x2,float y2);//计算两点间角度
    static float disBtPoints(float x1,float y1,float x2,float y2);//计算两点距离

    static float RA(float ang);//限制角度在-兀到+兀之间。

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


    static void initGL();//初始化openGL的函数
    static void setMaterials(materialStruct *m);//设置材质为指定材质，需要使用可以长期贮存的内存区域
    static void enableTexture();//开启贴图模式
    static GLuint loadTexture(pic &texture,GLuint *tex);//将一个图片绑定到贴图，并且返回一个贴图ID。
    static void disableTexture();//贴图模式结束
    static void setLight(lightStruct *l,int light);//设置光源。*l为参数，light为光源编号。需要使用glEnable来启动光源后才能设置。

    static void QUADQuick(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3,float x4,float y4,float z4);//快速画一个带贴图的四边形
    static void CylinderGLUTQuick(float x1,float y1,float z1,float x2,float y2,float z2,float r,int s,int l);//在两点之间生成一个圆柱体。

    static void readPic(const char fileName[],pic &unit);//该函数用于读取贴图到内存之中
    static void readStl(const char fileName[],model &mod);//该函数用于读取.stl格式的模型到内存之中
    static void drawStl(model &mod,float scale);//绘制stl模型
};

#endif // GLTOOLS_H
