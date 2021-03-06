﻿/**
 *   This source file contains dehazing construnctor, destructor, and 
 *   dehazing functions. 

 *   The detailed description of the algorithm is presented
 *   in "http://mcl.korea.ac.kr/projects/dehazing".  

 *   Last updated: 2013-02-06
 *   Author: Jin-Hwan, Kim.
 */
#include "dehazing.h"

dehazing::dehazing(){}

dehazing::dehazing(int nW, int nH, bool bPrevFlag, bool bPosFlag)
{
    m_nWid = nW;
    m_nHei = nH;

    // Flags for temporal coherence & post processing
    m_bPreviousFlag = bPrevFlag;
    m_bPostFlag = bPosFlag;

    m_fLambda1 = 5.0f;
    m_fLambda2 = 1.0f;

    // Transmission estimation block size
    m_nTBlockSize = 40;

    // Guided filter block size, step size(sampling step), & LookUpTable parameter
    m_nGBlockSize = 40;
    m_nStepSize = 2;
    m_fGSigma = 10.0f;

    // to specify the region of atmospheric light estimation
    m_nTopLeftX = 0;
    m_nTopLeftY = 0;
    m_nBottomRightX = m_nWid;
    m_nBottomRightY = m_nHei;

    m_pfSmallTransP = new float [320*240]; // previous trans. (video only)
    m_pfSmallTrans = new float [320*240];  // init trans.
    m_pfSmallTransR = new float [320*240]; // refined trans.
    m_pnSmallYImg = new int [320*240];    
    m_pnSmallYImgP = new int [320*240];
    m_pfSmallInteg = new float[320*240];
    m_pfSmallDenom = new float[320*240];
    m_pfSmallY = new float[320*240];

    m_pfTransmission = new float [m_nWid*m_nHei];
    m_pfTransmissionR = new float[m_nWid*m_nHei];
    m_pfTransmissionP = new float[m_nWid*m_nHei];
    m_pnYImg = new int [m_nWid*m_nHei];
    m_pnYImgP = new int [m_nWid*m_nHei];
    m_pfInteg = new float[m_nWid*m_nHei];
    m_pfDenom = new float[m_nWid*m_nHei];
    m_pfY  = new float[m_nWid*m_nHei];

    m_pfSmallPk_p = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfSmallNormPk = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfPk_p = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfNormPk = new float[m_nGBlockSize*m_nGBlockSize]; 
    m_pfGuidedLUT = new float[m_nGBlockSize*m_nGBlockSize]; 
}

dehazing::dehazing(int nW, int nH, int nTBlockSize, bool bPrevFlag, bool bPosFlag, float fL1, float fL2, int nGBlock)
{
    m_nWid = nW;
    m_nHei = nH;

    // Flags for temporal coherence & post processing
    m_bPreviousFlag = bPrevFlag;
    m_bPostFlag = bPosFlag;

    // parameters for each cost (loss cost, temporal coherence cost)
    m_fLambda1 = fL1;
    m_fLambda2 = fL2;

    // block size for transmission estimation
    m_nTBlockSize = nTBlockSize;

    // Guided filter block size, step size(sampling step), & LookUpTable parameter
    m_nGBlockSize = nGBlock;
    m_nStepSize = 2; 
    m_fGSigma = 10.0f;

    // to specify the region of atmospheric light estimation
    m_nTopLeftX = 0;
    m_nTopLeftY = 0;
    m_nBottomRightX = m_nWid;
    m_nBottomRightY = m_nHei;

    m_pfSmallTransP = new float [320*240];
    m_pfSmallTrans = new float [320*240];
    m_pfSmallTransR = new float [320*240];
    m_pnSmallYImg = new int [320*240];
    m_pnSmallYImgP = new int [320*240];

    m_pnSmallRImg = new int [320*240];
    m_pnSmallRImgP = new int [320*240];
    m_pnSmallGImg = new int [320*240];
    m_pnSmallGImgP = new int [320*240];
    m_pnSmallBImg = new int [320*240];
    m_pnSmallBImgP = new int [320*240];

    m_pfSmallInteg = new float[320*240];
    m_pfSmallDenom = new float[320*240];
    m_pfSmallY = new float[320*240];

    m_pfTransmission = new float [m_nWid*m_nHei];
    m_pfTransmissionR = new float[m_nWid*m_nHei];
    m_pfTransmissionP = new float[m_nWid*m_nHei];
    m_pnYImg = new int [m_nWid*m_nHei];
    m_pnYImgP = new int [m_nWid*m_nHei];

    m_pnRImg = new int [m_nWid*m_nHei];
    m_pnGImg = new int [m_nWid*m_nHei];
    m_pnBImg = new int [m_nWid*m_nHei];

    m_pnRImgP = new int [m_nWid*m_nHei];
    m_pnGImgP = new int [m_nWid*m_nHei];
    m_pnBImgP = new int [m_nWid*m_nHei];

    m_pfInteg = new float[m_nWid*m_nHei];
    m_pfDenom = new float[m_nWid*m_nHei];
    m_pfY  = new float[m_nWid*m_nHei];

    m_pfSmallPk_p = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfSmallNormPk = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfPk_p = new float[m_nGBlockSize*m_nGBlockSize];
    m_pfNormPk = new float[m_nGBlockSize*m_nGBlockSize]; 
    m_pfGuidedLUT = new float[m_nGBlockSize*m_nGBlockSize]; 
}

