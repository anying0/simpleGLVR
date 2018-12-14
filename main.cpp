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

int timeI;//��ǰʱ�̱���
view v;//���ƾ��ۻ��ƵĶ���
camera cam;//���������
locator loc;//��λVR�۾��Ķ���
alManager al;//openAL�������ƶ���

int widthF, heightF;//���ڵĿ�͸�
float width,height,length;//�����ĳ����

float headx,heady,headz;//ͷ��λ������
float headAng,headAtt,headRol;//ͷ���Ƕ�
vertexTransform headAngTrans; //ͷ���Ƕ�����ı任
float headAngT[20],headAttT[20],headRolT[20];//ͨ������VR�۾��õ���ͷ���ǶȵĲ���ֵ��ȡ����20����ƽ���˲�
int headN;//����ֵ����

float eyePosL;//�������ͷ��ת���ǰ�����
float eyePosW;//�ۼ��
float eyePosH;//�������ͷ��ת������¾���
float leftx,lefty,leftz;//��������
vertex eyeAt,eyeUp;//�۳�������
float rightx,righty,rightz;//��������


lightStruct whiteLight = {
    { 0.90, 0.90, 0.90, 1.0 },
    { 0.95, 0.95, 0.95, 1.0 },
    { 0.95, 0.95, 0.95, 1.0 },
    { 0.0, 0.0, 2000.0, 1.0 }
};//��ɫ��Դ




void calEyePos()//����ͷ����������۾�����
{
    //�Ƕ��ǿ���ֱ�Ӹ��Ƶ�



    vertex eyel,eyer;//����������
    vertexTransform trans;//�½����ױ任
    trans = headAngTrans;
    //trans.init();
    //����ͷ�ĽǶ���ת����
    //trans.rotate(VERTEX(0,0,1),-headAng);
    //trans.rotate(VERTEX(0,1,0),-headAtt);
    //trans.rotate(VERTEX(1,0,0),headRol);
    //�õ������۵����ͷ�����꣬�����õ�����������
    vertex up = trans.get(VERTEX(eyePosL,-eyePosW*0.5,eyePosH));
    eyer = MGLTools::add(VERTEX(headx,heady,headz),up);

    up = trans.get(VERTEX(eyePosL,eyePosW*0.5,eyePosH));
    eyel = MGLTools::add(VERTEX(headx,heady,headz),up);

    eyeAt = trans.get(VERTEX(1,0,0));
    eyeUp = trans.get(VERTEX(0,0,1));

    //��ֵ
    leftx = eyel.x;
    lefty = eyel.y;
    leftz = eyel.z;

    rightx = eyer.x;
    righty = eyer.y;
    rightz = eyer.z;
    //cout<<headx<<","<<heady<<","<<headz<<","<<headAtt<<","<<headAng<<","<<headRol<<endl;
}


void reshape(GLsizei w, GLsizei h)//���ڳߴ�仯
{
    widthF = w;
    heightF = h;
}


void displayInit()//��ʼ������
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void displayEnd()//�������Ƴ��򣬿�ʼ���Ƶ���Ļ
{
    glFlush();
}

void displayFrame()//���Ʊ߿�
{
    glDisable(GL_LIGHTING);//�رչ�Ч
    glDisable(GL_DEPTH_TEST);//�ر���ȼ��

    glMatrixMode(GL_PROJECTION);//�۲����ģʽ
    glLoadIdentity();//����
    //����
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

    glEnable(GL_LIGHTING);//�ָ���Ч
    glEnable(GL_DEPTH_TEST);//�ָ���ȼ��
}
void displayEye(float offsetX)//���ƺ���(һ���۾�)
{
    cam.play(0.001,1.6);//��������ͷ
    v.draw(timeI);//���ƻ���
}


