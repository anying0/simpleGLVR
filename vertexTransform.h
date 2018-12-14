#ifndef VERTEXTRANSFORM_H
#define VERTEXTRANSFORM_H
#include <mgltools.h>
//用于建立基底坐标系，并且对坐标系进行变换的类
//可以在将变换后坐标系中的点（向量）坐标逆向转换为母坐标系中的坐标
//在用于计算相对位置坐标时十分方便
//作者：yinan Miao from Beihang Univ.
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）
//QQ:1131042645

class vertexTransform
{
private:
    vertex x;//X轴
    vertex y;//Y轴
    vertex z;//Z轴
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
    void showBase();//显示所有的轴
    void init();//初始化坐标系
    void clear();//重置坐标系
    void rotate(vertex axis,float ang);//围绕转轴axis旋转坐标系ang度。
    void move(vertex in);//移动坐标系
    vertex get(vertex in);//得到当前坐标系下in向量（点）在母坐标系下的坐标。
};

#endif // VERTEXTRANSFORM_H