dehazing::~dehazing(void)
{
    if(m_pfSmallTransP != NULL)
        delete [] m_pfSmallTransP;
    if(m_pfSmallTrans != NULL)
        delete [] m_pfSmallTrans;
    if(m_pfSmallTransR != NULL)
        delete [] m_pfSmallTransR;
    if(m_pnSmallYImg != NULL)
        delete [] m_pnSmallYImg;
    if(m_pfSmallY != NULL)
        delete [] m_pfSmallY;
    if(m_pnSmallYImgP != NULL)
        delete [] m_pnSmallYImgP;

    if(m_pnSmallRImg != NULL)
        delete [] m_pnSmallRImg;
    if(m_pnSmallRImgP != NULL)
        delete [] m_pnSmallRImgP;
    if(m_pnSmallGImg != NULL)
        delete [] m_pnSmallGImg;
    if(m_pnSmallGImgP != NULL)
        delete [] m_pnSmallGImgP;
    if(m_pnSmallBImg != NULL)
        delete [] m_pnSmallBImg;
    if(m_pnSmallBImgP != NULL)
        delete [] m_pnSmallBImgP;

    if(m_pfSmallInteg != NULL)
    delete [] m_pfSmallInteg;
    if(m_pfSmallDenom != NULL)
    delete [] m_pfSmallDenom;
    if(m_pfSmallNormPk != NULL)
    delete [] m_pfSmallNormPk;
    if(m_pfSmallPk_p != NULL)
    delete [] m_pfSmallPk_p;
    
    if(m_pnYImg != NULL)
        delete [] m_pnYImg; 
    if(m_pnYImgP != NULL)
        delete [] m_pnYImgP;
    if(m_pnRImg != NULL)
        delete [] m_pnRImg; 
    if(m_pnGImg != NULL)
        delete [] m_pnGImg; 
    if(m_pnBImg != NULL)
        delete [] m_pnBImg; 
    if(m_pnRImgP != NULL)
        delete [] m_pnRImgP; 
    if(m_pnGImgP != NULL)
        delete [] m_pnGImgP; 
    if(m_pnBImgP != NULL)
        delete [] m_pnBImgP; 

    if(m_pfTransmission != NULL)
        delete [] m_pfTransmission;
    if(m_pfTransmissionR != NULL)
        delete [] m_pfTransmissionR;
    if(m_pfTransmissionP != NULL)
        delete [] m_pfTransmissionP;

    if(m_pfInteg != NULL)
        delete [] m_pfInteg;
    if(m_pfDenom != NULL)
        delete [] m_pfDenom;
    if(m_pfY != NULL)
        delete [] m_pfY;
    if(m_pfPk_p != NULL)
        delete [] m_pfPk_p;
    if(m_pfNormPk != NULL)
        delete [] m_pfNormPk;
    if(m_pfGuidedLUT != NULL)
        delete [] m_pfGuidedLUT;

    m_pfSmallTransP = NULL;
    m_pfSmallTrans = NULL;
    m_pfSmallTransR = NULL;
    m_pnSmallYImg = NULL;
    m_pnSmallYImgP = NULL;

    m_pnSmallRImg = NULL;
    m_pnSmallRImgP = NULL;
    m_pnSmallGImg = NULL;
    m_pnSmallGImgP = NULL;
    m_pnSmallBImg = NULL;
    m_pnSmallBImgP = NULL;

    m_pfSmallInteg = NULL;
    m_pfSmallDenom = NULL;
    m_pfSmallY = NULL;

    m_pfTransmission = NULL;
    m_pfTransmissionR = NULL;
    m_pfTransmissionP = NULL;
    m_pnYImg = NULL;
    m_pnYImgP = NULL;

    m_pnRImg = NULL;
    m_pnGImg = NULL;
    m_pnBImg = NULL;

    m_pnRImgP = NULL;
    m_pnGImgP = NULL;
    m_pnBImgP = NULL;

    m_pfInteg = NULL;
    m_pfDenom = NULL;
    m_pfY  = NULL;

    m_pfSmallPk_p = NULL;
    m_pfSmallNormPk = NULL;
    m_pfPk_p = NULL;
    m_pfNormPk = NULL; 
    m_pfGuidedLUT = NULL; 
}

