#include "locator.h"

locator::locator()
{

}

void locator::captureThread(CvCapture *capture,IplImage *pSrcImg,mutex *mut,int *end)
{
    while(1)//循环执行
    {
        IplImage *pic;//创建一个图像指针
        if (pic = cvQueryFrame(capture))//指针指向视频流抓取到的图片的位置
        {

        }
        else //如果没有获取图像则报错退出
        {
            cvReleaseCapture(&capture);
            cout << "no image stream!\n";
            system("pause");
            return ;
        }
        mut->lock();//锁上进程锁，防止冲突访问
        cvCopy(pic,pSrcImg);//将图像里的信息保存到中转图像中。
        if(*end)//如果标志位是1的话则关闭线程
        {
            return ;
        }
        mut->unlock();//解锁进程锁
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
    IplImage *testPic;//用于测试获取到的图像大小的图像指针
    CvSize srcImgSize;//图像大小变量
    capture = cvCaptureFromCAM(0);//打开摄像头，获取视频流
    if (capture == nullptr)
    {
        cout << "获取视频流错误" << endl;
        return ;
    }
    //cvSetCaptureProperty(capture,CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
    //cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);
    // cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);
    rec = new recongnizer(1.0472,0.7853,3.0,2);
    rec->setMode(PRTTP_ERRONLY);
    rec->setSize(15.1, 3.8, 6.7, -2.6);
    mut = new mutex;//新建一个线程锁
    testPic = cvQueryFrame(capture);//获取一张测试图像
    srcImgSize = cvGetSize(testPic);//获取测试图像大小
    pSrcImg = cvCreateImage(cvSize(srcImgSize.width, srcImgSize.height), IPL_DEPTH_8U, 3);//根据测试图像创建中转图像缓存
    getPic = cvCreateImage(cvSize(srcImgSize.width, srcImgSize.height), IPL_DEPTH_8U, 3);//创建最终获取图像的缓存
    endFlag = new int;//新建线程结束标志位并且设为0
    *endFlag = 0;
    capThread = new thread(captureThread,capture,pSrcImg,mut,endFlag);//启动线程
}

void locator::run(float *ans, int &ansType)
{
    mut->lock();
    cvCopy(pSrcImg,getPic);//进程锁锁定后拷贝出需要计算的图像
    mut->unlock();
    rec->run(getPic, ans, ansType);//识别图像
    //cvShowImage("frame", pSrcImg);



    //下面的代码可以看做对于输出格式的详细定义。
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
    *endFlag = 1;//锁定进程锁后设置关闭标志位为1,
    mut->unlock();
    capThread->join();//等待进程结束
    cvReleaseImage(&pSrcImg);//释放内存
    cvReleaseImage(&getPic);
    cvReleaseCapture(&capture);
    delete rec;//删除指针
    delete capThread;
    delete mut;
    delete endFlag;
    //全部清理完毕
}
