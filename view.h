#ifndef VIEW_H
#define VIEW_H
#include <mgltools.h>
#include <vertexTransform.h>


class view
{
private:

public:


    view();
    void init(float w,float l,float h,int div);//��ʼ�����������ó�����Լ��ָ�����
    void draw(int time);//���Ƴ��������뵱ǰ��ʱ��
    void end();//�����������ͷ��ڴ�
    void getBasicCam(int time,float *ans);//���ָ��ʱ�̵��������׼λ��.
    int getTimeLength();//��ù���������ʱ��

};

#endif // VIEW_H
