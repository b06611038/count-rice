#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <vector>

QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat image;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image = cv::imread("rice.png");
    cv::Mat image1=image.clone();
    for (int i=0;i<image.cols;i++)
        {
            for (int j=0;j<image.rows;j++)
            {
                int blue = image.at<cv::Vec3b>(j,i).val[0];
                int green = image.at<cv::Vec3b>(j,i).val[1];
                int red = image.at<cv::Vec3b>(j,i).val[2];
                if((blue<115)||(green<115)||(red<115))
                {
                    image.at<cv::Vec3b>(j,i).val[0]=255;
                    image.at<cv::Vec3b>(j,i).val[1]=255;
                    image.at<cv::Vec3b>(j,i).val[2]=255;
                }
                else
                {
                    image.at<cv::Vec3b>(j,i).val[0]=0;
                    image.at<cv::Vec3b>(j,i).val[1]=0;
                    image.at<cv::Vec3b>(j,i).val[2]=0;
                }
            }
        }
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat image2=image.clone();
    cv::Point anchor;
    cv::Mat element=cv::getStructuringElement(0,cv::Size(15,15),  cv::Point( -1, -1));
    cv::erode(image,image,element);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours( image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    QString number = QString::number(contours.size());
    ui->label_4->setText(number);
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    ui->label_3->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(cvMat2QImage(image1).scaled(ui->label->width(),ui->label->height(),Qt::IgnoreAspectRatio)));
    ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(image2).scaled(ui->label_2->width(),ui->label_2->height(),Qt::IgnoreAspectRatio)));
    ui->label_3->setPixmap(QPixmap::fromImage(cvMat2QImage(image).scaled(ui->label_3->width(),ui->label_3->height(),Qt::IgnoreAspectRatio)));

}
QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
