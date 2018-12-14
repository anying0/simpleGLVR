#include "vertexTransform.h"

vertexTransform::vertexTransform()
{

}

void vertexTransform::showBase()
{
    cout<<"base:"<<endl;
     cout<<"x:"<<x.x<<","<<x.y<<","<<x.z<<endl;
     cout<<"y:"<<y.x<<","<<y.y<<","<<y.z<<endl;
     cout<<"z:"<<z.x<<","<<z.y<<","<<z.z<<endl;
}

void vertexTransform::init()
{
    x.x = 1;
    x.y = 0;
    x.z = 0;
    y.x = 0;
    y.y = 1;
    y.z = 0;
    z.x = 0;
    z.y = 0;
    z.z = 1;
}

void vertexTransform::clear()
{
    x.x = 1;
    x.y = 0;
    x.z = 0;
    y.x = 0;
    y.y = 1;
    y.z = 0;
    z.x = 0;
    z.y = 0;
    z.z = 1;
}

void vertexTransform::rotate(vertex axis, float ang)
{
    vertex a,b,c;
    a = MGLTools::mult(x,axis.x);
    b = MGLTools::mult(y,axis.y);
    c = MGLTools::mult(z,axis.z);
    a = MGLTools::add(a,b);
    a = MGLTools::add(a,c);
    x = MGLTools::rotate(x,a,ang);
    y = MGLTools::rotate(y,a,ang);
    z = MGLTools::rotate(z,a,ang);
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

void vertexTransform::move(vertex in)
{
    vertex a,b,c;
    a = MGLTools::mult(x,in.x);
    b = MGLTools::mult(y,in.y);
    c = MGLTools::mult(z,in.z);
    a = MGLTools::add(a,b);
    a = MGLTools::add(a,c);
    x = MGLTools::add(x,a);
    y = MGLTools::add(y,a);
    z = MGLTools::add(z,a);
}

vertex vertexTransform::get(vertex in)
{
    vertex a,b,c;
    a = MGLTools::mult(x,in.x);
    b = MGLTools::mult(y,in.y);
    c = MGLTools::mult(z,in.z);
    a = MGLTools::add(a,b);
    a = MGLTools::add(a,c);
    return a;
}
