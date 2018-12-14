#ifndef LOCATOR_H
#define LOCATOR_H
#include <recongnizer.h>
#include <thread>
#include <mutex>

//VR�۾�ʶ���λ��ʶ����
//�����첽���ٶ�ȡͼ�����ɫ��Ŀ�����ʶ��
//���Ҽ���Ŀ���λ����Ϣ
//���ߣ�yinan Miao from Beihang Univ.
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645

class locator
{
private:
    recongnizer *rec;//ʶ����
    CvCapture *capture;//������ָ��
    thread *capThread;//ͼ���ȡ�߳�ָ��
    IplImage *pSrcImg;//��תͼ������ָ��
    mutex *mut;//�߳���
    IplImage *getPic;//���ջ�ȡ����ͼ��ָ��
    int *endFlag;//�߳̽�����־����ָ��

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
    static void captureThread(CvCapture *capture,IplImage *pSrcImg,mutex *mut,int *end);//�߳�ִ�к������첽������Ƶ��������capture,�Լ�ͼ���߳����Լ�ֹͣ��־���ڴ��ַ��
    locator();//���캯��

    void init();//��ʼ������
    void run(float *ans,int &ansType);//����һ�Σ����Եõ�һ�ε�ǰ����ͷ��Ϣ��ͼ����ʶ��λ����Ϣ������λ��ֵ�Լ���������
    //**  posΪλ�����������ָ�룬��������Ϊ6��                                                **
    //**  pos[0]Ϊ�����ˮƽ��λ�ã�����������ͷ����Ϊ������ʱ��Ŀ���ˮƽλ��ƫ�ǡ�����ƫΪ����**
    //**  pos[1]Ϊ�������ֱ��λ�ã�����������ͷ����Ϊ������ʱ��Ŀ�����ֵλ��ƫ�ǡ�����ƫΪ����**
    //**  pos[2]Ϊ�����Ŀ����룬��������ͷ��ʶ��Ŀ��ľ��롣                                  **
    //**  pos[3]ΪĿ��ĸ����ǣ�������Ŀ�����泯������ͷΪ������ʱ��Ŀ��ĸ����ǡ�����Ϊ����    **
    //**  pos[4]ΪĿ��Ĳ໬�ǣ�������Ŀ�����泯������ͷΪ������ʱ��Ŀ��Ĳ໬�ǡ����Ҳ໬Ϊ����**
    //**  pos[5]ΪĿ��Ĺ�ת�ǣ�������Ŀ�����泯������ͷΪ������ʱ��Ŀ��Ĺ�ת�ǣ�˳ʱ��Ϊ����  **
    //**  һЩ���ͣ�                                                                            **
    //**    ˮƽ��λ�ú���ֱ��λ�ã�ֱ�Ӵ���Ŀ������������ͷ�е�λ�ã�x,y�����Ǹ���Ŀ�������ֵ **
    //**  ֱ�ӻ���Ϊ�Ƕ�ֵ�����ġ���Ͼ���ֵ�����Լ���Ŀ���������������λ��(x,y,z)��         **
    //**    �����ǡ��໬�ǡ���ת�Ƿ�ӳ��Ŀ����ԵĽ���̬����Ŀ�����泯������ͷΪ���������˼ **
    //**  ����������Ŀ��������ʱ��ֻҪĿ����ȫ����������ͷ������û�й�ת����������ͷ�п��Կ���û**
    //**  ���κη���任��ʶ��Ŀ������ͼ��ʱ����ʱ�ĸ����ǡ��໬�ǡ���ת�ǽ�Ϊ0���ڴ˻����ϣ��� **
    //**  ��������̬�ı仯����ʶ��Ϊ�����ǡ��໬�ǡ���ת�ǡ����Ŀ�����ֱ��λ���Լ�ˮƽ��λ�ã�**
    //**  �����ǡ��໬�ǡ���ת�ǣ����Ի���Ϊ��׼����Ը����ǡ���Բ໬���Լ���Թ�ת�ǡ�        **
    //**	���pos�е�ֵ��ȫ��ӳ�������ɶȵ���̬��Ϣ��                                         **
    void end();//�رպ���
};

#endif // LOCATOR_H
