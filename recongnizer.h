#pragma once


#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>


//����ʶ��Ŀ���ű����
//�����漰��������ѧ�Լ��ṹ������˲������㷨�в������ע��
//����ʶ����ɫ��ָ���ṹ����Ϣ
//���ߣ�yinan Miao from Beihang Univ.
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645

#define ANSTYPE_ALL 0//�õ������е�λ������
#define ANSTYPE_POS 1//ֻ�õ��˺������������
#define ANSTYPE_NON 2//û��ʶ���κ�Ŀ��

#define PRTTP_TESTMOD 0//����ģʽ����ʾ������Ϣ
#define PRTTP_ANSONLY 1//ֻ��ʾ���ģʽ
#define PRTTP_ERRONLY 2//ֻ��ʾ������Ϣģʽ
#define PRTTP_NOTHING 3//����ʾģʽ


using namespace std;


class recongnizer
{
private:
    float sizeThrehold;//ʶ��Ŀ����ֱ���ߴ���ֵ
    float verticalSizeRate;//����ߴ����(һ���ߴ�����)
    float horizontalFOV;//�����ӽ�
    float verticalFOV;//�����ӽ�
    float standardSlip;//��׼�໬��(һ���ߴ�����)
    float standardPitch;//��׼������(һ���ߴ�����)
    float slipA;//һЩ�ڹ��ߴ�����
    float slipB;//һЩ�ڹ��ߴ�����
    float pitchA;//һЩ�ڹ��ߴ�����
    float pitchB;//һЩ�ڹ��ߴ�����
    float width;//���(�ߴ�����)
    int printType;//������ͣ����ڿ������
private:
	float ParallelScaleRate4P(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,float scale); 
	void calAB();
public:
	//***************************************ͼ���������*****************************************
	//**  �����������������ͼ���е�ʶ��Ŀ���λ�ˡ�                                            **
	//**                                                                                        **
	//**  pSrcImgΪͼ�����룬Ҫ�����й������㡣                                             **
	//**                                                                                        **
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
	//**    �����ǡ��໬�ǡ���ת�Ƿ�ӳ��Ŀ�����AUV�Ľ���̬����Ŀ�����泯������ͷΪ���������˼ **
	//**  ����������Ŀ��������ʱ��ֻҪĿ����ȫ����������ͷ������û�й�ת����������ͷ�п��Կ���û**
	//**  ���κη���任��ʶ��Ŀ������ͼ��ʱ����ʱ�ĸ����ǡ��໬�ǡ���ת�ǽ�Ϊ0���ڴ˻����ϣ��� **
	//**  ��������̬�ı仯����ʶ��Ϊ�����ǡ��໬�ǡ���ת�ǡ����Ŀ�����ֱ��λ���Լ�ˮƽ��λ�ã�**
	//**  �����ǡ��໬�ǡ���ת�ǣ����Ի���Ϊ��׼����Ը����ǡ���Բ໬���Լ���Թ�ת�ǡ�        **
	//**	���pos�е�ֵ��ȫ��ӳ�������ɶȵ���̬��Ϣ��                                         **
	//**                                                                                        **
	//**  ansTypeΪ���������������ansType��ֵΪANSTYPE_ALL����ʶ����ȫ�����ɶ���Ϣ��pos[0**
	//**  -6]ȫ������Ч�����ansType��ֵΪANSTYPE_POS����ֻʶ����λ�ý���Ϣ��ֻ��pos[0]��pos[1**
	//**  ]����Ч�ġ����ansType��ֵΪANSTYPE_NON����û��ʶ��Ŀ�꣬pos[]������Ч��Ϣ��        **
	//********************************************************************************************
	void run(IplImage *pSrcImg3, float *pos, int &ansType);
	//*************************************Ŀ��������ú���***************************************
	//**  ���������������ʶ��Ŀ���һЩ���β����������������ȷ���޷�����ʶ��ġ�              **
	//**  widthi�����ȣ����������ű굽���Ҳ���ű�ĺ�����롣                              **
	//**  sidei����߳����ǲ���������ű�֮��ľ��롣                                           **
	//**  heighti����߶ȣ������²���ű굽���ϲ���ű��������롣                             **
	//**  deepi������ȣ�����ǰ����ű굽���ڲ���ű����������롣                             **
	//**  �����ô˺���ʱ��ʹ�ý�ģʱ�����۲�����(��λ������)                                    **
	//********************************************************************************************
	void setSize(float widthi, float sidei, float heighti, float deepi);
	//***********************************����ģʽ���ú���*****************************************
	//**  ������������趨����ģʽ��                                                            **
	//**  ����PRTTP_TESTMOD�������ģʽ������ʾ���в�����Ϣ�Լ����������Ϣ��                   **
	//**  ����PRTTP_ANSONLY����������ģʽ������ʾ�м���̵Ľ����                             **
	//**  ����PRTTP_ERRONLY����ֻ��ʾ����ģʽ������ʾ��Ҫ�Ĺ�����Ϣ�Լ���ʱ�������ڴ�ģʽ���С� **
	//**  ����PRTTP_NOTHING������ʾ��Ϣģʽ���������С�                                       **
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
	//*********************************���캯��***************************************************
	//**  ����Ĺ��캯������һЩ��������ѡ��ʹ��Ĭ��ֵ���Խ��в��ԡ�                          **
	//**  verticalFOVIn����������ͷ����ֱ�ӳ��ǣ����ȣ���                                       **
	//**  horiziontalFOVIn����������ͷ��ˮƽ�ӳ���(����)��                                      **
	//**  binTreholdIn�����˶�ֵ��ʱ����ֵ�������˳�ͼ���е�������(0-255)��                   **
	//**  sizeThreholdIn�����˶��ڹ�ߴ�С�����ƣ�С�����ֱֵ�������㲻���ߡ�(���ص�λ)      **
	//**  prtTypeI����������ģʽ������ͬsetMode()������                                         **
	//********************************************************************************************
    recongnizer(float verticalFOVIn = 1.5708, float horizontalFOVIn = 1.5708, float sizeThreholdIn = 5.0,int prtTypeI = 2);


	~recongnizer();
};

