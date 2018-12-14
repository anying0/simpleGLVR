#pragma once


#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>


//用于识别目标信标的类
//本类涉及到过多数学以及结构解算因此不会在算法中插入过多注释
//可以识别绿色的指定结构化信息
//作者：yinan Miao from Beihang Univ.
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）
//QQ:1131042645

#define ANSTYPE_ALL 0//得到了所有的位姿数据
#define ANSTYPE_POS 1//只得到了横向、纵向角数据
#define ANSTYPE_NON 2//没有识别到任何目标

#define PRTTP_TESTMOD 0//测试模式，显示所有信息
#define PRTTP_ANSONLY 1//只显示结果模式
#define PRTTP_ERRONLY 2//只显示错误信息模式
#define PRTTP_NOTHING 3//不显示模式


using namespace std;


class recongnizer
{
private:
    float sizeThrehold;//识别目标点的直径尺寸阈值
    float verticalSizeRate;//纵向尺寸比率(一个尺寸特征)
    float horizontalFOV;//纵向视角
    float verticalFOV;//横向视角
    float standardSlip;//标准侧滑角(一个尺寸特征)
    float standardPitch;//标准俯仰角(一个尺寸特征)
    float slipA;//一些内构尺寸特征
    float slipB;//一些内构尺寸特征
    float pitchA;//一些内构尺寸特征
    float pitchB;//一些内构尺寸特征
    float width;//宽度(尺寸特征)
    int printType;//输出类型，用于控制输出
private:
	float ParallelScaleRate4P(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,float scale); 
	void calAB();
public:
	//***************************************图像分析函数*****************************************
	//**  调用这个函数来分析图像中的识别目标的位姿。                                            **
	//**                                                                                        **
	//**  pSrcImg为图像输入，要求不能有过大的噪点。                                             **
	//**                                                                                        **
	//**  pos为位姿输出的数组指针，长度至少为6。                                                **
	//**  pos[0]为输出的水平角位置，代表以摄像头方向为正方向时，目标的水平位置偏角。向右偏为正。**
	//**  pos[1]为输出的竖直角位置，代表以伸向头方向为正方向时，目标的数值位置偏角。向上偏为正。**
	//**  pos[2]为输出的目标距离，代表摄像头与识别目标的距离。                                  **
	//**  pos[3]为目标的俯仰角，代表以目标正面朝向摄像头为正方向时，目标的俯仰角。上仰为正。    **
	//**  pos[4]为目标的侧滑角，代表以目标正面朝向摄像头为正方向时，目标的侧滑角。向右侧滑为正。**
	//**  pos[5]为目标的滚转角，代表以目标正面朝向摄像头为正方向时，目标的滚转角，顺时针为正。  **
	//**  一些解释：                                                                            **
	//**    水平角位置和竖直角位置，直接代表目标中心在摄像头中的位置（x,y），是根据目标的像素值 **
	//**  直接换算为角度值而来的。配合距离值，可以计算目标相对于自身的相对位置(x,y,z)。         **
	//**    俯仰角、侧滑角、滚转角反映了目标相对AUV的角姿态。以目标正面朝向摄像头为正方向的意思 **
	//**  是设置无论目标在哪里时，只要目标完全正对着摄像头，并且没有滚转，即从摄像头中可以看到没**
	//**  有任何仿射变换的识别目标正面图像时，这时的俯仰角、侧滑角、滚转角皆为0。在此基础上，如 **
	//**  发生角姿态的变化，则识别为俯仰角、侧滑角、滚转角。配合目标的竖直角位置以及水平角位置，**
	//**  俯仰角、侧滑角、滚转角，可以换算为标准的相对俯仰角、相对侧滑角以及相对滚转角。        **
	//**	因此pos中的值完全反映了六自由度的姿态信息。                                         **
	//**                                                                                        **
	//**  ansType为输出类别的输出。如果ansType的值为ANSTYPE_ALL，则识别到了全六自由度信息，pos[0**
	//**  -6]全部都有效。如果ansType的值为ANSTYPE_POS，则只识别到了位置角信息，只有pos[0]和pos[1**
	//**  ]是有效的。如果ansType的值为ANSTYPE_NON，则没有识别到目标，pos[]中无有效信息。        **
	//********************************************************************************************
	void run(IplImage *pSrcImg3, float *pos, int &ansType);
	//*************************************目标参数设置函数***************************************
	//**  这个函数用于设置识别目标的一些几何参数。如果参数不正确是无法进行识别的。              **
	//**  widthi代表宽度，是最左侧光信标到最右侧光信标的横向距离。                              **
	//**  sidei代表边长，是侧边两个光信标之间的距离。                                           **
	//**  heighti代表高度，是最下侧光信标到最上侧光信标的纵向距离。                             **
	//**  deepi代表深度，是最前侧光信标到最内测光信标的纵深向距离。                             **
	//**  不调用此函数时，使用建模时的理论参数。(单位：毫米)                                    **
	//********************************************************************************************
	void setSize(float widthi, float sidei, float heighti, float deepi);
	//***********************************运行模式设置函数*****************************************
	//**  这个函数用于设定运行模式。                                                            **
	//**  输入PRTTP_TESTMOD代表测试模式，会显示所有测试信息以及处理过程信息。                   **
	//**  输入PRTTP_ANSONLY代表结果测试模式，会显示中间过程的结果。                             **
	//**  输入PRTTP_ERRONLY代表只显示故障模式，会显示必要的故障信息以及耗时。建议在此模式运行。 **
	//**  输入PRTTP_NOTHING代表不显示信息模式，高速运行。                                       **
	//********************************************************************************************
	void setMode(int modeI);
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
	
	void ansToChar(float *in, char *out);
	//*********************************构造函数***************************************************
	//**  此类的构造函数。有一些参数可以选择，使用默认值可以进行测试。                          **
	//**  verticalFOVIn代表了摄像头的竖直视场角（弧度）。                                       **
	//**  horiziontalFOVIn代表了摄像头的水平视场角(弧度)。                                      **
	//**  binTreholdIn代表了二值化时的阈值，用于滤出图像中的亮点光斑(0-255)。                   **
	//**  sizeThreholdIn代表了对于光斑大小的限制，小于这个值直径的亮点不算光斑。(像素单位)      **
	//**  prtTypeI代表了运行模式，含义同setMode()函数。                                         **
	//********************************************************************************************
    recongnizer(float verticalFOVIn = 1.5708, float horizontalFOVIn = 1.5708, float sizeThreholdIn = 5.0,int prtTypeI = 2);


	~recongnizer();
};

