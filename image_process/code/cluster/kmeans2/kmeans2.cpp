#include "cv.h"
#include "highgui.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "file_ops.h"

using namespace std;

template<typename T>T sqr(T x){return x*x;}
CvMat* mat=0;
IplImage* image=0;
IplImage *histimg=cvCreateImage(cvSize(256,300),IPL_DEPTH_8U,3);
IplImage* DrawHist(map<int,int>colornum,int n);
map<int,int> Fine(CvMat* mat1u);

//CvMat* Merge(CvMat* Fine,int FineNum);
int main(int argc, char *argv[])
{
    std::vector<std::string> paths; 
    std::string root_dir;

    if (argc == 2) 
        root_dir = std::string(argv[1]);

    // Get files' path and number in the "root_dir" dirctory
    std::size_t imgNum = FileOps::get_image_paths(root_dir, paths, false);

    for (std::size_t idx = 0; idx < imgNum; idx++) {

        image=cvLoadImage(paths[idx].c_str(), 1);
        cvNamedWindow("Show",1);
        cvShowImage("Show",image);

        IplImage *hist0=0;//cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
        IplImage *hist1=0;//cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
        CvMat* r=cvCreateMat(image->height,image->width,CV_8UC1);
        CvMat* g=cvCreateMat(image->height,image->width,CV_8UC1);
        CvMat* b=cvCreateMat(image->height,image->width,CV_8UC1);
        CvMat* rgb=cvCreateMat(image->height,image->width,CV_32FC3);
        CvMat* result=cvCreateMat(image->height,image->width,CV_32FC3);
        CvMat* result00=cvCreateMat(image->height,image->width,CV_8UC3);
        cvConvertScale(image,rgb,1.0/255);
        cvSplit(image,r,g,b,NULL);
        map<int,int>fine_r=Fine(r);
        map<int,int>fine_g=Fine(g);
        map<int,int>fine_b=Fine(b);

        int FineNum=fine_r.size()*fine_g.size()*fine_b.size();
        cout<<"FineNum: "<<FineNum<<endl;
        CvMat* Fine=cvCreateMat(1,FineNum,CV_32FC3);
        //vector<CvScalar>avg;////////////////////////////
        int i=0;
        for(map<int,int>::iterator it0=fine_r.begin();it0!=fine_r.end();it0++){
            for(map<int,int>::iterator it1=fine_g.begin();it1!=fine_g.end();it1++){
                float* f=(float*)(Fine->data.ptr);
                for(map<int,int>::iterator it2=fine_b.begin();it2!=fine_b.end();it2++)
                {
                    f[3*i]=(float)it0->first/255;
                    f[3*i+1]=(float)it1->first/255;
                    f[3*i+2]=(float)it2->first/255;
                    i++;
                }
            }
        }

        //CvMat* Fine2=0;
        //Fine2=Merge(Fine,FineNum);
            vector<pair<int,int>>merge;
        float* f1=(float*)(Fine->data.ptr);
        float* f2=(float*)(Fine->data.ptr);
        for(int m=0;m<FineNum;m++)
        {
            
            for(int n=m+1;n<FineNum;n++)
            {
                float d=sqr(f1[3*m]-f2[3*n])+sqr(f1[3*m+1]-f2[3*n+1])+sqr(f1[3*m+2]-f2[3*n+2]);
                if(d<=0.1){
                    merge.push_back(make_pair(m,n));
                }

            }
        }
        cout<<"merge:"<<merge.size()<<endl;
        //Fine1中存储把颜色距离平方小于0.05的元素合并后的结果，结果存在第一个元素位置，第二个位置置零
        CvMat* Fine1=cvCreateMat(1,FineNum,CV_32FC3);
        float* F1=(float*)(Fine1->data.ptr);
        for(vector<pair<int,int>>::iterator iter=merge.begin();iter!=merge.end();iter++)
        {
            if(F1[3*(iter->first)]!=0)
            {
                F1[3*(iter->first)]=(f1[3*(iter->first)]+f1[3*(iter->second)])/2;
                F1[3*(iter->first)+1]=(f1[3*(iter->first)+1]+f1[3*(iter->second)+1])/2;
                F1[3*(iter->first)+2]=(f1[3*(iter->first)+2]+f1[3*(iter->second)+2])/2;
                F1[3*(iter->second)]=0;
                F1[3*(iter->second)+1]=0;
                F1[3*(iter->second)+2]=0;
                f1[3*(iter->second)]=0;
                f1[3*(iter->second)+1]=0;
                f1[3*(iter->second)+2]=0;
            }
            else
                {
                    F1[3*(iter->first)]=f1[3*(iter->first)];
                    F1[3*(iter->first)+1]=f1[3*(iter->first)+1];
                    F1[3*(iter->first)+2]=f1[3*(iter->first)+2];
                }
        }
        
        //求Fine1中非零元素个数
        int no=0;
        for(int m=0;m<FineNum;m++)
        {
            if(F1[m]!=0)
                no++;
        }
        cout<<"Not Zore Num:"<<no<<endl;
        //Fine2中存储merge后Fine1中的非零元素
        CvMat* Fine2=cvCreateMat(1,no*3,CV_32FC1);
        float* F2=(float*)(Fine2->data.ptr);
        for(int m=0;m<FineNum;m++)
        {
            if(F1[m]!=0)
            {
                *F2=F1[m];
                F2++;
            }

        }
        cout<<"F2_Width: "<<Fine2->width<<endl;

        
        //int no=Fine2->width;

        CvMat* id =cvCreateMat(image->height,image->width,CV_32SC1);
        for(int r=0;r<rgb->rows;r++){
            int* idx=(int*)(id->data.ptr+r*id->step);
            float* rgbData=(float*)(rgb->data.ptr+r*rgb->step);
            for(int c=0;c<rgb->cols;c++,idx++){
                
                float D=200000;
                int t=0;
                float* f=(float*)(Fine2->data.ptr);
                for(int i=0;i<no;i++){
                    float dist=sqr(rgbData[3*c]-f[3*i])+sqr(rgbData[3*c+1]-f[3*i+1])+sqr(rgbData[3*c+2]-f[3*i+2]);
                    if(dist<D){
                        D=dist;
                        t=i;
                    }
                    *idx=t;
                }

            }
        }
        //vector<CvScalar>avg1;//////////////////////
        CvMat *colorNum= cvCreateMat(1,no, CV_32SC1);
        cvZero(Fine2);
        cvZero(colorNum);

        float* f=(float*)(Fine2->data.ptr);
        
        int* num1=(int*)(colorNum->data.ptr);
        for(int r=0;r<rgb->rows;r++){
            int* idx=(int*)(id->data.ptr+r*id->step);
            float* rgbData=(float*)(rgb->data.ptr+r*rgb->step);
            for(int c=0;c<rgb->cols;c++){

                f[3*idx[c]]+=rgbData[3*c];
                f[3*idx[c]+1]+=rgbData[3*c+1];
                f[3*idx[c]+2]+=rgbData[3*c+2];
                num1[*idx]++;
            }
        }
        for(int i=0;i<no;i++)
        {
            f[3*i]/=num1[i];
            f[3*i+1]/=num1[i];
            f[3*i+2]/=num1[i];
        }
        for(int r=0;r<rgb->rows;r++){
            int* idx=(int*)(id->data.ptr+r*id->step);
            float* resultData=(float*)(result->data.ptr+r*result->step);
            for(int c=0;c<rgb->cols;c++,idx++)
            {
                //CvScalar avg1;
                resultData[3*c]=f[3*(*idx)];
                resultData[3*c+1]=f[3*(*idx)+1];
                resultData[3*c+2]=f[3*(*idx)+2];
                //cvSet2D(result,r,c,avg1);
            }
        }
        //cvConvertScale(result,result00,255);
        
        cvNamedWindow("Result",1);
        cvShowImage("Result",result);
        //cvSaveImage("D:/test/out_C-mean量化颜色(峰&谷，直接合并)+(d小于0.1)merge颜色/9.jpg",result00);
        cvWaitKey(0);
    }

    return 0;
}

