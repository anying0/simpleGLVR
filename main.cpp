#include <Windows.h>

#include <al.h>
#include <alc.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include "GL/glut.h"
#include <mgltools.h>
#include <vector>
#include <camera.h>
#include <view.h>
#include <vertexTransform.h>
#include <locator.h>
#include "almanager.h"


using namespace std;

int timeI;//当前时刻变量
view v;//控制景观绘制的对象
camera cam;//摄像机对象
locator loc;//定位VR眼镜的对象
alManager al;//openAL声音控制对象

int widthF, heightF;//窗口的宽和高
float width,height,length;//场景的长宽高

float headx,heady,headz;//头部位置坐标
float headAng,headAtt,headRol;//头部角度
vertexTransform headAngTrans; //头部角度坐标的变换
float headAngT[20],headAttT[20],headRolT[20];//通过测量VR眼镜得到的头部角度的测量值，取连续20组做平均滤波
int headN;//测量值长度

float eyePosL;//眼相对于头部转轴的前后距离
float eyePosW;//眼间距
float eyePosH;//眼相对于头部转轴的上下距离
float leftx,lefty,leftz;//左眼坐标
vertex eyeAt,eyeUp;//眼朝向向量
float rightx,righty,rightz;//右眼坐标


lightStruct whiteLight = {
    { 0.90, 0.90, 0.90, 1.0 },
    { 0.95, 0.95, 0.95, 1.0 },
    { 0.95, 0.95, 0.95, 1.0 },
    { 0.0, 0.0, 2000.0, 1.0 }
};//白色光源




void calEyePos()//根据头部坐标计算眼睛坐标
{
    //角度是可以直接复制的



    vertex eyel,eyer;//左右眼坐标
    vertexTransform trans;//新建基底变换
    trans = headAngTrans;
    //trans.init();
    //按照头的角度旋转基底
    //trans.rotate(VERTEX(0,0,1),-headAng);
    //trans.rotate(VERTEX(0,1,0),-headAtt);
    //trans.rotate(VERTEX(1,0,0),headRol);
    //得到左右眼的相对头的坐标，进而得到左右眼坐标
    vertex up = trans.get(VERTEX(eyePosL,-eyePosW*0.5,eyePosH));
    eyer = MGLTools::add(VERTEX(headx,heady,headz),up);

    up = trans.get(VERTEX(eyePosL,eyePosW*0.5,eyePosH));
    eyel = MGLTools::add(VERTEX(headx,heady,headz),up);

    eyeAt = trans.get(VERTEX(1,0,0));
    eyeUp = trans.get(VERTEX(0,0,1));

    //赋值
    leftx = eyel.x;
    lefty = eyel.y;
    leftz = eyel.z;

    rightx = eyer.x;
    righty = eyer.y;
    rightz = eyer.z;
    //cout<<headx<<","<<heady<<","<<headz<<","<<headAtt<<","<<headAng<<","<<headRol<<endl;
}


void reshape(GLsizei w, GLsizei h)//窗口尺寸变化
{
    widthF = w;
    heightF = h;
}


void displayInit()//初始化绘制
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void displayEnd()//结束绘制程序，开始绘制到屏幕
{
    glFlush();
}

void displayFrame()//绘制边框
{
    glDisable(GL_LIGHTING);//关闭光效
    glDisable(GL_DEPTH_TEST);//关闭深度检测

    glMatrixMode(GL_PROJECTION);//观察矩阵模式
    glLoadIdentity();//清零
    //绘制
    glOrtho(-1, 1, -1, 1, -1, 1);
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, -1, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);

    glVertex3f(-0.5 - 0.1, 0, 0);
    glVertex3f(-0.5 + 0.1, 0, 0);
    glVertex3f(-0.5 , -0.1, 0);
    glVertex3f(-0.5 , 0.1 , 0);

    glVertex3f(0.5 - 0.1, 0, 0);
    glVertex3f(0.5 + 0.1, 0, 0);
    glVertex3f(0.5, -0.1, 0);
    glVertex3f(0.5, 0.1, 0);

    glEnd();

    glEnable(GL_LIGHTING);//恢复光效
    glEnable(GL_DEPTH_TEST);//恢复深度检测
}
void displayEye(float offsetX)//绘制函数(一个眼睛)
{
    cam.play(0.001,1.6);//设置摄像头
    v.draw(timeI);//绘制画面
}


