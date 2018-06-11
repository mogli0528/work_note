#ifndef _UTILS_H_
#define _UTILS_H_

#include <cmath>
#include <vector>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


namespace utils
{

  /*
   * produit scalaire
   */
  float dot(const vector<float>& a, const vector<float>& b);
  
  /*
   * retourne le vecteur des valeurs du patch centrer en x,y
   */
  vector<float> getGrayScaleVector(CImg<unsigned char> & I,int x,int y,int size);

  /*
   * normalise un vecteur
   */
  vector<float> normalize(vector<float>& a);

  /*
   * Norme vecteur
   */ 
  float norme(vector<float>& x);

  /*
   * filtre moyenneur
   */
  const float means(vector<float>& a);

  /*
   * filtre median
   */
  const float median(vector<float>& a);

  /*
   * Deviation standard de Gauss 2D
   */
  float standarDeviationGauss(int x, int y, float h);
  
  float standarDeviationGauss1D(int x, float h);
    
  /*
   * Distance euclidienne 2 par rapport a un noyau gaussian g
   */
  float euclideanDistance(vector<float>& a, vector<float>& b,float g);

  float euclideanDistanceGauss(vector<float>& a, vector<float>& b,float h);


  /*
   * Just a Zero padding fonction for filter
   */
  CImg<unsigned char> zeroPadding(const CImg<unsigned char>& I,int patchSize);
  
  CImg<unsigned char> zeroPadding(const CImg<unsigned char>& I, int patchSize, int search);

  CImg<unsigned char> dilaPadding(CImg<unsigned char> const &I, int size, int search);
  
  CImg<unsigned char> eroPadding(CImg<unsigned char> const &I, int size, int search);

  float meansSquareError(const CImg<unsigned char>& I, const CImg<unsigned char>& F);
};

#endif
