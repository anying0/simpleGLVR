#include "MGLTools.h"

MGLTools::MGLTools()
{

}

vertex MGLTools::newV(float x, float y, float z)//新建一个向量(点)并且返回
{
    vertex a;
    a.x = x;
    a.y = y;
    a.z = z;
    return a;
}

vertex MGLTools::crossProduct(vertex a, vertex b)//向量积就是叉乘
{
    vertex c;
    c.x = a.y*b.z - a.z*b.y;
    c.y = a.z*b.x - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;
    return c;
}

float MGLTools::dot(vertex a, vertex b)//点乘
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vertex MGLTools::norm(vertex a)//归一化
{
    float l = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    a.x /= l;
    a.y /= l;
    a.z /= l;
    return a;
}

vertex MGLTools::add(vertex a, vertex b)
{
    vertex ans;
    ans.x = a.x + b.x;
    ans.y = a.y + b.y;
    ans.z = a.z + b.z;
    return ans;
}

vertex MGLTools::mult(vertex a, float b)
{
    vertex ans;
    ans.x = a.x*b;
    ans.y = a.y*b;
    ans.z = a.z*b;
    return ans;
}

vertex MGLTools::rotate(vertex a, vertex b, float ang)//旋转
{
    //while(ang > 3.1415926)ang-=3.1415926*2;
    //while(ang < -3.1415926)ang += 3.1415926*2;
    b = norm(b);
    vertex q,w,e;
    q = MGLTools::mult(a,cos(ang));
    w = MGLTools::mult(b,(1-cos(ang))*MGLTools::dot(a,b));
    e = MGLTools::mult(MGLTools::crossProduct(a,b),sin(ang));
    q = MGLTools::add(q,w);
    q = MGLTools::add(q,e);
    return q;
}

void MGLTools::setMaterials(materialStruct *m)//设置材质
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m->shininess);
}

void MGLTools::enableTexture()//设置贴图并且开启贴图模式
{
    glEnable(GL_TEXTURE_2D);
}

GLuint MGLTools::loadTexture(pic &texture,GLuint *tex)//载入贴图
{
    //glEnable(GL_TEXTURE_2D);
    glGenTextures(20,tex);
    glBindTexture(GL_TEXTURE_2D,tex[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.imagewidth, texture.imageheight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.pixeldata);
    return 0;
}

void MGLTools::disableTexture()//关闭贴图模式
{
    glDisable(GL_TEXTURE_2D);
}

void MGLTools::QUADQuick(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)//根据四个点画一个面的函数
{
    //这个函数主要用于压缩其他函数的长度，否则一个函数太长不便于浏览
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);
    glEnd();
}

void MGLTools::CylinderGLUTQuick(float x1, float y1, float z1, float x2, float y2, float z2, float r, int s, int l)
{
    // 起始线段：以(0,1,0)为起点,它的长度(distance)通过目标线段计算,
    //           终点坐标为(0,1-distance,0)
    // 目标线段：以(x1,y1,z1)为起点，以(x2,y2,z2)为终点
    // 计算目标向量
    GLfloat   dx   =   x2   -   x1;
    GLfloat   dy   =   y2   -   y1;
    GLfloat   dz   =   z2   -   z1;
    // 算出目标向量模(即AB长度)
    GLfloat   distance  =  sqrt(dx*dx + dy*dy + dz*dz);
    // 计算平移量
    GLfloat  px = x1;
    GLfloat  py = y1;
    GLfloat  pz = z1;
    // 起始线段的末端点
    GLfloat  bx = px;
    GLfloat  by = py;
    GLfloat  bz = pz + distance;
    // 计算起始向量
    GLfloat  sx = bx - x1;
    GLfloat  sy = by - y1;
    GLfloat  sz = bz - z1;
    // 计算向量(sx,sy,sz)与向量(dx,dy,dz)的法向量(sy*dz - dy*sz,sz*dx - sx*dz,sx*dy - dx*sy)
    GLfloat fx = sy*dz - dy*sz;
    GLfloat fy = sz*dx - sx*dz;
    GLfloat fz = sx*dy - dx*sy;
    // 求两向量间的夹角
    // 计算第三条边的长度
    GLfloat ax = fabs(x2 - bx);
    GLfloat ay = fabs(y2 - by);
    GLfloat az = fabs(z2 - bz);
    GLfloat length = sqrt(ax*ax + ay*ay + az*az);
    // 根据余弦定理计算夹角
    GLfloat angle = acos((distance*distance*2 - length*length)/(2*distance*distance))*180.0f/3.14159;
    glPushMatrix();
    glTranslatef(x1,y1,z1);
    glRotatef(angle,fx,fy,fz);
    float ad = 3.1415926*2/l;
    float bd = distance/s;
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<s;j++)
        {
            glBegin(GL_POLYGON);
            glNormal3f(cos((i+0.5)*ad),sin((i+0.5)*ad),0);
            glVertex3f(r*cos(i*ad),r*sin(i*ad),j*bd);
            glVertex3f(r*cos((i+1)*ad),r*sin((i+1)*ad),j*bd);
            glVertex3f(r*cos((i+1)*ad),r*sin((i+1)*ad),(j+1)*bd);
            glVertex3f(r*cos(i*ad),r*sin(i*ad),(j+1)*bd);
            glEnd();
        }
    }
    glPopMatrix();
}

