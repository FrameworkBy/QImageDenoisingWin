#include "nlm_filter_gray3.h"
#include "utils.h"
#include <cmath>

void nlm_filter_gray3(QImage* imageNoise,
                      QImage* imageFiltered,
                      QSize imageSize,
                      int halfWindowSize,
                      int halfPatchSize,
                      double fSigma,
                      double fParam) {
    /* SIZES */
    int patchSize = halfPatchSize * 2 + 1;
    int m = imageSize.width();
    int n = imageSize.height();
    int w = m;
    int h = n;
    int incWidth = patchSize - 1 + w;
    int incHeight = patchSize - 1 + h;

    /* CREATE ARRAYS */
    double** colorInput = new double*[w];
    for (int i = 0; i < w; i++) {
        colorInput[i] = new double[h];
    }
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            colorInput[i][j] = qGray(imageNoise->pixel(i,j));
        }
    }
    double** fpCount = new double*[incWidth];
    double** colorOutput = new double*[incWidth];
    for (int i = 0; i < incWidth; i++) {
        colorOutput[i] = new double[incHeight];
        fpCount[i] = new double[incHeight];
    }
    for (int i = 0; i < incWidth; i++)
        for (int j = 0; j < incHeight; j++) {
            fpCount[i][j] = 0.0;
            colorOutput[i][j] = 0.0;
        }

    double** denoised = new double*[patchSize];
    for (int i = 0; i < patchSize; i++) {
        denoised[i] = new double[patchSize];
    }

    /* FILTER PARAMETER */
    double fSigma2 = fSigma * fSigma;
    double fH = fParam * fSigma;
    double fH2 = fH * fH;
    double icwl = patchSize * patchSize;
    fH2 *= icwl;

    /* INCREASE IMAGE */
    double** increasedImage = new double*[incWidth];
    for (int i = 0; i < incWidth; i++) {
        increasedImage[i] = new double[incHeight];
    }
    nlm_increse_image(colorInput, increasedImage, imageSize, halfPatchSize);

    /* FILTER */
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int i1 = i+halfPatchSize;
            int j1 = j+halfPatchSize;

            double fMaxWeight = 0.0;
            double fTotalWeight = 0.0;

            int rmin = std::max(i1-halfWindowSize,halfPatchSize);
            int rmax = std::min(i1+halfWindowSize,m+halfPatchSize);
            int smin = std::max(j1-halfWindowSize,halfPatchSize);
            int smax = std::min(j1+halfWindowSize,n+halfPatchSize);

            for (int ii = 0; ii < patchSize; ii++) {
                for (int jj = 0; jj < patchSize; jj++) {
                    denoised[ii][jj] = 0.0;
                }
            }

            for (int r = rmin; r < rmax; r++) {
                for (int s = smin; s < smax; s++) {
                    if (r == i1 && s == j1) {
                        continue;
                    }

                    double fDif = 0;
                    double dif = 0;
                    for (int ii = -halfPatchSize; ii <= halfPatchSize; ii++) {
                        for (int jj = -halfPatchSize; jj <= halfPatchSize; jj++) {
                            dif = increasedImage[i1+ii][j1+jj]-increasedImage[r+ii][s+jj];
                            fDif += dif*dif;
                        }
                    }

                    fDif = std::max(fDif - 2.0 * (double) icwl *  fSigma2, 0.0);
                    fDif = fDif / fH2;
                    double fWeight = exp(-fDif);

                    if (fWeight > fMaxWeight) {
                        fMaxWeight = fWeight;
                    }

                    fTotalWeight += fWeight;

                    for (int ii = -halfPatchSize; ii <= halfPatchSize; ii++) {
                        int il = ii + halfPatchSize;
                        for (int jj = -halfPatchSize; jj <= halfPatchSize; jj++) {
                            int jl =  jj + halfPatchSize;
                            denoised[il][jl] += fWeight * increasedImage[r+ii][s+jj];
                        }
                    }
                }

                for (int ii = -halfPatchSize; ii <= halfPatchSize; ii++) {
                    int il = ii + halfPatchSize;
                    for (int jj = -halfPatchSize; jj <= halfPatchSize; jj++) {
                        int jl =  jj + halfPatchSize;
                        denoised[il][jl] += fMaxWeight * increasedImage[i1+ii][j1+jj];
                    }
                }

                fTotalWeight += fMaxWeight;

                if (fTotalWeight > 0.0001) {
                    for (int ii = -halfPatchSize; ii <= halfPatchSize; ii++) {
                        int il = ii + halfPatchSize;
                        for (int jj = -halfPatchSize; jj <= halfPatchSize; jj++) {
                            int jl =  jj + halfPatchSize;
                            fpCount[i1+ii][j1+jj]++;
                            colorOutput[i1+ii][j1+jj] += denoised[il][jl] / fTotalWeight;
                        }
                    }
                }
            }

        }
    }
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (fpCount[i+halfPatchSize][j+halfPatchSize] > 0.0) {
                colorOutput[i+halfPatchSize][j+halfPatchSize] /= fpCount[i+halfPatchSize][j+halfPatchSize];
            } else {
                colorOutput[i+halfPatchSize][j+halfPatchSize] = colorInput[i][j];
            }
        }
    }

    /* CREATE FILTERED IMAGE */
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int gray = colorOutput[i+halfPatchSize][j+halfPatchSize];
            imageFiltered->setPixel(i, j, qRgb(gray, gray, gray));
        }
    }

    /* CLEAR MEMORY */
    for (int i = 0; i < w; i++) {
        delete []colorInput[i];
    }
    for (int i = 0; i < incWidth; i++) {
        delete []colorOutput[i];
        delete []fpCount[i];
    }
    for (int i = 0; i < patchSize; i++) {
        delete[] denoised[i];
    }
    delete []colorOutput;
    delete []colorInput;
    delete []denoised;
    delete []fpCount;
}