void display()//���ƻص�����
{
    calEyePos();//�����۾�λ��
    displayInit();//��ʼ������
    glViewport(0, 0, widthF / 2, heightF);//Ȧ�����Ϊ�滭����
    cam.setPos(leftx,lefty,leftz);//���������������λ��
    cam.setDirection(eyeAt,eyeUp);//��������������۽Ƕ�
    displayEye(0);//����
    glViewport(widthF / 2, 0, widthF / 2, heightF);//ͬ������Ҳ�
    cam.setPos(rightx,righty,rightz);
    cam.setDirection(eyeAt,eyeUp);
    displayEye(0);
    glViewport(0, 0, widthF, heightF);//����ȫ����
    //���Ʊ߿�
    displayFrame();
    displayEnd();
}



void idle()//���е��ûص��������ᱻ�����Ե���
{
    int idleTime = 40;//��������
    clock_t start_time1 = clock();//��ÿ�ʼʱ��
    float ans[6];//VR�۾�λ��
    float basicCam[6];//����ͷ��λ��
    //����ͷ��λ�� + VR�۾�λ�� = ����ͷ��λ��
    timeI += 40;//ʱ������
    if(timeI > v.getTimeLength())timeI -= v.getTimeLength();//����������ʱ���򷵻���ʼʱ��
    v.getBasicCam(timeI,basicCam);//�õ���ǰʱ�̵Ļ���ͷ��λ��
    int ansType;//�������
    int key = 1;
    while(key)//ѭ��ʶ��VR�۾�λ��
    {
        loc.run(ans,ansType);//������ͷʶ��VR�۾�λ��
        if(ansType == ANSTYPE_ALL)//�ɹ�ʶ�������ɶ�λ��
        {
            //��ȡ�Ƕ���Ϣ
            headAngT[headN] = ans[4];
            headAttT[headN] = -ans[3];
            headRolT[headN] = ans[5];
            headN++;//ѭ�������־+1
            if(headN >= 15)headN = 0;
        }
        clock_t end_time1 = clock();//��ʱ
        clock_t timeCost = end_time1 - start_time1;
        if(timeCost > idleTime - 10)//����Ѿ��ӽ�һ�����ڵ�ʱ��
        {
            key = 0;//ֹͣѭ��
        }
    }
    //����ͷ��Ŀ��Ƕ�
    float headAngAim = 0;
    float headAttAim = 0;
    float headRolAim = 0;
    //��ǰ15��VR�۾���������ƽ��
    for(int i = 0;i<15;i++)
    {
        headAngAim += headAngT[i];
        headAttAim += headAttT[i];
        headRolAim += headRolT[i];
    }
    headAngAim /= 15;
    headAttAim /= 15;
    headRolAim /= 15;

    //����ƽ����ʽ��ͷ������ͽǶȸ�ֵ
    headAng = RA(RA(basicCam[3] - headAng)*0.5 + headAng);
    headAtt = RA(RA(basicCam[4] - headAtt)*0.5 + headAtt);
    headRol = RA(RA(basicCam[5]- headRol)*0.3 + headRol);
    headx = (basicCam[0] - headx)*0.5 + headx;
    heady = (basicCam[1] - heady)*0.5 + heady;
    headz = (basicCam[2] - headz)*0.5 + headz;

    //���㳯��������ͷ�����������ڽ���3D��Ч�Ĳ��š�
    headAngTrans.init();
    headAngTrans.rotate(VERTEX(0,0,1),-headAng);
    headAngTrans.rotate(VERTEX(0,1,0),-headAtt);
    headAngTrans.rotate(VERTEX(1,0,0),headRol);
    headAngTrans.rotate(VERTEX(0,0,1),-headAngAim);
    headAngTrans.rotate(VERTEX(0,1,0),-headAttAim);
    headAngTrans.rotate(VERTEX(1,0,0),headRolAim);
    vertex at = headAngTrans.get(VERTEX(1,0,0));
    vertex up = headAngTrans.get(VERTEX(0,0,1));
    //����3D��Ч����λ�úͽǶ�
    al.setListenerPos(VERTEX(headx,heady,headz),VERTEX(0,0,0),at,up);


    cout<<"cam:"<<headx<<","<<heady<<","<<headz<<","<<headAng<<","<<headAtt<<","<<headRol<<endl;
    cout<<"ang:"<<at.x<<","<<at.y<<","<<at.z<<","<<up.x<<","<<up.y<<","<<up.z<<endl;
    glutPostRedisplay();
}

