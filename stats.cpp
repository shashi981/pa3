#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){
    for(unsigned x = 0; x < im.width(); x++){
        vector<long> sR, sB, sG, sqR, sqB, sqG;
        for(unsigned y = 0; y < im.height(); y++){        
            long sumr = 0, sumb = 0, sumg = 0, sumsr = 0, sumsb = 0, sumsg = 0;  
            RGBAPixel *pix = im.getPixel(x,y);
            sumr += pix->r;
            sumb += pix->b;
            sumg += pix->g;
            sumsr += pow(pix->r,2);
            sumsb += pow(pix->b,2);
            sumsg += pow(pix->g,2);
            sR.push_back(sumr);
            sB.push_back(sumb);
            sG.push_back(sumg);
            sqR.push_back(sumsr);
            sqB.push_back(sumsb);
            sqG.push_back(sumsg);
        }
        sumRed.push_back(sR);
        sumBlue.push_back(sB);
        sumGreen.push_back(sG);
        sumsqRed.push_back(sqR);
        sumsqBlue.push_back(sqB);
        sumsqGreen.push_back(sqG);
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    long sum = 0;
    switch(channel){
        case 'r':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumRed[x][y];
                }
            }
            break;
        case 'b':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumBlue[x][y];
                }
            }
            break;
        case 'g':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumGreen[x][y];
                }
            }
            break;
        default: break;
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    long sum = 0;
    switch(channel){
        case 'r':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumsqRed[x][y];
                }
            }
            break;
        case 'b':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumsqBlue[x][y];
                }
            }
            break;
        case 'g':
            for(int x = ul.first; x < w + ul.first; x++){
                for(int y = ul.second; y < h + ul.second; y++){
                    sum += sumsqGreen[x][y];
                }
            }
            break;
        default: break;
    }
    return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    RGBAPixel mean = getAvg(ul,w,h);
    double redVar = getSumSq('r', ul, w, h) - pow(getSum('r',ul,w,h),2)/(w*h);
    double greenVar = getSumSq('g', ul, w, h) - pow(getSum('g',ul,w,h),2)/(w*h);
    double blueVar = getSumSq('b', ul, w, h) - pow(getSum('b',ul,w,h),2)/(w*h);

    return (redVar+greenVar+blueVar);
    
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    RGBAPixel *pix = new RGBAPixel(getSum('r',ul,w,h)/(w*h),getSum('g',ul,w,h)/(w*h),getSum('b',ul,w,h)/(w*h));
    return *pix;
}
