#ifndef UTILS
#define UTILS

#include <QImage>
#include <QSize>

void calcMsePsnr(double* mse, double* psnr, QImage *image1, QImage *image2, QSize size);
void make_kernel(double** gKernel, int kernel_size, double sigma);

template<typename TYPE>
void nlm_increse_image(TYPE ** src, TYPE ** dst, QSize srcImageSize, int inc) {
    int srcW = srcImageSize.width();
    int srcH = srcImageSize.height();
    // COPY ORIGINAL IMAGE
    for (int i = inc; i < srcH+inc; i++) {
        for  (int j = inc; j < srcW+inc; j++) {
            dst[j][i] = src[j-inc][i-inc];
        }
    }

    for (int j1 = 0, j2 = srcH+inc, k1 = 1, k2 = 1; j1 < inc; j1++, k1-=2, k2+=2, j2++)
    {
        for (int i = inc; i < srcW+inc; i++) {
            dst[i][j1] = dst[i][j1+inc+k1];
            dst[i][j2] = dst[i][j2-k2];
        }
    }

    for (int i1 = 0, i2 = srcW+inc, k1 = 1, k2 = 1; i1 < inc; i1++, i2++, k1-=2, k2+=2) {
        for (int j = 0; j < srcH+2*inc; j++) {
            dst[i1][j] = dst[i1+inc+k1][j];
            dst[i2][j] = dst[i2-k2][j];
        }
    }
}

#endif // UTILS

