/*图像检测部分*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "switch_detect.h"

using namespace cv;
using namespace std;

vector<Vec4f> lines_adv;
vector<Lines> candidate;
vector<vector<Lines>> groups(21);
vector<Lines> tmp;
vector<Lines> edge_fin;
vector<int> direction(181);

int haveballas;
void LSD(string pic_path)
//对图像进行LSD线检测
{
    //freopen("./length_lsd6.txt", "w", stdout);
    Mat image=imread(pic_path,IMREAD_GRAYSCALE);
    Ptr<LineSegmentDetector> ls=createLineSegmentDetector(LSD_REFINE_STD);
    ls->detect(image, lines_adv);
    Mat drawnlines(image);
    ls->drawSegments(drawnlines, lines_adv);
    //imshow("LSD", drawnlines);
    //waitKey();
}


//对直线长度排序
void InsertSortByLength(vector<Lines> &l) {
    for(size_t i = 1; i < l.size(); ++i){
        for(int j = i;j > 0;--j){
            if(l[j].length > l[j - 1].length){
                Lines temp = l[j];
                l[j] = l[j-1];
                l[j-1] = temp;
            }
        }
    }
}

//对原点到直线的距离进行排序
void InsertSortByRho(vector<Lines> &l) {
    for(size_t i = 1;i < l.size();++i){
        for(int j = i;j > 0;--j){
            if(l[j].rho < l[j - 1].rho){
                Lines temp = l[j];
                l[j] = l[j-1];
                l[j-1] = temp;
            }
        }
    }
}

/*对有砟和无砟进行检测*/
int have_ballas() {
    int count = 0;
    int count1 = 0;

    for(size_t i = 0; i<lines_adv.size(); i++)
    {
        Vec4f l =lines_adv[i];
        if(gettheta(l) < 0)
            direction[gettheta(l)+180]++;
        else
            direction[gettheta(l)]++;
    }
    printf("角度\t数量\t\n");
    for(size_t j = 0; j<direction.size(); j++)
    {
        printf("%zu\t%d\t\n",j,direction[j]);
        if (direction[j] > thresh_num)
            count++;
    }
    printf("count = %d\n", count);
    for(size_t j=0; j<direction.size(); j++)
        {
            count1 = count1 + direction[j];
        }
        printf("count = %d\n", count1);
        //if(count > thresh_sum)
        if(count1 > pan)
        {
            haveballas = 1;
            printf("该场景为有砟场景\n");
            //return true;
        }
        else
        {
            haveballas = 0;
            printf("该场景为无砟场景\n");
            //return false;
        }
    return haveballas;
}