void display()//绘制回调函数
{
    calEyePos();//计算眼睛位置
    displayInit();//初始化绘制
    glViewport(0, 0, widthF / 2, heightF);//圈定左侧为绘画区域
    cam.setPos(leftx,lefty,leftz);//设置摄像机到左眼位置
    cam.setDirection(eyeAt,eyeUp);//设置摄像机到左眼角度
    displayEye(0);//绘制
    glViewport(widthF / 2, 0, widthF / 2, heightF);//同理绘制右侧
    cam.setPos(rightx,righty,rightz);
    cam.setDirection(eyeAt,eyeUp);
    displayEye(0);
    glViewport(0, 0, widthF, heightF);//锁定全画幅
    //绘制边框
    displayFrame();
    displayEnd();
}



void idle()//空闲调用回调函数，会被周期性调用
{
    int idleTime = 40;//运行周期
    clock_t start_time1 = clock();//获得开始时间
    float ans[6];//VR眼镜位姿
    float basicCam[6];//基础头部位姿
    //基础头部位姿 + VR眼镜位姿 = 最终头部位姿
    timeI += 40;//时间增加
    if(timeI > v.getTimeLength())timeI -= v.getTimeLength();//如果超出最大时长则返回起始时间
    v.getBasicCam(timeI,basicCam);//得到当前时刻的基础头部位姿
    int ansType;//结果种类
    int key = 1;
    while(key)//循环识别VR眼镜位姿
    {
        loc.run(ans,ansType);//从摄像头识别VR眼镜位姿
        if(ansType == ANSTYPE_ALL)//成功识别六自由度位姿
        {
            //获取角度信息
            headAngT[headN] = ans[4];
            headAttT[headN] = -ans[3];
            headRolT[headN] = ans[5];
            headN++;//循环数组标志+1
            if(headN >= 15)headN = 0;
        }
        clock_t end_time1 = clock();//计时
        clock_t timeCost = end_time1 - start_time1;
        if(timeCost > idleTime - 10)//如果已经接近一个周期的时长
        {
            key = 0;//停止循环
        }
    }
    //设置头部目标角度
    float headAngAim = 0;
    float headAttAim = 0;
    float headRolAim = 0;
    //对前15次VR眼镜结果求和求平均
    for(int i = 0;i<15;i++)
    {
        headAngAim += headAngT[i];
        headAttAim += headAttT[i];
        headRolAim += headRolT[i];
    }
    headAngAim /= 15;
    headAttAim /= 15;
    headRolAim /= 15;

    //利用平滑公式给头部坐标和角度赋值
    headAng = RA(RA(basicCam[3] - headAng)*0.5 + headAng);
    headAtt = RA(RA(basicCam[4] - headAtt)*0.5 + headAtt);
    headRol = RA(RA(basicCam[5]- headRol)*0.3 + headRol);
    headx = (basicCam[0] - headx)*0.5 + headx;
    heady = (basicCam[1] - heady)*0.5 + heady;
    headz = (basicCam[2] - headz)*0.5 + headz;

    //计算朝向向量与头顶向量，用于进行3D音效的播放。
    headAngTrans.init();
    headAngTrans.rotate(VERTEX(0,0,1),-headAng);
    headAngTrans.rotate(VERTEX(0,1,0),-headAtt);
    headAngTrans.rotate(VERTEX(1,0,0),headRol);
    headAngTrans.rotate(VERTEX(0,0,1),-headAngAim);
    headAngTrans.rotate(VERTEX(0,1,0),-headAttAim);
    headAngTrans.rotate(VERTEX(1,0,0),headRolAim);
    vertex at = headAngTrans.get(VERTEX(1,0,0));
    vertex up = headAngTrans.get(VERTEX(0,0,1));
    //设置3D音效听者位置和角度
    al.setListenerPos(VERTEX(headx,heady,headz),VERTEX(0,0,0),at,up);


    cout<<"cam:"<<headx<<","<<heady<<","<<headz<<","<<headAng<<","<<headAtt<<","<<headRol<<endl;
    cout<<"ang:"<<at.x<<","<<at.y<<","<<at.z<<","<<up.x<<","<<up.y<<","<<up.z<<endl;
    glutPostRedisplay();
}

