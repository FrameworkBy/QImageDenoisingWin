#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QDebug>

#define sdelete(PTR) if(PTR) delete PTR

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage* originalImage  = NULL;
    QImage* noisyImage     = NULL;
    QImage* denoisedImage1 = NULL;
    QImage* denoisedImage2 = NULL;
    QGraphicsScene* scene1 = NULL;
    QGraphicsScene* scene2 = NULL;
    QGraphicsScene* scene3 = NULL;

public slots:
    void openImageDialog();
    void openImageFile(QString fileName);
    void addNoise();
    void denoise();
};

#endif // MAINWINDOW_H
