#ifndef ALMANAGER_H
#define ALMANAGER_H
#include <al.h>
#include <alc.h>
#include <iostream>
#include <mgltools.h>

//����ʹ��openAL����3Dʵʱ��Ч�ķ�װ��
//���openAL�Ĺ��ܽ����˼򻯷�װ
//ÿ����Դֻ֧��һ��wav�ļ��Ĳ��ţ�����Ҳ���ߣ����ǽӿ�ʮ�ּ��
//�����Բ���24·3D��Ƶ
//�����趨��λ�ú��������������������������
//���ߣ�yinan Miao from Beihang Univ
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645



using namespace std;

#define MAX_BUFFER_SIZE 24//���֧��24·�������޸�

//---------------------------------------------------------------
//WAV����������أ��Ӷ��޹ػ�alut����������ģ��
struct WAVE_Data {//Wav�ļ�������ģ��
    char subChunkID[4]; //should contain the word data
    long subChunk2Size; //Stores the size of the data block
};

struct WAVE_Format {//wav�ļ����ݲ�������
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct RIFF_Header {//RIFF���׼ģ��
    char chunkID[4];
    long chunkSize;//size not including chunkSize or chunkID
    char format[4];
};
//----------------------------------------------------------------

class alManager
{
private:
    ALCdevice *device;//�豸ָ��
    ALCcontext *context;//������ָ��
    ALCboolean bEAX;//������Ч��չ��������׼��
    ALuint buffers[MAX_BUFFER_SIZE+1];//���ڴ�����Ƶ���ݵĻ���ID
    ALuint sources[MAX_BUFFER_SIZE+1];//���ڴ�����ԴID
    bool loadWavFile(const string filename, ALuint buffer,ALsizei* size, ALsizei* frequency,ALenum* format);//�����ļ�������wav�ļ���ָ����bufferID���С�

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
    alManager();//���캯����û��ʵ�����á�����Ϊ��openAL���趨�У�ֻ������һ��OPENAL�������ģ�����ڱ�����趨�в����ظ�������ʼ����
    void loadWav2Buffer(ALuint bufferI,const char *fileName);//�����ļ�������ָ��wav�ļ������Ұ󶨵�ָ��ID��buffer����Դ��
    void setListenerPos(vertex pos,vertex vel,vertex at,vertex up);//�������ߵ�λ�á��ٶȡ������������Լ�ͷ������������
    void setSourcePos(int sI,vertex pos,vertex vel,vertex at);//����ָ��ID��Դ��λ�á��ٶȡ�����������
    void setSourcePhy(int sI,float Gain,float maxDis,float halfDistance,float rollOff = 1.0);//����ָ��ID��Դ���������������ϵ������󴫲����롢˥�������Լ���˥������ʵ���ź�= (GAIN-20*log10(1+ROLLOFF*(����-��˥����)/��˥����))ԭʼ�ź�
    void setSourceLoop(int sI,int isLoop);//����ָ��ID��Դ�Ƿ�ѭ����Ĭ�ϲ�ѭ����
    void play(int sI);//����ָ��ID����Դ
    int getSourceId(int sI);//�õ�ָ��ID��ʵ����ԴID����openAL�����ѿ���ʹ��������Զ������
    void init();//��ʼ��openAL
    void end();//�ر�openAL
};

#endif // ALMANAGER_H