void dehazing::AirlightEstimation(IplImage* imInput)
{
    int nMinDistance = 65536;
    int nDistance;

    int nX, nY;

    int nMaxIndex;
    double dpScore[3];
    double dpMean[3];
    double dpStds[3];

    float afMean[4] = {0};
    float afScore[4] = {0};
    float nMaxScore = 0;

    int nWid = imInput->width;
    int nHei = imInput->height;
    int nStep = imInput->widthStep;

    // 4 sub-block
    IplImage *iplUpperLeft = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 3);
    IplImage *iplUpperRight = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 3);
    IplImage *iplLowerLeft = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 3);
    IplImage *iplLowerRight = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 3);

    IplImage *iplR = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 1);
    IplImage *iplG = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 1);
    IplImage *iplB = cvCreateImage(cvSize(nWid/2, nHei/2),IPL_DEPTH_8U, 1);

    // divide 
    cvSetImageROI(imInput, cvRect(0, 0, nWid/2, nHei/2));
    cvCopyImage(imInput, iplUpperLeft);
    cvSetImageROI(imInput, cvRect(nWid/2+nWid%2, 0, nWid, nHei/2));
    cvCopyImage(imInput, iplUpperRight);
    cvSetImageROI(imInput, cvRect(0, nHei/2+nHei%2, nWid/2, nHei));
    cvCopyImage(imInput, iplLowerLeft);
    cvSetImageROI(imInput, cvRect(nWid/2+nWid%2, nHei/2+nHei%2, nWid, nHei));
    cvCopyImage(imInput, iplLowerRight);

    // compare to threshold(200) --> bigger than threshold, divide the block
    if(nHei*nWid > 200) {
        // compute the mean and std-dev in the sub-block
        // upper left sub-block
        cvCvtPixToPlane(iplUpperLeft, iplR, iplG, iplB, 0);  // cvSplit()

        cvMean_StdDev(iplR, dpMean, dpStds);
        cvMean_StdDev(iplG, dpMean+1, dpStds+1);
        cvMean_StdDev(iplB, dpMean+2, dpStds+2);
        
        // dpScore: mean - std-dev
        dpScore[0] = dpMean[0]-dpStds[0];
        dpScore[1] = dpMean[1]-dpStds[1];
        dpScore[2] = dpMean[2]-dpStds[2];

        afScore[0] = (float)(dpScore[0]+dpScore[1]+dpScore[2]);

        nMaxScore = afScore[0];
        nMaxIndex = 0;

        // upper right sub-block
        cvCvtPixToPlane(iplUpperRight, iplR, iplG, iplB, 0);

        cvMean_StdDev(iplR, dpMean, dpStds);
        cvMean_StdDev(iplG, dpMean+1, dpStds+1);
        cvMean_StdDev(iplB, dpMean+2, dpStds+2);
        
        dpScore[0] = dpMean[0]-dpStds[0];
        dpScore[1] = dpMean[1]-dpStds[1];
        dpScore[2] = dpMean[2]-dpStds[2];

        afScore[1] = (float)(dpScore[0]+dpScore[1]+dpScore[2]);
        
        if(afScore[1] > nMaxScore) {
            nMaxScore = afScore[1];
            nMaxIndex = 1;
        }

        // lower left sub-block
        cvCvtPixToPlane(iplLowerLeft, iplR, iplG, iplB, 0);

        cvMean_StdDev(iplR, dpMean, dpStds);
        cvMean_StdDev(iplG, dpMean+1, dpStds+1);
        cvMean_StdDev(iplB, dpMean+2, dpStds+2);
        
        dpScore[0] = dpMean[0]-dpStds[0];
        dpScore[1] = dpMean[1]-dpStds[1];
        dpScore[2] = dpMean[2]-dpStds[2];

        afScore[2] = (float)(dpScore[0]+dpScore[1]+dpScore[2]);
        
        if(afScore[2] > nMaxScore) {
            nMaxScore = afScore[2];
            nMaxIndex = 2;
        }

        // lower right sub-block
        cvCvtPixToPlane(iplLowerRight, iplR, iplG, iplB, 0);

        cvMean_StdDev(iplR, dpMean, dpStds);
        cvMean_StdDev(iplG, dpMean+1, dpStds+1);
        cvMean_StdDev(iplB, dpMean+2, dpStds+2);
        
        dpScore[0] = dpMean[0]-dpStds[0];
        dpScore[1] = dpMean[1]-dpStds[1];
        dpScore[2] = dpMean[2]-dpStds[2];

        afScore[3] = (float)(dpScore[0]+dpScore[1]+dpScore[2]);

        if(afScore[3] > nMaxScore) {
            nMaxScore = afScore[3];
            nMaxIndex = 3;
        }

        // select the sub-block, which has maximum score
        switch (nMaxIndex)
        {
        case 0:
            AirlightEstimation(iplUpperLeft); break;
        case 1:
            AirlightEstimation(iplUpperRight); break;
        case 2:
            AirlightEstimation(iplLowerLeft); break;
        case 3:
            AirlightEstimation(iplLowerRight); break;
        }
    } else {
        // select the atmospheric light value in the sub-block according to
        // || (I_r(p), I_g(p), I_b(p)) - (255, 255, 255)||
        for(nY = 0; nY < nHei; nY++) {
            for(nX = 0; nX < nWid; nX++) {
                // 255-r, 255-g, 255-b
                nDistance = int(sqrt(float(255-(uchar)imInput->imageData[nY*nStep+nX*3])*float(255-(uchar)imInput->imageData[nY*nStep+nX*3])
                    +float(255-(uchar)imInput->imageData[nY*nStep+nX*3+1])*float(255-(uchar)imInput->imageData[nY*nStep+nX*3+1])
                    +float(255-(uchar)imInput->imageData[nY*nStep+nX*3+2])*float(255-(uchar)imInput->imageData[nY*nStep+nX*3+2])));

                if(nMinDistance > nDistance) {
                    nMinDistance = nDistance;
                    m_anAirlight[0] = (uchar)imInput->imageData[nY*nStep+nX*3];
                    m_anAirlight[1] = (uchar)imInput->imageData[nY*nStep+nX*3+1];
                    m_anAirlight[2] = (uchar)imInput->imageData[nY*nStep+nX*3+2];
                }
            }
        }
    }

    cvReleaseImage(&iplUpperLeft);
    cvReleaseImage(&iplUpperRight);
    cvReleaseImage(&iplLowerLeft);
    cvReleaseImage(&iplLowerRight);

    cvReleaseImage(&iplR);
    cvReleaseImage(&iplG);
    cvReleaseImage(&iplB);
}

