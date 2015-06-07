#include "utils.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

void calcMsePsnr(double* mse, double* psnr, QImage *image1, QImage *image2, QSize size) {
    double P = 0;
    for (int i = 0; i < size.width(); i++) {
        for (int j = 0; j < size.height(); j++) {
            int pixel1 = qGray(image1->pixel(i,j));
            int pixel2 = qGray(image2->pixel(i,j));
            double SQ = pixel1-pixel2;
            P += SQ*SQ;
        }
    }
    P /= size.width()*size.height();
    *mse = sqrt(P);
    *psnr = 10 * log10(65025./((*mse) * (*mse)));

}

void make_kernel(double** gKernel, int kernel_size, double sigma) {
    double r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    double sum = 0.0;

    int size = kernel_size / 2;

    int dd = kernel_size%2?0:1;

    // generate kernel
    for (int x = -size; x <= size-dd; x++)
    {
        for(int y = -size; y <= size-dd; y++)
        {
            r = sqrt(x*x + y*y);
            gKernel[x + size][y + size] = (exp(-(r*r)/s))/(M_PI * s);
            sum += gKernel[x + size][y + size];
        }
    }
    // normalize the Kernel
    for(int i = 0; i < kernel_size; ++i)
        for(int j = 0; j < kernel_size; ++j)
            gKernel[i][j] /= sum;
}
