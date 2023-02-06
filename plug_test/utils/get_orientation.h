#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<pair>
using namespace std;

vector<float>crossproduct(float d1,float d2,float d3,float e1,float e2,float e3){
    vector<float>ans;
    ans.push_back(d2*e3-d3*e2);
    ans.push_back(d3*e1-d1*e3);
    ans.push_back(d1*e2-d2*e1);
    return ans;
}

float dotproduct(vector<float>ans,vector<float>ans1){
    float num=ans[0]*ans1[0]+ans[1]*ans1[1]+ans[2]*ans1[2];
    float den=sqrt(ans[0]*ans[0]+ans[1]*ans[1]+ans[2]*ans[2])*sqrt(ans1[0]*ans1[0]+ans1[1]*ans1[1]+ans1[2]*ans1[2]);
    return num/den;
}

pair <float, float> get_orientation(float x1,float y1,float z1,
                                    float x2,float y2,float z2,
                                    float x3,float y3,float z3,
                                    float x4,float y4,float z4){
    float d1=x3-x1;
    float d2=y3-y1;
    float d3=z3-z1;
    float e1=x4-x2;
    float e2=y4-y2;
    float e3=z4-z2;
    vector<float>ans1=crossproduct(d1,d2,d3,e1,e2,e3);
    float a1 = 0, b1 = 0, c1 = 0, a2 = 1, b2 = 0, c2 = 0, a3 = 1, b3 = 1, c3 = 0, a4 = 0, b4 = 1, c4 = 0;
    float f1=a3-a1;
    float f2=b3-b1;
    float f3=c3-c1;
    float g1=a4-a2;
    float g2=b4-b2;
    float g3=c4-c2;
    vector<float>ans2=crossproduct(f1,f2,f3,g1,g2,g3);
    float h1 = 0, i1 = 0, j1 = 0, h2 = 1, i2 = 0, j2 = 0, h3 = 1, i3 = 0, j3 = 1, h4 = 0, i4 = 0, j4 = 1;
    float k1=h3-h1;
    float k2=i3-i1;
    float k3=j3-j1;
    float l1=h4-h2;
    float l2=i4-i2;
    float l3=j4-j2;
    vector<float>ans3=crossproduct(k1,k2,k3,l1,l2,l3);

    float cosine1 = dotproduct(ans1,ans2);
    float cosine2 = dotproduct(ans1,ans3)
    // cout<<cosine<<endl;
    float result1=acos(cosine1);
    float result2=acos(cosine2);
    return(make_pair(result1*180/3.1415, result2*180/3.1415));
}
