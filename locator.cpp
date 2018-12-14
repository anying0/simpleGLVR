#include "locator.h"

locator::locator()
{

}

void locator::captureThread(CvCapture *capture,IplImage *pSrcImg,mutex *mut,int *end)
{
    while(1)//ѭ��ִ��
    {
        IplImage *pic;//����һ��ͼ��ָ��
        if (pic = cvQueryFrame(capture))//ָ��ָ����Ƶ��ץȡ����ͼƬ��λ��
        {

        }
        else //���û�л�ȡͼ���򱨴��˳�
        {
            cvReleaseCapture(&capture);
            cout << "no image stream!\n";
            system("pause");
            return ;
        }
        mut->lock();//���Ͻ���������ֹ��ͻ����
        cvCopy(pic,pSrcImg);//��ͼ�������Ϣ���浽��תͼ���С�
        if(*end)//�����־λ��1�Ļ���ر��߳�
        {
            return ;
        }
        mut->unlock();//����������
    }
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

void locator::init()
{
    IplImage *testPic;//���ڲ��Ի�ȡ����ͼ���С��ͼ��ָ��
    CvSize srcImgSize;//ͼ���С����
    capture = cvCaptureFromCAM(0);//������ͷ����ȡ��Ƶ��
    if (capture == nullptr)
    {
        cout << "��ȡ��Ƶ������" << endl;
        return ;
    }
    //cvSetCaptureProperty(capture,CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
    //cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);
    // cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);
    rec = new recongnizer(1.0472,0.7853,3.0,2);
    rec->setMode(PRTTP_ERRONLY);
    rec->setSize(15.1, 3.8, 6.7, -2.6);
    mut = new mutex;//�½�һ���߳���
    testPic = cvQueryFrame(capture);//��ȡһ�Ų���ͼ��
    srcImgSize = cvGetSize(testPic);//��ȡ����ͼ���С
    pSrcImg = cvCreateImage(cvSize(srcImgSize.width, srcImgSize.height), IPL_DEPTH_8U, 3);//���ݲ���ͼ�񴴽���תͼ�񻺴�
    getPic = cvCreateImage(cvSize(srcImgSize.width, srcImgSize.height), IPL_DEPTH_8U, 3);//�������ջ�ȡͼ��Ļ���
    endFlag = new int;//�½��߳̽�����־λ������Ϊ0
    *endFlag = 0;
    capThread = new thread(captureThread,capture,pSrcImg,mut,endFlag);//�����߳�
}

void locator::run(float *ans, int &ansType)
{
    mut->lock();
    cvCopy(pSrcImg,getPic);//�����������󿽱�����Ҫ�����ͼ��
    mut->unlock();
    rec->run(getPic, ans, ansType);//ʶ��ͼ��
    //cvShowImage("frame", pSrcImg);



    //����Ĵ�����Կ������������ʽ����ϸ���塣
    if (ansType == ANSTYPE_NON)
        cout << "NO SOLUTION!" << endl;
    else if (ansType == ANSTYPE_POS)
    {
        cout << "POSITION ONLY!" << endl;
        //cout << "horiziontal angle:" << ans[0] << endl;;
        //cout << "vertical angle:" << ans[1] << endl;;
    }
    else
    {
        cout << "ALL SOLUTION GET!" << endl;
       // cout << "horiziontal angle:" << ans[0] << endl;;
        //cout << "vertical angle:" << ans[1] << endl;;
       // cout << "distance:" << ans[2] << endl;;
        //cout << "pitch:" << ans[3] << endl;;
       // cout << "slip:" << ans[4] << endl;;
       // cout << "roll:" << ans[5] << endl;;
    }


}

void locator::end()
{
    mut->lock();
    *endFlag = 1;//���������������ùرձ�־λΪ1,
    mut->unlock();
    capThread->join();//�ȴ����̽���
    cvReleaseImage(&pSrcImg);//�ͷ��ڴ�
    cvReleaseImage(&getPic);
    cvReleaseCapture(&capture);
    delete rec;//ɾ��ָ��
    delete capThread;
    delete mut;
    delete endFlag;
    //ȫ���������
}
