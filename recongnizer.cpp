#include "recongnizer.h"


float recongnizer::ParallelScaleRate4P(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,float scale)
{
    float k1 = (y1 - y2) / (x1 - x2 + 0.001);
    float k2 = (y3 - y4) / (x3 - x4 + 0.001);
    float d1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    float d2 = sqrt((x3 - x4)*(x3 - x4) + (y3 - y4)*(y3 - y4));
    float ans = abs(atan(k1) - atan(k2));
    if (ans >= 1.57079)ans = 3.1415926 - ans;
    ans += (abs(d1 / d2 - scale))*3.14 / scale;
    return ans;
}

void recongnizer::calAB()
{
    slipB = (8 * standardSlip / 3.1415926) - 1;
    slipA = (4 / 3.1415926) - ((16 * standardSlip) / (3.1415926*3.1415926));
    pitchB = (8 * standardPitch / 3.1415926) - 1;
    pitchA = (4 / 3.1415926) - ((16 * standardPitch) / (3.1415926*3.1415926));
    //cout << "slipAB:" << slipA << "," << slipB << endl;
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

void recongnizer::run(IplImage * pSrcImg3, float * pos, int &ansType)
{
    for (int i = 0; i < 6; i++)
        pos[i] = -20000;

    clock_t start_time1 = clock();
    //创建窗口并且显示源图片
    float scaleSize = 3;
    CvSize srcImgSize = cvGetSize(pSrcImg3);
    IplImage *pSrcImg2 = cvCreateImage(cvSize(srcImgSize.width / scaleSize, srcImgSize.height / scaleSize), IPL_DEPTH_8U, 3);
    cvResize(pSrcImg3, pSrcImg2);
    srcImgSize.width /= scaleSize;
    srcImgSize.height /= scaleSize;

    IplImage* pSrcImg = cvCreateImage(srcImgSize,IPL_DEPTH_8U,1);
    IplImage* pSrcImgC1 = cvCreateImage(srcImgSize,IPL_DEPTH_8U,1);
    IplImage* pSrcImgC2 = cvCreateImage(srcImgSize,IPL_DEPTH_8U,1);
    IplImage* pSrcImgC3 = cvCreateImage(srcImgSize,IPL_DEPTH_8U,1);
    IplImage* pSrcImgBin = cvCreateImage(srcImgSize, IPL_DEPTH_8U, 1);//黑白图像
    cvSplit(pSrcImg2,pSrcImgC1,pSrcImgC2,pSrcImgC3,NULL);


    float maxTH = 0;
    for(int i=0;i<pSrcImg->imageSize;i++)
    {
        int r = (uchar)pSrcImgC3->imageData[i];
        int g = (uchar)pSrcImgC2->imageData[i];
        int b = (uchar)pSrcImgC1->imageData[i];
        if( ((float)g)/r > 1.4  && ((float)g)/b > 1.4 && (r+g+b)/3.0 > 64)//识别绿色的条件
        {
            pSrcImgBin->imageData[i] = 255;
            pSrcImg->imageData[i] = g;
        }
        else
        {
            pSrcImgBin->imageData[i] = 0;
            pSrcImg->imageData[i] = g*0.2;
        }


                    //min((((255 - abs(r-b))*max(g - (r+b)/2,0))/64),255);
        if(pSrcImg->imageData[i] - 10 > maxTH)
        {
            maxTH = pSrcImg->imageData[i] - 10;
        }
    }
    IplImage* pResImg = cvCloneImage(pSrcImg);
    IplImage* pSrcImgDouble = cvCreateImage(srcImgSize, IPL_DEPTH_32F, 1);
    cvConvertScale(pSrcImg, pSrcImgDouble, 1.0 / 255);


    if (printType <= PRTTP_TESTMOD)
    {
        cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("src", 300, 10);
        cvShowImage("src", pSrcImg);
    }

    long start_time = cvGetTickCount();

    //进行一阶导和二阶导的高斯卷积

    long end_time = cvGetTickCount();
    if(printType <= PRTTP_ANSONLY)std::cout << (end_time - start_time) / cvGetTickFrequency() << endl;//

    //释放高斯卷积模板占用的内存

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = NULL;

    //cvThreshold(pSrcImg, pSrcImgBin, maxTH, 255, CV_THRESH_BINARY);//阈值化
    if (printType <= PRTTP_TESTMOD)
    {
        cvNamedWindow("bin", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("bin", 300, 10);
        cvShowImage("bin", pSrcImgBin);
    }
    cvFindContours(pSrcImgBin, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));//边缘检测
    cvDrawContours(pResImg, contours, cvScalar(100), cvScalar(100), 1);
    //将所有边缘的边框储存起来
    vector<CvRect> areaStar;
    CvSeq* _contours = contours;
    while (NULL != _contours)
    {
        areaStar.push_back(cvBoundingRect(_contours));
        _contours = _contours->h_next;
    }

    cvReleaseMemStorage(&storage);
    //delete contours;
    //delete storage;


    //释放临时变量所占用的内存

    vector<CvPoint2D32f> pointDetect;
    //定义一些与坐标相关的变量
    float minTemp = 255;
    float _i = 0;
    float _j = 0;
    float _s = 0.0;
    float _t = 0.0;
    float _Subx = 0.0;
    float _Suby = 0.0;
    const float halfPixel = 0.5;

    vector<float> ansXs;
    vector<float> ansYs;

    for (vector<CvRect>::const_iterator iter = areaStar.begin(); iter != areaStar.end(); iter++)
    {
        _Subx = iter->x + iter->width*0.5;
        _Suby = iter->y + iter->height*0.5;
        if(_Subx > 0.05*srcImgSize.width && _Subx < 0.95*srcImgSize.width && _Suby > 0.05*srcImgSize.height && _Suby < 0.95*srcImgSize.height)
        {
            pointDetect.push_back(cvPoint2D32f(_Subx, _Subx));
            //std::cout << "右下 x: " << _Subx << " y: " << _Suby << endl;
            cv::Point p;
            p.x = _Subx;
            p.y = _Suby;
            ansXs.push_back(_Subx);
            ansYs.push_back(_Suby);
            cvCircle(pResImg, p, 2, cv::Scalar(0, 0, 255));
        }
    }//end for: iter


    if (ansXs.size() >= 6)
    {
        int a[6];
        int pAns[6];
        int an = ansXs.size();
        float vAns = -5000;
        if (printType <= PRTTP_ANSONLY)
        {
            for (int i = 0; i < an; i++)
            {
                cout << ansXs[i] << "," << ansYs[i] << endl;
            }
        }

        for (a[0] = 0; a[0] < an; a[0]++)
        {
            for (a[1] = 0; a[1] < an; a[1]++)
            {
                if (a[1] != a[0])for (a[2] = 0; a[2] < an; a[2]++)
                {
                    if (a[2] != a[1] && a[2] != a[0])for (a[3] = 0; a[3] < an; a[3]++)
                    {
                        if (a[3] != a[2] && a[3] != a[1] && a[3] != a[0])for (a[4] = 0; a[4] < an; a[4]++)
                        {
                            if (a[4] != a[3] && a[4] != a[2] && a[4] != a[1] && a[4] != a[0])for (a[5] = 0; a[5] < an; a[5]++)
                            {
                                if (a[5] != a[4] && a[5] != a[3] && a[5] != a[2] && a[5] != a[1] && a[5] != a[0])
                                {
                                    if ((ansYs[a[0]] < ansYs[a[3]]) &&
                                            (ansYs[a[0]] < ansYs[a[4]]) &&
                                            (ansYs[a[0]] < max(ansYs[a[1]], ansYs[a[2]])) &&
                                            (ansYs[a[1]] < ansYs[a[3]]) &&
                                            (ansYs[a[2]] < ansYs[a[4]]) &&
                                            (ansXs[a[1]] < ansXs[a[2]]) &&
                                            (ansXs[a[3]] < ansXs[a[4]]) &&
                                            (ansXs[a[1]] < ansXs[a[0]]) &&
                                            (ansXs[a[0]] < ansXs[a[2]]) &&
                                            (ansYs[a[5]] < ansYs[a[3]]) &&
                                            (ansYs[a[5]] < ansYs[a[4]]) &&
                                            (ansXs[a[5]] < ansXs[a[2]]) &&
                                            (ansXs[a[5]] < ansXs[a[4]]) &&
                                            (ansXs[a[5]] > ansXs[a[1]]) &&
                                            (ansXs[a[5]] > ansXs[a[3]]) &&
                                            (
                                                (ansYs[a[1]] < ansYs[a[2]] && ansXs[a[1]] < ansXs[a[0]] && ansYs[a[0]] < ansYs[a[2]] && ansXs[a[2]] > ansXs[a[3]]) ||
                                                (ansYs[a[1]] > ansYs[a[2]] && ansXs[a[2]] > ansXs[a[0]] && ansYs[a[0]] < ansYs[a[1]] && ansXs[a[1]] < ansXs[a[4]])
                                                )
                                            )
                                    {
                                        float s1 = ParallelScaleRate4P(ansXs[a[1]], ansYs[a[1]], ansXs[a[3]], ansYs[a[3]], ansXs[a[2]], ansYs[a[2]], ansXs[a[4]], ansYs[a[4]],1.0);
                                        float s2 = ParallelScaleRate4P(
                                                    ansXs[a[0]], ansYs[a[0]], (ansXs[a[1]] + ansXs[a[2]]) / 2.0, (ansYs[a[1]] + ansYs[a[2]]) / 2.0,
                                                ansXs[a[0]], ansYs[a[0]], (ansXs[a[3]] + ansXs[a[4]]) / 2.0, (ansYs[a[3]] + ansYs[a[4]]) / 2.0,
                                                verticalSizeRate);
                                        s1 = s1 + s2;
                                        if (vAns < 0 || (vAns > s1))
                                        {
                                            if (printType <= PRTTP_ANSONLY)std::cout << "find one:" << s1 << endl;
                                            vAns = s1;
                                            for (int i = 0; i < 6; i++)
                                            {
                                                pAns[i] = a[i];
                                                if (printType <= PRTTP_ANSONLY)cout << i << ":" << ansXs[pAns[i]] << "," << ansYs[pAns[i]] << endl;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        //std::cout << "refuse" << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (vAns > -4000)
        {
            if (printType <= PRTTP_ERRONLY)
            {
                if (printType <= PRTTP_ANSONLY)cout << "score:" << vAns << endl;
                for (int i = 0; i < 6; i++)
                {
                    char s[] = "0";
                    s[0] += i;
                    cv::Point p;
                    p.x = ansXs[pAns[i]];
                    p.y = ansYs[pAns[i]];
                    if (printType <= PRTTP_ANSONLY)cout << s << ":" << ansXs[pAns[i]] << "," << ansYs[pAns[i]] << endl;
                    //cout << s << endl;
                    CvFont font;
                    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1, 1, 1, 2, 8);
                    cvPutText(pResImg, s, p, &font, cv::Scalar(255, 255, 255));
                }
            }
            float hw = srcImgSize.width / 2;
            float hh = srcImgSize.height / 2;
            float cx = (ansXs[pAns[1]] + ansXs[pAns[2]]) / 2;
            float cy = (ansYs[pAns[1]] + ansYs[pAns[2]]) / 2;
            float xAng = ((cx - (hw)) / (hw))*(horizontalFOV / 2);
            float yAng = -1*((cy - (hh)) / (hh))*(verticalFOV / 2);

            float fx = (ansXs[pAns[3]] + ansXs[pAns[4]]) / 2;
            float fy = (ansYs[pAns[3]] + ansYs[pAns[4]]) / 2;

            float slipX = atan((ansXs[pAns[5]] - cx) / (abs(ansXs[pAns[1]] - cx) + 0.001));
            float slip = slipX * slipX*slipA + abs(slipX) * slipB;
            if (slipX > 0)slip = -1 * slip;
            float pitchX = atan((ansYs[pAns[5]] - cy) / (abs(fy - cy) + 0.001));
            //cout<<"---" <<pitchX<<endl;
            float pitch = pitchX * pitchX*pitchA + abs(pitchX) * pitchB;
            if (pitchX < 0)pitch = -1 * pitch;
            float roll = atan2(ansYs[pAns[2]] - ansYs[pAns[1]], ansXs[pAns[2]] - ansXs[pAns[1]]);

            float widthS = sqrt((ansXs[pAns[1]] - ansXs[pAns[2]])*(ansXs[pAns[1]] - ansXs[pAns[2]]) + (ansYs[pAns[1]] - ansYs[pAns[2]])* (ansYs[pAns[1]] - ansYs[pAns[2]]));
            float widthT = abs(widthS / cos(slip));
            float FK = (srcImgSize.width / widthT);
            float distance = width * (FK)*0.5 / tan(horizontalFOV / 2);
            if (printType <= PRTTP_ANSONLY)
            {
                cout << "xAng:" << xAng << endl;
                cout << "yAng:" << yAng << endl;
                cout << "distance:" << distance << endl;
                cout << "pitch:" << pitch << endl;
                cout << "slip:" << slip << endl;
                cout << "roll:" << roll << endl;
            }
            pos[0] = xAng;
            pos[1] = yAng;
            pos[2] = distance;
            pos[3] = pitch;
            pos[4] = slip;
            pos[5] = roll;
            ansType = ANSTYPE_ALL;

        }
        else
        {
            ansType = ANSTYPE_NON;
            if (printType <= PRTTP_ANSONLY)cout << "no solution!" << endl;
        }

    }
    else if(ansXs.size() != 0)
    {
        if (printType <= PRTTP_ANSONLY)std::cout << "not enough points detected." << endl;
        int an = ansXs.size();
        float cx = 0;
        float cy = 0;
        for (int i = 0; i < an; i++)
        {
            cx += ansXs[i];
            cy += ansYs[i];
        }
        cx /= an;
        cy /= an;
        float hw = srcImgSize.width / 2;
        float hh = srcImgSize.height / 2;
        float xAng = ((cx - (hw)) / (hw))*(horizontalFOV / 2);
        float yAng = -1 * ((cy - (hh)) / (hh))*(verticalFOV / 2);
        pos[0] = xAng;
        pos[1] = yAng;
        ansType = ANSTYPE_POS;
    }
    else
    {
        ansType = ANSTYPE_NON;
        if (printType <= PRTTP_ANSONLY)cout << "no solution!(no points detected)" << endl;
    }
    if (printType <= PRTTP_TESTMOD)
    {
        cvNamedWindow("res", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("res", 300, 10);
        cvShowImage("res", pResImg);
    }
    if (printType <= PRTTP_ERRONLY)
    {
        cvShowImage("VR眼镜角度识别", pResImg);
    }
    if(printType <= PRTTP_TESTMOD)
        int k = cvWaitKey(0);
    else if(printType <= PRTTP_ANSONLY)
        system("pause");
    if (printType <= PRTTP_TESTMOD)
    {
        cvDestroyWindow("res");
        cvDestroyWindow("bin");
        cvDestroyWindow("src");
    }
    cvReleaseImage(&pSrcImg);
    cvReleaseImage(&pSrcImgC1);
    cvReleaseImage(&pSrcImgC2);
    cvReleaseImage(&pSrcImgC3);
    cvReleaseImage(&pSrcImg2);
    cvReleaseImage(&pSrcImgBin);
    cvReleaseImage(&pSrcImgDouble);
    cvReleaseImage(&pResImg);
    clock_t end_time1 = clock();
    if (printType <= PRTTP_ERRONLY)std::cout <<"time cost:"<< (end_time1 - start_time1) << endl;
    return ;
}

void recongnizer::setSize(float widthi, float sidei, float heighti, float deepi)
{
    standardSlip = atan(widthi / (2 * deepi));
    standardPitch = atan(sidei / deepi);
    verticalSizeRate = (heighti - sidei) / heighti;
    width = widthi;
    calAB();
}

void recongnizer::setMode(int modeI)
{
    printType = modeI;
    if (modeI < PRTTP_NOTHING)
    {
        cvNamedWindow("VR眼镜角度识别", CV_WINDOW_AUTOSIZE);
    }
}

void recongnizer::ansToChar(float * in, char * out)
{
    char *inC = (char *)in;
    for (int i = 0; i < 24; i++)
    {
        out[i] = inC[i];
    }
}


recongnizer::recongnizer(float verticalFOVIn, float horizontalFOVIn, float sizeThreholdIn, int prtTypeI)
{
    sizeThrehold = sizeThreholdIn;
    verticalFOV = verticalFOVIn;
    horizontalFOV = horizontalFOVIn;
    standardSlip = 0.7854;
    standardPitch = 0.46365;
    verticalSizeRate = 0.53734;
    width = 650;
    printType = prtTypeI;
    if (prtTypeI < PRTTP_NOTHING)
    {
        cvNamedWindow("VR眼镜角度识别", CV_WINDOW_NORMAL);
        cvResizeWindow("VR眼镜角度识别",300,300);
    }
    calAB();
}


recongnizer::~recongnizer()
{
}