void dehazing::RestoreImage(IplImage* imInput, IplImage* imOutput)
{
    int nStep = imInput->widthStep;

    int nX, nY;
    float fA_R, fA_G, fA_B;

    fA_B = (float)m_anAirlight[0];
    fA_G = (float)m_anAirlight[1];
    fA_R = (float)m_anAirlight[2];

    // post processing flag
    if(m_bPostFlag == true) {
        PostProcessing(imInput,imOutput);
    } else {
#pragma omp parallel for
        // (2) I' = (I - Airlight)/Transmission + Airlight
        for(nY=0; nY<m_nHei; nY++) {
            for(nX=0; nX<m_nWid; nX++) { 
                // (3) Gamma correction using LUT
                imOutput->imageData[nY*nStep+nX*3]  = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+0])-fA_B)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_B))];
                imOutput->imageData[nY*nStep+nX*3+1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+1])-fA_G)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_G))];
                imOutput->imageData[nY*nStep+nX*3+2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+2])-fA_R)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_R))];
            }
        }
    }
}

void dehazing::PostProcessing(IplImage* imInput, IplImage* imOutput)
{
    const int nStep = imInput->widthStep;
    const int nNumStep= 10;
    const int nDisPos= 20;

    float nAD0, nAD1, nAD2;
    int nS, nX, nY;
    float fA_R, fA_G, fA_B;

    fA_B = (float)m_anAirlight[0];
    fA_G = (float)m_anAirlight[1];
    fA_R = (float)m_anAirlight[2];

#pragma omp parallel for private(nAD0, nAD1, nAD2, nS)
    for(nY=0; nY<m_nHei; nY++)
    {
        for(nX=0; nX<m_nWid; nX++)
        { 
            // (1)  I' = (I - Airlight)/Transmission + Airlight
            imOutput->imageData[nY*nStep+nX*3]  = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+0])-fA_B)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_B))];
            imOutput->imageData[nY*nStep+nX*3+1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+1])-fA_G)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_G))];
            imOutput->imageData[nY*nStep+nX*3+2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput->imageData[nY*nStep+nX*3+2])-fA_R)/CLIP_Z(m_pfTransmissionR[nY*m_nWid+nX]) + fA_R))];

            // if transmission is less than 0.4, we apply post processing because more dehazed block yields more artifacts
            if( nX > nDisPos+nNumStep && m_pfTransmissionR[nY*m_nWid+nX-nDisPos] < 0.4 )
            {
                nAD0 = (float)((int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3]) - (int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3]));
                nAD1 = (float)((int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3+1]) - (int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3+1]));
                nAD2 = (float)((int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3+2]) - (int)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3+2]));
                
                if(std::max(std::max(abs(nAD0),abs(nAD1)),abs(nAD2)) < 20 
                    &&  abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3+0]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1-nNumStep)*3+0])
                    +abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3+1]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1-nNumStep)*3+1])
                    +abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1)*3+2]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1-nNumStep)*3+2])
                    +abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3+0]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nNumStep)*3+0])
                    +abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3+1]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nNumStep)*3+1])
                    +abs((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos)*3+2]-(uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nNumStep)*3+2]) < 30 )
                {
                    for( nS = 1 ; nS < nNumStep+1; nS++)
                    {
                        imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+0]=(uchar)CLIP((float)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+0])+(float)nS*nAD0/(float)nNumStep);
                        imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+1]=(uchar)CLIP((float)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+1])+(float)nS*nAD1/(float)nNumStep);
                        imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+2]=(uchar)CLIP((float)((uchar)imOutput->imageData[nY*nStep+(nX-nDisPos-1+nS-nNumStep)*3+2])+(float)nS*nAD2/(float)nNumStep);
                    }
                }
            }
        }
    }
}