map<int,int> Fine(CvMat* mat1u)
{
    map<int,int>colornum1;
    IplImage *hist0=0;//cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
    IplImage *hist1=0;//cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
    for(int i=0;i<mat1u->rows;i++)
    {
        for(int j=0;j<mat1u->cols;j++)
        {
            int color=(int)(cvGetReal2D(mat1u,i,j));
            ++colornum1[color];
        }
    }

    int n=colornum1.size();
    /*
    colornum1[0]=0;
    colornum1[1]=0;
    colornum1[2]=0;
    colornum1[n-3]=0;
    colornum1[n-1]=0;
    colornum1[n-2]=0;
    */
    cout<<"ColorNum:"<<n<<endl;
    hist0=DrawHist(colornum1,n);
    // cvNamedWindow("Hist0",1);
    // cvShowImage("Hist0",hist0);
    //平滑
    vector<pair<int, int>> num1;
    for( map<int,int>::iterator iter=colornum1.begin();iter!=colornum1.end();iter++)
    {
        num1.push_back(pair<int,int>(iter->first,iter->second));
    }


    for(vector<pair<int,int>>::size_type i=3;i!=n-4;i++)
    {
        int n=(int)((num1[i-3].second+num1[i-2].second+num1[i-1].second+num1[i].second+num1[i+1].second+num1[i+2].second+num1[i+3].second)/7);
        colornum1[num1[i].first]=n;
    }
    hist1=DrawHist(colornum1,n);

    
    num1.clear();
    for( map<int,int>::iterator iter=colornum1.begin();iter!=colornum1.end();iter++)
    {
        num1.push_back(pair<int,int>(iter->first,iter->second));
    }
    map<int,int>merge1;
    map<int,int>merge2;
    int j=0;
    for(vector<pair<int,int>>::size_type i=3;i!=n-4;i++)
    {
        if((num1[i-2].second<num1[i-1].second&&num1[i-1].second<num1[i].second 
            && num1[i].second>num1[i+1].second&& num1[i+1].second>num1[i+2].second
            ||num1[i-2].second>num1[i-1].second&&num1[i-1].second>num1[i].second 
            && num1[i].second<num1[i+1].second&& num1[i+1].second<num1[i+2].second)
            &&num1[i-1].second!=0&&num1[i-2].second!=0&&num1[i-3].second!=0
            &&num1[i+1].second!=0&&num1[i+2].second!=0&&num1[i+3].second!=0)
            {
                        
                merge1[i]=num1[i].second;
                cout<<num1[i].first<<": "<<num1[i].second<<endl;
            
                
            }
    }
    
////////////////////////////////////////////////////////////////////////////////////
    //同一通道上颜色距离小于32的删除

    vector<pair<int, int>> num3;
    for( map<int,int>::iterator iter=merge1.begin();iter!=merge1.end();iter++)
    {
        num3.push_back(pair<int,int>(iter->first,iter->second));
    }

    vector<pair<int,int>>::iterator it3,it2;
    it2=num3.begin();
    it3=num3.begin();
    ++it3;
    while(it3!=num3.end())
    {
        if(abs(it2->first-it3->first)<32)
        {
            it3=num3.erase(it3);
        }
        else
        {
            it2=it3;
            it3++;
        }
    }


    int m3=num3.size();
    cout<<"merge color num:"<<m3<<endl;
    
    for(vector<pair<int,int>>::size_type i=0;i!=m3;i++)
    {
        cout<<num3[i].first<<endl;
        merge2[num3[i].first]=num3[i].second;
    }
    
/////////////////////////////////////////////////////////////////////////////////////////////
    for( map<int,int>::iterator iter=merge2.begin();iter!=merge2.end();iter++)
    {
        int x=iter->first;
        double y=iter->second;
        int h=(int)(300-0.1*y);
        cvCircle(hist1,cvPoint(x,h),1,CV_RGB(255,0,0),-1);
    }

    // cvNamedWindow("Hist1",1);
    // cvShowImage("Hist1",hist1);
    cvWaitKey(0);
    
    return merge2;
}

IplImage* DrawHist(map<int,int>colornum,int n)
{
    cvZero(histimg);
    int bin_w=1;
    
    double bin_h=(double)300/3000;

    for(map<int,int>::iterator it=colornum.begin();it!=colornum.end();it++)
    {
         double val =(double) (it->second*bin_h );
        CvScalar color = CV_RGB(255,255,0); //(hsv2rgb(i*180.f/hdims);
        cvLine( histimg, cvPoint(it->first*bin_w,300),
            cvPoint((it->first)*bin_w,(int)(300-val)),color, 1, 8, 0 );
    }
    return histimg;
    
}