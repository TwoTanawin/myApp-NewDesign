#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QFileDialog>
#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <Qtcore>
#include <QResource>
#include <QDir>
#include <QStandardPaths>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <qvalidator.h>

#include <ctime>

// Define global variables for video feed and thread status
cv::VideoCapture videoCapture;
std::thread cam_thread;
bool thread4Running = true;

// Create a QVector to store an array of integers
QVector<cv::Mat> myArrayBuffer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Convert string to Qstring
QString MainWindow::string_to_qstring(const std::string& str) {
    return QString::fromStdString(str);
}

// Convert Qstring to string
std::string MainWindow::qstringToString(const QString& qstr)
{
    return qstr.toStdString();
}


void MainWindow::on_pushButton_3_clicked()
{
    // Set thread4Running to true
    thread4Running = true;

    // Open IP camera using RTSP protocol
    videoCapture.open(qstringToString(ui->lineEdit->text()));

    // Check if the camera is opened successfully
    if (!videoCapture.isOpened())
    {
        std::cout << "Error: Could not open camera!" << std::endl;
        return;
    }

    // Start camera thread
    cam_thread = std::thread([&]() {
        while (thread4Running) {
            cv::Mat frame;
            videoCapture >> frame;

            // Add the frame to the buffer
            myArrayBuffer.append(frame);
            myArrayBuffer.resize(10);
            if (myArrayBuffer.size() == 5){
                myArrayBuffer.clear();
            }
            else{
                myArrayBuffer.append(frame);
            }

            // Convert OpenCV Mat to Qt QImage
            QImage qImg = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            qImg = qImg.rgbSwapped();

            // Resize the QImage to the label size
            qImg = qImg.scaled(ui->label_cam->size(), Qt::KeepAspectRatio);

            // Display the QImage on the label
            ui->label_cam->setPixmap(QPixmap::fromImage(qImg));
            ui->label_cam->setAlignment(Qt::AlignCenter);
            ui->label_cam->update();
        }
    });

    ui->textBrowser->append("Camera is connected.");
}

void MainWindow::closeEvent(QCloseEvent *event){
    // Stop camera thread
    thread4Running = false;
    if (cam_thread.joinable())
        cam_thread.join();

    // Release videoCapture resources
    videoCapture.release();

    ui->textBrowser->append("Camera is disconnect.");
}


void MainWindow::on_pushButton_4_clicked()
{
    // Stop camera thread
    thread4Running = false;
    if (cam_thread.joinable())
        cam_thread.join();

    // Release videoCapture resources
    videoCapture.release();

    ui->textBrowser->append("Camera is disconnect.");
}