void dehazing::HazeRemoval(IplImage* imInput, IplImage* imOutput, int nFrame)
{
    if(nFrame == 0)
    {
        IplImage* imAir;
        // initializing
        MakeExpLUT(); 
        GuideLUTMaker(); 
        GammaLUTMaker(0.7f); 

        // specify the ROI region of atmospheric light estimation(optional)
        cvSetImageROI(imInput, cvRect(m_nTopLeftX, m_nTopLeftY, m_nBottomRightX-m_nTopLeftX, m_nBottomRightY-m_nTopLeftY));

        imAir = cvCreateImage(cvSize(m_nBottomRightX-m_nTopLeftX, m_nBottomRightY-m_nTopLeftY),IPL_DEPTH_8U, 3);
        cvCopyImage(imInput, imAir);
        
        AirlightEstimation(imAir);

        // Y value of A (atmosperic light) in YUY Color Space
        m_nAirlight = (((int)(uchar)m_anAirlight[0]*25 + (int)(uchar)m_anAirlight[1]*129 + (int)(uchar)m_anAirlight[2]*66 +128) >>8) + 16;

        cvReleaseImage(&imAir);
        cvResetImageROI(imInput);
    }
    
    // Convert To YUV sapce, we only use Y componment here.
    IplImageToInt(imInput);
   
    // down sampling to fast Transmission estimation
    DownsampleImage();
    
    // trnasmission estimation
    TransmissionEstimation(m_pnSmallYImg,m_pfSmallTrans, m_pnSmallYImgP, m_pfSmallTransP, nFrame, 320, 240);
        
    // store Previous image data for temporal coherent processing
    memcpy(m_pfSmallTransP, m_pfSmallTrans, 320*240);
    memcpy(m_pnSmallYImgP, m_pnSmallYImg, 320*240);
    
    UpsampleTransmission();
    
    /*
    IplImage *test = cvCreateImage(cvSize(320, 240),IPL_DEPTH_8U, 1);
    for(int nK = 0; nK < 320*240; nK++)
        test->imageData[nK] = (uchar)(m_pnSmallYImg[nK]);
    cvNamedWindow("tests");
    cvShowImage("tests", test);
    cvWaitKey(-1);
    */
    FastGuidedFilter();

    // Restore the Hazy Image
    RestoreImage(imInput, imOutput);
}

