#ifndef NLM_FILTER_GRAY_H
#define NLM_FILTER_GRAY_H


#include <QImage>
#include <QSize>

void nlm_filter_gray(QImage* imageNoise,
                        QImage *imageFiltered,
                        QSize imageSize,
                        int halfWindowSize,
                        int halfPatchSize,
                        double fSigma, double fParam);

#endif // NLM_FILTER_GRAY_H