void MGLTools::setLight(lightStruct *l, int light)//设置光源
{
    glLightfv(light, GL_AMBIENT, l->ambient);
    glLightfv(light, GL_DIFFUSE, l->diffuse);
    glLightfv(light, GL_SPECULAR, l->specular);
    glLightfv(light, GL_POSITION, l->positon);
}

void MGLTools::readPic(const char fileName[], pic &unit)//从文件读取图片到结构体，具体原理不用细究，主要是按照文件的具体结构读取的
{
    FILE* pfile = fopen(fileName, "rb");
    if (pfile == 0)
        cout << "cannot find pic:"<< fileName << endl;
    else
    {
        fseek(pfile, 0x0012, SEEK_SET);
        fread(&unit.imagewidth, sizeof(unit.imagewidth), 1, pfile);
        fseek(pfile, 0x0016, SEEK_SET);
        fread(&unit.imageheight, sizeof(unit.imageheight), 1, pfile);
        unit.pixellength = unit.imagewidth * unit.imageheight * 3;
        unit.pixeldata = (GLubyte*)malloc(unit.pixellength*2);
        if (unit.pixeldata == 0) exit(0);
        fseek(pfile, 0x0036, SEEK_SET);
        fread(unit.pixeldata, unit.pixellength, 1, pfile);
        for (int i = 0; i < unit.pixellength; i += 3)
        {
            unsigned char swap;
            swap = unit.pixeldata[i];
            unit.pixeldata[i] = unit.pixeldata[i + 2];
            unit.pixeldata[i + 2] = swap;
        }
    }
    fclose(pfile);
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


void MGLTools::readStl(const char fileName[], model &mod)//读取三维模型
{
    FILE *f = fopen(fileName, "r");
    unsigned char *data;
    unsigned char dataV[4];
    long long int count = 0;
    data = (unsigned char *)malloc(10000 * sizeof(unsigned char));
    fread(data, sizeof(unsigned char), 10000, f);
    for (int i = 0; i < 80; i++)
    {
        count++;
    }
    for (int i = 0; i < 4; i++)
    {
        dataV[i] = data[count];
        count++;
    }
    long long int num = *(unsigned int*)dataV;

    count = 0;
    delete data;
    fclose(f);
    f = fopen(fileName,"r");
    data = (unsigned char *)malloc((num+5)*50 * sizeof(unsigned char));
    fread(data, sizeof(unsigned char), (num+5)*50, f);
    for (int i = 0; i < 80; i++)
    {
        count++;
    }
    for (int i = 0; i < 4; i++)
    {
        dataV[i] = data[count];
        count++;
    }
    for (long long int i = 0; i < num; i++)
    {
        //cout << i << endl;
        float fl[12];
        vertex facet, p1, p2, p3;
        for (int j = 0; j < 12; j++)
        {
            char c[4];
            for (int k = 0; k < 4; k++)
            {
                dataV[k] = data[count];
                count++;
            }
            fl[j] = *(float*)dataV;
        }
        count++;
        count++;
        facet.x = fl[0];
        facet.y = fl[1];
        facet.z = fl[2];
        p1.x = fl[3];
        p1.y = fl[4];
        p1.z = fl[5];
        p2.x = fl[6];
        p2.y = fl[7];
        p2.z = fl[8];
        p3.x = fl[9];
        p3.y = fl[10];
        p3.z = fl[11];
        mod.facet.push_back(facet);
        mod.p1.push_back(p1);
        mod.p2.push_back(p2);
        mod.p3.push_back(p3);
    }
    delete data;
    fclose(f);
}

void MGLTools::drawStl(model &mod,float scale)//绘制STL文件，这种文件里其实储存的就是三角形的面以及其法向向量，因此十分好绘制
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < mod.facet.size(); i++)
    {
        glNormal3f(mod.facet[i].x, mod.facet[i].y, mod.facet[i].z);
        glVertex3f(mod.p1[i].x , mod.p1[i].y / scale, mod.p1[i].z / scale);
        glVertex3f(mod.p2[i].x / scale, mod.p2[i].y / scale, mod.p2[i].z / scale);
        glVertex3f(mod.p3[i].x / scale, mod.p3[i].y / scale, mod.p3[i].z / scale);
    }
    glEnd();
}

float MGLTools::angBtPoints(float x1, float y1, float x2, float y2)//计算两点间夹角
{
    float ang;
    ang = atan2(y2-y1,x2-x1);
    //if(y1-y2<0)ang += 3.1415926;
    return ang;
}

float MGLTools::disBtPoints(float x1, float y1, float x2, float y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

float RA(float ang)
{
    if(ang > 3.1415926)ang -= 3.1415926*2;
    if(ang < -3.1415926)ang += 3.1415926*2;
    return ang;
}

void MGLTools::initGL()
{
    glClearColor(0,0,0,0);//背景颜色黑色
    glColor3f(1,1,1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT1);
}
