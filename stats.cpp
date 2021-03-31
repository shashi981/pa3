#include "stats.h"


#include "stats.h"

stats::stats(PNG & im)
{
    for(unsigned i = 0; i < im.width(); i++)
    {
        vector<long> statR, statB, statG, sqrR, sqrB, sqrG;
        for(unsigned j = 0; j < im.height(); j++)
        {        
            long sum_statr = 0, 
            sum_statb = 0, 
            sum_statg = 0, 
            sum_sqr = 0, 
            sum_sqb = 0, 
            sum_sqg = 0;  
            RGBAPixel *pix = im.getPixel(i,j);
            sum_statr += pix->r;
            sum_statb += pix->b;
            sum_statg += pix->g;
            sum_statr += pow(pix->r,2);
            sum_sqb += pow(pix->b,2);
            sum_sqg += pow(pix->g,2);
            statR.push_back(sum_statr);
            statB.push_back(sum_statb);
            statG.push_back(sum_statg);
            sqrR.push_back(sum_sqr);
            sqrB.push_back(sum_sqb);
            sqrG.push_back(sum_sqg);
        }
        sumRed.push_back(statR);
        sumBlue.push_back(statB);
        sumGreen.push_back(statG);
        sumsqRed.push_back(sqrR);
        sumsqBlue.push_back(sqrB);
        sumsqGreen.push_back(sqrG);
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h)
{
    long sum = 0;
    switch(channel)
    {
        case 'b':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumBlue[i][j];
                }
            }
            break;
        case 'g':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumGreen[i][j];
                }
            }
            break;
        case 'r':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumRed[i][j];
                }
            }
            break;    
        default: break;
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h)
{
    long sum = 0;
    switch(channel)
    {
        case 'b':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumsqBlue[i][j];
                }
            }
            break;
        case 'g':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumsqGreen[i][j];
                }
            }
            break;
        case 'r':
            for(int i = ul.first; i < w + ul.first; i++)
            {
                for(int j = ul.second; j < h + ul.second; j++)
                {
                    sum += sumsqRed[i][j];
                }
            }
            break;
        default: break;
    }
    return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h)
{
    RGBAPixel mean = getAvg(ul,w,h);
    double r = getSumSq('r', ul, w, h)-pow(getSum('r',ul,w,h),2)/(w*h);
    double g = getSumSq('g', ul, w, h)-pow(getSum('g',ul,w,h),2)/(w*h);
    double b = getSumSq('b', ul, w, h)-pow(getSum('b',ul,w,h),2)/(w*h);

    return (r+g+b);
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h)
{
    RGBAPixel *pixel = new RGBAPixel(getSum('r',ul,w,h)/(w*h),getSum('g',ul,w,h)/(w*h),getSum('b',ul,w,h)/(w*h));
    return *pixel;
}