void init()//参数初始化
{
    MGLTools::setLight(&whiteLight, GL_LIGHT0);
    glEnable(GL_LIGHTING);   //开关:使用光
    glEnable(GL_LIGHT0);     //打开0#灯
    glEnable(GL_DEPTH_TEST); //打开深度测试
    glEnable(GL_TEXTURE_2D);
}

void initValues()//各个值的初始化
{
    //设置时间、场景的长宽高
    timeI = 0;
    width = 1000;
    height = 1000;
    length = 1000;
    v.init(width,length,height,50);//初始化场景
    whiteLight.positon[2] = height/2 - 20;//设置光源位置在半球形天空的顶部偏下位置
    //设置头部初始位姿与眼睛位置的几何尺寸
    headx = -50;
    heady = 0;
    headz = 50;
    headAng = 0;
    headAtt = 3.1415926*0.4;
    headRol = 0;
    eyePosL = 0.09;
    eyePosW = 0.075;
    eyePosH = 0.01;

    //将VR眼镜位姿测量值的循环数组置0
    for(int i=0;i<20;i++)
    {
        headAngT[i] = 0;
        headAttT[i] = 0;
        headRolT[i] = 0;
    }
    headN = 0;


    cam.init(0,0,30.0,0,0);//初始化摄像头参数
    loc.init();//初始化VR眼镜识别模块
    al.init();//初始化openAL
    al.loadWav2Buffer(0,"summer.wav");//载入背景音乐
    al.play(0);//播放背景音乐
    al.setListenerPos(VERTEX(0,0,0),VERTEX(0,0,0),VERTEX(0,0,0),VERTEX(0,0,0));//设置初始听者位姿
    al.setSourcePos(0,VERTEX(0,0,20),VERTEX(0,0,0),VERTEX(0,0,1));//设置声音源位姿
    al.setSourcePhy(0,1.0,50000,1.0,15);//设置声音的物理参数
}

void keyDown(unsigned char key, int x, int y)
{

}

void keyUp(unsigned char key, int x, int y)
{

}

//这是键盘回调函数，特殊按键（无键值）按下事件回调。在函数被调用时，可以改变keys数组中关于按键状态的记录。
void specDown(int key, int x, int y)
{

}

//这是键盘回调函数，特殊按键抬起事件回调。在函数被调用时，可以改变keys数组中关于按键状态的记录。
void specUp(int key, int x, int y)
{

}


void mouse(int button, int state, int x, int y)//鼠标回调函数，检测动作
{

}

void end()
{
    v.end();
    loc.end();
    al.end();
}

int main(int argc, char **argv)//主函数，用于初始化并且链接回调函数
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//单缓存，RGB颜色，有深度检测
    glutInitWindowSize(1600, 600);//窗口尺寸
    glutInitWindowPosition(0, 0);//窗口位置
    glutCreateWindow("VR视景");
    glutDisplayFunc(display);//绑定绘制回调函数
    glutIdleFunc(idle);//绑定空闲回调函数
    glutMouseFunc(mouse);//绑定鼠标回调函数
    glutKeyboardFunc(keyDown);//绑定键盘字母按下回调函数
    glutKeyboardUpFunc(keyUp);//绑定键盘字母松开回调函数
    glutSpecialFunc(specDown);//绑定键盘特殊键按下回调函数
    glutSpecialUpFunc(specUp);//绑定键盘特殊键松开回调函数
    glutReshapeFunc(reshape);//绑定窗口变形回调函数
    initValues();//值初始化
    init();//GL初始化
    glutMainLoop();//进入glut的主循环
    system("pause");//结束后暂停
    end();//释放所有内存
    return 0;
}
