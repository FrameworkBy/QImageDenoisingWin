#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "noise.h"
#include "nlm_filter_gray.h"
#include "nlm_filter_gray2.h"
#include "nlm_filter_gray3.h"
#include "utils.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    originalImage = new QImage();

    scene1 = new QGraphicsScene();
    ui->graphicsView->setScene(scene1);

    scene2 = new QGraphicsScene();
    ui->graphicsView_2->setScene(scene2);

    scene3 = new QGraphicsScene();
    ui->graphicsView_3->setScene(scene3);

    connect(ui->actionOpen, SIGNAL(triggered()),this, SLOT(openImageDialog()));
    connect(ui->noise, SIGNAL(clicked()), this, SLOT(addNoise()));
    connect(ui->denoise, SIGNAL(clicked()), this, SLOT(denoise()));
}

MainWindow::~MainWindow()
{
    sdelete (originalImage);
    sdelete (noisyImage);
    sdelete (denoisedImage1);
    sdelete (denoisedImage2);
    sdelete (scene1);
    sdelete (scene2);
    sdelete (scene3);
    delete ui;
}

void MainWindow::openImageFile(QString fileName)
{
    QImageReader* imageReader = new QImageReader(fileName);
    imageReader->read(this->originalImage);
    delete imageReader;

    scene1->clear();
    scene2->clear();
    scene3->clear();

    ui->graphicsView->setSceneRect(originalImage->rect());
    ui->graphicsView_2->setSceneRect(originalImage->rect());
    ui->graphicsView_3->setSceneRect(originalImage->rect());

    scene1->addPixmap(QPixmap::fromImage(*originalImage));
    ui->graphicsView->show();
    sdelete (noisyImage);
    noisyImage = NULL;
}

void MainWindow::addNoise()
{
    double noise = ui->noiseBox->value();
    if (noise == 0 && noisyImage) {
        delete noisyImage;
        noisyImage = NULL;

        scene1->clear();
        scene1->addPixmap(QPixmap::fromImage(*originalImage));
        ui->graphicsView->show();
        return;
    } else if (noise == 0) {
        return;
    }
    noisyImage = addAWGN_GRAY(originalImage,originalImage->size(),noise);
    scene1->clear();
    scene1->addPixmap(QPixmap::fromImage(*noisyImage));
    ui->graphicsView->show();
}

void MainWindow::denoise()
{
    if (!noisyImage) return;

    double noise = ui->noiseBox->value();

    int hP = 1;
    int hW = 10;
    double h = 0.40;
    if (noise > 15 && noise <= 30) {
        hP = 2;
        hW = 10;
        h  = 0.40;
    } else if (noise > 30 && noise <= 45) {
        hP = 3;
        hW = 16;
        h  = 0.35;
    } else if (noise > 45 && noise <= 75) {
        hP = 4;
        hW = 16;
        h  = 0.35;
    } else if (noise > 75 && noise <= 100) {
        hP = 5;
        hW = 16;
        h  = 0.30;
    }

    denoisedImage1 = new QImage(originalImage->size(), QImage::Format_RGB32);
    QDateTime mStartTime = QDateTime::currentDateTime();
    nlm_filter_gray(noisyImage, denoisedImage1, originalImage->size(), hW, hP, noise, h);
    QDateTime mFinishTime = QDateTime::currentDateTime();
    qDebug() << QDateTime::fromMSecsSinceEpoch(mFinishTime.toMSecsSinceEpoch() - mStartTime.toMSecsSinceEpoch()).time();

    scene2->clear();
    scene2->addPixmap(QPixmap::fromImage(*denoisedImage1));
    ui->graphicsView_2->show();
    double mse;
    double psnr;
    calcMsePsnr(&mse, &psnr, originalImage, denoisedImage1, originalImage->size());
    delete denoisedImage1;
    denoisedImage1 = NULL;

    ui->mse1->setText(QString::number(mse));
    ui->psnr1->setText(QString::number(psnr));

    // *************************************************
    denoisedImage2 = new QImage(originalImage->size(), QImage::Format_RGB32);
    mStartTime = QDateTime::currentDateTime();
    nlm_filter_gray3(noisyImage, denoisedImage2, originalImage->size(), hW, hP, noise, h);
    mFinishTime = QDateTime::currentDateTime();
    qDebug() << QDateTime::fromMSecsSinceEpoch(mFinishTime.toMSecsSinceEpoch() - mStartTime.toMSecsSinceEpoch()).time();

    scene3->clear();
    scene3->addPixmap(QPixmap::fromImage(*denoisedImage2));
    ui->graphicsView_3->show();
    calcMsePsnr(&mse, &psnr, originalImage, denoisedImage2, originalImage->size());
    delete denoisedImage2;
    denoisedImage2 = NULL;

    ui->mse2->setText(QString::number(mse));
    ui->psnr2->setText(QString::number(psnr));
}

void MainWindow::openImageDialog()
{
    this->openImageFile(QFileDialog::getOpenFileName(this,QString(),"/home/alexandr/work/Data/Parrot", "*.png && *.jpg && *.bmp"));
    // this->openImageFile("C:/1WORK/GitHub/Parrot_ideal.png");
}
