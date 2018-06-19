/**
 *   This source file contains additional function for dehazing algorithm.
 *
 *   The detailed description of the algorithm is presented
 *   in "http://mcl.korea.ac.kr/projects/dehazing".  
 *
 *   Last updated: 2013-02-06
 *   Author: Jin-Hwan, Kim.
 */

#include "dehazing.h"

/** 
 *   \brief: Convert RGB to YUV(Y)
 * 
 *   \param: 
 *       imInput - input RGB IplImage
 *   \return:
 *       m_pnYImg - output Y, integer array
 * 
 *   faster method:   
 *      Y = 0.299 R + 0.587 G + 0.114 B 
 *      because: 19595 / (2 >> (16-1)) = 0.299
 *      Y = (19595 * (uchar)imInput->imageData[nY*nStep+nX*3+2]
 *         + 38469 * (uchar)imInput->imageData[nY*nStep+nX*3+1] 
 *         + 7471 * (uchar)imInput->imageData[nY*nStep+nX*3]) >> 16;
 *       
*/
void dehazing::IplImageToInt(IplImage* imInput)
{
    int nY, nX;
    int nStep;

    nStep = imInput->widthStep;

    for(nY=0; nY<m_nHei; nY++) {
        for(nX=0; nX<m_nWid; nX++) {
            m_pnYImg[nY*m_nWid+nX] =
                (19595 * (uchar)imInput->imageData[nY*nStep+nX*3+2]
            + 38469 * (uchar)imInput->imageData[nY*nStep+nX*3+1] 
            + 7471 * (uchar)imInput->imageData[nY*nStep+nX*3]) >> 16;
        }
    }
}

/* 
    \brief: Convert 2D array to 1D arrays (3 arrays)

    \param: 
        imInput - input IplImage
    \return:
        m_pnRImg - output integer arrays R
        m_pnGImg - output integer arrays G
        m_pnBImg - output integer arrays B
*/
void dehazing::IplImageToIntColor(IplImage* imInput)
{
    int nY, nX;
    int nStep;

    nStep = imInput->widthStep;

    for(nY=0; nY<m_nHei; nY++) {
        for(nX=0; nX<m_nWid; nX++) {
            m_pnBImg[nY*m_nWid+nX] = (uchar)imInput->imageData[nY*nStep+nX*3];
            m_pnGImg[nY*m_nWid+nX] = (uchar)imInput->imageData[nY*nStep+nX*3+1];
            m_pnRImg[nY*m_nWid+nX] = (uchar)imInput->imageData[nY*nStep+nX*3+2];
        }
    }
}

/* 
    \brief: Downsample the image to fixed sized image (320 x 240)

    \param:(hidden) 
        m_pnYImg - input Y Image, returned by IplImageToInt(). 
    \return:
        m_pnSmallYImg - output down sampled image
*/

void dehazing::DownsampleImage()
{
    int nX, nY;

    float fRatioY, fRatioX;
    fRatioX = (float)m_nWid/(float)320;
    fRatioY = (float)m_nHei/(float)240;

    for(nY=0; nY<240; nY++) {
        for(nX=0; nX<320; nX++) {
            m_pnSmallYImg[nY*320+nX] = m_pnYImg[(int)(nY*fRatioY)*m_nWid+(int)(nX*fRatioX)];
        }
    }
}

/* 
    \brief: Downsample the image to fixed sized image (320 x 240) ** for color

    \param:(hidden) 
        m_pnRImg - input R Image
        m_pnGImg - input G Image
        m_pnBImg - input B Image
    \return:
        m_pnSmallRImg - output down sampled image
        m_pnSmallGImg - output down sampled image
        m_pnSmallBImg - output down sampled image
*/
void dehazing::DownsampleImageColor()
{
    int nX, nY;

    float fRatioY, fRatioX;
    fRatioX = (float)m_nWid/(float)320;
    fRatioY = (float)m_nHei/(float)240;

    for( nY = 0; nY < 240; nY++ ) {
        for( nX = 0; nX < 320; nX++ ) {
            m_pnSmallRImg[nY*320+nX] = m_pnRImg[(int)(nY*fRatioY)*m_nWid+(int)(nX*fRatioX)];
            m_pnSmallGImg[nY*320+nX] = m_pnGImg[(int)(nY*fRatioY)*m_nWid+(int)(nX*fRatioX)];
            m_pnSmallBImg[nY*320+nX] = m_pnBImg[(int)(nY*fRatioY)*m_nWid+(int)(nX*fRatioX)];
        }
    }
}

