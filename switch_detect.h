#ifndef SWITCH_DETECT_H
#define SWITCH_DETECT_H
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define pi 3.1415926
#define a1 100
#define a2 225
#define r1 4
#define r2 45
#define f1 200
#define f2 590
#define thresh_num 30
#define pan 2900

//线段结构体
struct Lines{
    int seq; //序列号
    double length; //长度
    int theta; //与x轴的角度
    double rho; //原点到直线的距离
};


Lines vec_init(Vec4f l, int i);
void LSD(string pic_path);
void InsertSortByRho(vector<Lines> &l);//对rho进行排序
void InsertSortByLength(vector<Lines> &l);//对length进行排序
int select();//对直线进行筛选，选出符合铁轨边缘的直线
int have_ballas();//对有砟和无砟进行检测
double getlength(Vec4f l);//得到直线长度
int gettheta(Vec4f l);  //得到直线与x轴的角度
double getrho(Vec4f l);//得到原点到直线的距离
int getseq(int i);//得到直线的序列号
void clear_arrays();//在一次检测后清空各个数组，用于下一次检测


#endif // SWITCH_DETECT_H