int* dehazing::GetAirlight()
{
    return m_anAirlight;
}

int* dehazing::GetYImg()
{
    return m_pnYImg;
}

float* dehazing::GetTransmission()
{
    return m_pfTransmissionR;
}

void dehazing::LambdaSetting(float fLambdaLoss, float fLambdaTemp)
{
    m_fLambda1 = fLambdaLoss;
    if(fLambdaTemp>0)
        m_fLambda2 = fLambdaTemp;
    else
        m_bPreviousFlag = false;
}

void dehazing::PreviousFlag(bool bPrevFlag)
{
    m_bPreviousFlag = bPrevFlag;
}

void dehazing::TransBlockSize(int nBlockSize)
{
    m_nTBlockSize = nBlockSize;
}

void dehazing::FilterBlockSize(int nBlockSize)
{
    m_nGBlockSize = nBlockSize;
}

void dehazing::SetFilterStepSize(int nStepSize)
{
    m_nStepSize = nStepSize;
}

void dehazing::AirlightSerachRange(Point pointTopLeft, Point pointBottomRight)
{
    m_nTopLeftX = pointTopLeft.x;
    m_nTopLeftY = pointTopLeft.y;
    m_nBottomRightX = pointBottomRight.x;
    m_nBottomRightY = pointBottomRight.y;
}

void dehazing::FilterSigma(float nSigma)
{
    m_fGSigma = nSigma;
}

bool dehazing::Decision(IplImage* imSrc1, IplImage* imSrc2, int nThreshold)
{
    int nX, nY;
    int nStep;

    int nMAD;

    nMAD = 0;

    nStep = imSrc1->widthStep;
    if(imSrc2->widthStep != nStep)
    {
        cout << "Size of src1 and src2 is not the same" << endl; 
        exit(1);
    }
        
    for(nY=0; nY<m_nHei; nY++)
    {
        for(nX=0; nX<m_nWid; nX++)
        {
            nMAD += abs((int)imSrc1->imageData[nY*nStep+nX*3+2]-(int)imSrc2->imageData[nY*nStep+nX*3+2]);
            nMAD += abs((int)imSrc1->imageData[nY*nStep+nX*3+1]-(int)imSrc2->imageData[nY*nStep+nX*3+1]);
            nMAD += abs((int)imSrc1->imageData[nY*nStep+nX*3+0]-(int)imSrc2->imageData[nY*nStep+nX*3+0]);
        }
    }

    nMAD /= (m_nWid*m_nHei);
    if(nMAD > nThreshold)
        return true;
    else
        return false; 
}