/* 
    \brief: upsample the fixed sized transmission to original size

    \param:(hidden) 
        m_pfSmallTransR - input transmission (320 x 240)
    \return:
        m_pfTransmission - output transmission 
*/
void dehazing::UpsampleTransmission()
{
    int nX, nY;

    float fRatioY, fRatioX;
    // width and height scale factor 
    fRatioX = (float)320/(float)m_nWid;
    fRatioY = (float)240/(float)m_nHei;

    for(nY=0; nY<m_nHei; nY++)
    {
        for(nX=0; nX<m_nWid; nX++)
        {
            // restore transmission m_pfSmallTrans from downSample transmission
            m_pfTransmission[nY*m_nWid+nX] = m_pfSmallTrans[(int)(nY*fRatioY)*320+(int)(nX*fRatioX)];
        }
    }
}

/* 
    \brief: Make a Look Up Table(LUT) for applying information in previous frame.

    \return:
        m_pfExpLUT - output table 
*/
void dehazing::MakeExpLUT()
{
    int nIdx;

    for ( nIdx = 0 ; nIdx < 256; nIdx++ )
    {
        m_pfExpLUT[nIdx] = exp(-(float)(nIdx*nIdx)/10.0f);
    }
}

/* 
    \brief: Make a Look Up Table(LUT) for guided filtering

    \return:
        m_pfGuidedLUT - output table
*/
void dehazing::GuideLUTMaker()
{
    int nX, nY;
    int temp_x, temp_y;
    int idx_1, idx_2, idx_3, idx_4;

    for ( nX = 0 ; nX < m_nGBlockSize/2; nX++ ) {
        for ( nY = 0 ; nY < m_nGBlockSize/2 ; nY++ ) {
            temp_x = nX-m_nGBlockSize/2+1;
            temp_y = nY-m_nGBlockSize/2+1;
            idx_1 = nY*m_nGBlockSize+nX;
            idx_2 = (m_nGBlockSize-nY-1)*m_nGBlockSize+nX;
            idx_3 = nY*m_nGBlockSize+m_nGBlockSize-nX-1;
            idx_4 = (m_nGBlockSize-nY-1)*m_nGBlockSize+m_nGBlockSize-nX-1;

            m_pfGuidedLUT[idx_1]=(exp(-(temp_x*temp_x+temp_y*temp_y)/(2*m_fGSigma*m_fGSigma)));
            m_pfGuidedLUT[idx_2]=(exp(-(temp_x*temp_x+temp_y*temp_y)/(2*m_fGSigma*m_fGSigma)));
            m_pfGuidedLUT[idx_3]=(exp(-(temp_x*temp_x+temp_y*temp_y)/(2*m_fGSigma*m_fGSigma)));
            m_pfGuidedLUT[idx_4]=(exp(-(temp_x*temp_x+temp_y*temp_y)/(2*m_fGSigma*m_fGSigma)));
        }
    }
}
/* 
    \brief: Make a Look Up Table(LUT) for gamma correction
            y = x^gamma;  x in [0, 1]
    \param:
        fParameter - gamma value.
    \return:
        m_pfGuidedLUT - output table
*/
void dehazing::GammaLUTMaker(float fParameter)
{
    int nIdx;

    for(nIdx=0; nIdx<256; nIdx++) {
        m_pucGammaLUT[nIdx] = (uchar)(pow((float)nIdx/255, fParameter)*255.0f);
    }
}