#ifndef VERTEXTRANSFORM_H
#define VERTEXTRANSFORM_H
#include <mgltools.h>
//���ڽ�����������ϵ�����Ҷ�����ϵ���б任����
//�����ڽ��任������ϵ�еĵ㣨��������������ת��Ϊĸ����ϵ�е�����
//�����ڼ������λ������ʱʮ�ַ���
//���ߣ�yinan Miao from Beihang Univ.
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�
//QQ:1131042645

class vertexTransform
{
private:
    vertex x;//X��
    vertex y;//Y��
    vertex z;//Z��
public:
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
    vertexTransform();
    void showBase();//��ʾ���е���
    void init();//��ʼ������ϵ
    void clear();//��������ϵ
    void rotate(vertex axis,float ang);//Χ��ת��axis��ת����ϵang�ȡ�
    void move(vertex in);//�ƶ�����ϵ
    vertex get(vertex in);//�õ���ǰ����ϵ��in�������㣩��ĸ����ϵ�µ����ꡣ
};

#endif // VERTEXTRANSFORM_H