/*对直线进行筛选*/
int select() {
    int isswitch = 0;
    for(size_t i=0;i<lines_adv.size();i++)
    {
        Vec4f la=lines_adv[i];
        //初步筛选符合轨道边缘条件的直线
        if ((getlength(la) > a1))
        {
            if(gettheta(la) < 0 && gettheta(la)+180 >= 80 && gettheta(la)+180 <= 100)
                candidate.push_back(vec_init(la, static_cast<int>(i)));
            if(gettheta(la) >=0 && gettheta(la) >= 80 && gettheta(la) <=100)
                candidate.push_back(vec_init(la, static_cast<int>(i)));
        }

    }
    printf("candidate序号\t 长度\t theta\t rho\t\n");
    for (size_t c = 0; c< candidate.size();c++)
    {
        if(candidate[c].theta < 0)
            printf("%d\t%lf\t%d\t%lf\t\n",candidate[c].seq,candidate[c].length,candidate[c].theta + 180,candidate[c].rho);
        else
            printf("%d\t%lf\t%d\t%lf\t\n",candidate[c].seq,candidate[c].length,candidate[c].theta,candidate[c].rho);
    }
    //分组
    for(size_t j = 0; j<candidate.size(); j++)
    {
        Lines l = candidate[j];
        if (l.theta == 80)
            groups[0].push_back(l);
        else if (l.theta == 81)
            groups[1].push_back(l);
        else if (l.theta == 82)
            groups[2].push_back(l);
        else if (l.theta == 83)
            groups[3].push_back(l);
        else if (l.theta == 84)
            groups[4].push_back(l);
        else if (l.theta == 85)
            groups[5].push_back(l);
        else if (l.theta ==86)
            groups[6].push_back(l);
        else if (l.theta == 87)
            groups[7].push_back(l);
        else if (l.theta == 88)
            groups[8].push_back(l);
        else if (l.theta == 89)
            groups[9].push_back(l);
        else if (l.theta == 90)
            groups[10].push_back(l);
        else if (l.theta == 91)
            groups[11].push_back(l);
        else if (l.theta == 92)
            groups[12].push_back(l);
        else if (l.theta == 93)
            groups[13].push_back(l);
        else if (l.theta == 94)
            groups[14].push_back(l);
        else if (l.theta == 95)
            groups[15].push_back(l);
        else if (l.theta == 96)
            groups[16].push_back(l);
        else if (l.theta == 97)
            groups[17].push_back(l);
        else if (l.theta == 98)
            groups[18].push_back(l);
        else if (l.theta == 99)
            groups[19].push_back(l);
        else
            groups[20].push_back(l);
    }
    //对每个分组进行筛选
    for(size_t k = 0; k< groups.size();k++ )
    {
        if (groups[k].size() == 0)
            continue;
        InsertSortByLength(groups[k]);
        if(groups[k][0].length > a2)
            tmp.push_back(groups[k][0]);
        for(size_t p = 1;p<groups[k].size();p++)
            if(groups[k][p].length > a2 && abs(groups[k][p].rho - groups[k][p-1].rho) > r1)
                tmp.push_back(groups[k][p]);
    }

    //对rho增序排序
    InsertSortByRho(tmp);
    //最终筛选
    int z = 0;
    if(haveballas == 0)
    {
        size_t n;
        for( n = 0;n<tmp.size();)
        {
            if(tmp[n].rho<f1)
            {
                n++;
            }
            else if(tmp[n].rho>=f1)
            {
                edge_fin.push_back(tmp[n]);
                break;
            }
         }

         for(size_t m = n+1; m<tmp.size(); m++)
         {
            if(abs(tmp[m].rho - edge_fin[z].rho) > r2 && tmp[m].rho> f1 && tmp[m].rho < f2)
            {
                edge_fin.push_back(tmp[m]);
                z++;
            }
          }
    }
    else
    {
        edge_fin.push_back(tmp[0]);
        for(size_t m = 1; m<tmp.size(); m++)
        {
            if(abs(tmp[m].rho - edge_fin[z].rho) > r2)
            {
                edge_fin.push_back(tmp[m]);
                z++;
            }
        }
    }
    printf("\n");
    printf("edge序号\t长度\ttheta\trho\t\n");
    for (size_t c = 0; c< edge_fin.size();c++)
    {
        if (edge_fin[c].theta < 0)
            printf("%d\t%lf\t%d\t%lf\t\n",edge_fin[c].seq,edge_fin[c].length,edge_fin[c].theta + 180,edge_fin[c].rho);
        else
            printf("%d\t%lf\t%d\t%lf\t\n",edge_fin[c].seq,edge_fin[c].length,edge_fin[c].theta,edge_fin[c].rho);
    }
    //给出判断

    if(edge_fin.size() <= 2)
    {

        printf("图像属于单轨场景\n");
        isswitch = 0;
    }
    else
    {
        printf("图像属于道岔场景\n");
        isswitch = 1;
    }
    return isswitch;
}


//计算直线长度
double getlength(Vec4f l){
    return sqrt(pow((l[0]-l[2]), 2) + pow((l[1]-l[3]),2));
}

//计算直线与x轴的角度
int gettheta(Vec4f l) {
    double arc;
    if (l[0] == l[2])
        return 90;
    else {
        arc = atan((l[1] - l[3]) / (l[0] - l[2]));
        return int(arc * 180 / pi);
    }
}

//计算原点到直线的距离
double getrho(Vec4f l) {
    double k;
    if(l[0] == l[2])
        return l[0];
    else
    {
        k = (l[1] - l[3]) / (l[0] - l[2]);
        return abs(k * l[0] - l[1])/sqrt(1 + pow(k,2));
    }

}

int getseq(int i) {
    return i+1;
}

//结构体数组初始化
Lines vec_init(Vec4f l, int i) {
    Lines line;
    line.length = getlength(l);
    line.seq = getseq(i);
    line.rho = getrho(l);
    line.theta = gettheta(l);
    return line;
}

void clear_arrays()
{
    lines_adv.clear();
    lines_adv.shrink_to_fit();
    candidate.clear();
    candidate.shrink_to_fit();
    for (size_t i = 0; i<groups.size();i++)
        groups[i].clear();
    tmp.clear();
    tmp.shrink_to_fit();
    edge_fin.clear();
    edge_fin.shrink_to_fit();
    for (size_t j = 0; j<direction.size();j++)
        direction[j] = 0;
}