void init()//������ʼ��
{
    MGLTools::setLight(&whiteLight, GL_LIGHT0);
    glEnable(GL_LIGHTING);   //����:ʹ�ù�
    glEnable(GL_LIGHT0);     //��0#��
    glEnable(GL_DEPTH_TEST); //����Ȳ���
    glEnable(GL_TEXTURE_2D);
}

void initValues()//����ֵ�ĳ�ʼ��
{
    //����ʱ�䡢�����ĳ����
    timeI = 0;
    width = 1000;
    height = 1000;
    length = 1000;
    v.init(width,length,height,50);//��ʼ������
    whiteLight.positon[2] = height/2 - 20;//���ù�Դλ���ڰ�������յĶ���ƫ��λ��
    //����ͷ����ʼλ�����۾�λ�õļ��γߴ�
    headx = -50;
    heady = 0;
    headz = 50;
    headAng = 0;
    headAtt = 3.1415926*0.4;
    headRol = 0;
    eyePosL = 0.09;
    eyePosW = 0.075;
    eyePosH = 0.01;

    //��VR�۾�λ�˲���ֵ��ѭ��������0
    for(int i=0;i<20;i++)
    {
        headAngT[i] = 0;
        headAttT[i] = 0;
        headRolT[i] = 0;
    }
    headN = 0;


    cam.init(0,0,30.0,0,0);//��ʼ������ͷ����
    loc.init();//��ʼ��VR�۾�ʶ��ģ��
    al.init();//��ʼ��openAL
    al.loadWav2Buffer(0,"summer.wav");//���뱳������
    al.play(0);//���ű�������
    al.setListenerPos(VERTEX(0,0,0),VERTEX(0,0,0),VERTEX(0,0,0),VERTEX(0,0,0));//���ó�ʼ����λ��
    al.setSourcePos(0,VERTEX(0,0,20),VERTEX(0,0,0),VERTEX(0,0,1));//��������Դλ��
    al.setSourcePhy(0,1.0,50000,1.0,15);//�����������������
}

void keyDown(unsigned char key, int x, int y)
{

}

void keyUp(unsigned char key, int x, int y)
{

}

//���Ǽ��̻ص����������ⰴ�����޼�ֵ�������¼��ص����ں���������ʱ�����Ըı�keys�����й��ڰ���״̬�ļ�¼��
void specDown(int key, int x, int y)
{

}

//���Ǽ��̻ص����������ⰴ��̧���¼��ص����ں���������ʱ�����Ըı�keys�����й��ڰ���״̬�ļ�¼��
void specUp(int key, int x, int y)
{

}


void mouse(int button, int state, int x, int y)//���ص���������⶯��
{

}

void end()
{
    v.end();
    loc.end();
    al.end();
}

int main(int argc, char **argv)//�����������ڳ�ʼ���������ӻص�����
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//�����棬RGB��ɫ������ȼ��
    glutInitWindowSize(1600, 600);//���ڳߴ�
    glutInitWindowPosition(0, 0);//����λ��
    glutCreateWindow("VR�Ӿ�");
    glutDisplayFunc(display);//�󶨻��ƻص�����
    glutIdleFunc(idle);//�󶨿��лص�����
    glutMouseFunc(mouse);//�����ص�����
    glutKeyboardFunc(keyDown);//�󶨼�����ĸ���»ص�����
    glutKeyboardUpFunc(keyUp);//�󶨼�����ĸ�ɿ��ص�����
    glutSpecialFunc(specDown);//�󶨼�����������»ص�����
    glutSpecialUpFunc(specUp);//�󶨼���������ɿ��ص�����
    glutReshapeFunc(reshape);//�󶨴��ڱ��λص�����
    initValues();//ֵ��ʼ��
    init();//GL��ʼ��
    glutMainLoop();//����glut����ѭ��
    system("pause");//��������ͣ
    end();//�ͷ������ڴ�
    return 0;
}
