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
#include <fstream>

std::vector<cv::Point> refPt_ProcessingRegion0;
std::vector<cv::Point> myArr_ProcessingRegion0;
cv::Mat image_ProcessingRegion0;

std::vector<cv::Point> refPt_VehicleDetectionRegion0;
std::vector<cv::Point> myArr_VehicleDetectionRegion0;
cv::Mat image_VehicleDetectionRegion0;

std::vector<cv::Point> refPt_RegionWithCountingLines0;
std::vector<cv::Point> myArr_RegionWithCountingLines0;
cv::Mat image_RegionWithCountingLines0;

std::vector<cv::Point> refPt_RegionForLane0;
std::vector<cv::Point> myArr_RegionForLane0;
cv::Mat image_RegionForLane0;

std::vector<cv::Point> refPt_RegionForLane1;
std::vector<cv::Point> myArr_RegionForLane1;
cv::Mat image_RegionForLane1;

std::vector<cv::Point> refPt_RegionForLane2;
std::vector<cv::Point> myArr_RegionForLane2;
cv::Mat image_RegionForLane2;

// Read write ROI file TA
cv::Mat buffProcessingRegion0;
QVector<cv::Mat> buffProcessingRegion1;

cv::Mat buffVehicleDetectionRegion0;
QVector<cv::Mat> buffVehicleDetectionRegion1;

cv::Mat buffRegionWithCountingLines0;
QVector<cv::Mat> buffRegionWithCountingLines1;

cv::Mat buffRegionForLane0_0;
QVector<cv::Mat> buffRegionForLane0_1;

cv::Mat buffRegionForLane1_0;
QVector<cv::Mat> buffRegionForLane1_1;

cv::Mat buffRegionForLane2_0;
QVector<cv::Mat> buffRegionForLane2_1;

// Define global variables for video feed and thread status
cv::VideoCapture videoCapture;
std::thread cam_thread;
bool thread4Running = true;

// Create a QVector to store an array of integers
QVector<cv::Mat> myArrayBuffer;

// HA IP WW ROI variable
std::vector<cv::Point> refPt_ha;
std::vector<cv::Point> myArr_ha;
cv::Mat image_ha;

std::vector<cv::Point> refPt_ip;
std::vector<cv::Point> myArr_ip;
cv::Mat image_ip;

std::vector<cv::Point> refPt_ww;
std::vector<cv::Point> myArr_ww;
cv::Mat image_ww;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    getConfigfile();


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


std::string matToString(const cv::Mat& image) {
    std::stringstream ss;
    ss << image;
    return ss.str();
}

void MainWindow::getConfigfile(){
    readINI();
    readTAROI();
    readHAROI();
    readIPROI();
    readWWROI();
}

void MainWindow::setCheckbutton(){
    ui->btn_del_lane0->setChecked(false);
    ui->btn_del_lane1->setChecked(false);
    ui->btn_del_lane2->setChecked(false);

    ui->btn_del_lane0->setEnabled(false);
    ui->btn_del_lane1->setEnabled(false);
    ui->btn_del_lane2->setEnabled(false);

//    ui->btn_draw_roi->setVisible(false);
}

void MainWindow::readINI(){

    std::ifstream file("QtAICenter.ini");

    QSettings settings("QtAICenter.ini",QSettings::Format::IniFormat);

//    file.is_open() && file.good()
//    settings.value("Sigle_Value")== "" || settings.value("Sigle_Value_2") == ""

    if(!file.is_open() && !file.good()){
//        ui->textBrowser->setText("INI empty");
//        QMessageBox::critical(nullptr, "Error", "Config INI file not found.");
//        ui->textBrowser->setText("INI empty");
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_get_origin_ta->setText("");
        ui->lineEdit_get_origin_ha->setText("");
        ui->lineEdit_get_origin_ip->setText("");
        ui->lineEdit_get_origin_ww->setText("");
    }
    else{
        ui->lineEdit->setText(settings.value("Sigle_Value").toString());
        ui->lineEdit_2->setText(settings.value("Sigle_Value_2").toString());
        ui->lineEdit_get_origin_ta->setText(settings.value("Sigle_Value_3").toString());
        ui->lineEdit_get_origin_ha->setText(settings.value("Sigle_Value_4").toString());
        ui->lineEdit_get_origin_ip->setText(settings.value("Sigle_Value_5").toString());
        ui->lineEdit_get_origin_ww->setText(settings.value("Sigle_Value_6").toString());
    }
}

void MainWindow::readINI_Output_Dir(){

}


void MainWindow::readTAROI(){
    cv::FileStorage fs;
    fs.open("ROI_ta_use.roi", cv::FileStorage::READ);
    if (!fs.isOpened())
    {
//        QMessageBox::critical(nullptr, "Error", "Config file not found.");
        ui->lineEdit_ProcessingRegion0->setText("");
        ui->lineEdit_VehicleDetectionRegion0->setText("");
        ui->lineEdit_RegionWithCountingLines0->setText("");
        ui->lineEdit_RegionForLane0->setText("");
        ui->lineEdit_RegionForLane1->setText("");
        ui->lineEdit_RegionForLane2->setText("");
    }
    else{
        fs["ProcessingRegion0"] >> buffProcessingRegion0;
        fs["VehicleDetectionRegion0"] >> buffVehicleDetectionRegion0;
        fs["RegionWithCountingLines0"] >> buffRegionWithCountingLines0;
        fs["RegionForLane0"] >> buffRegionForLane0_0;
        fs["RegionForLane1"] >> buffRegionForLane1_0;
        fs["RegionForLane2"] >> buffRegionForLane2_0;
        //    std::cout << "mROI: " << mROI << std::endl;
        ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(buffProcessingRegion0)));
        ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(buffVehicleDetectionRegion0)));
        ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(buffRegionWithCountingLines0)));
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(buffRegionForLane0_0)));
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(buffRegionForLane1_0)));
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(buffRegionForLane2_0)));

        //    buffProcessingRegion1 = matToQVector(buffProcessingRegion0);
        //    std::cout << "my buff buffProcessingRegion1 : " << buffProcessingRegion1;
        fs.release();
    }
//    fs["ProcessingRegion0"] >> buffProcessingRegion0;
//    fs["VehicleDetectionRegion0"] >> buffVehicleDetectionRegion0;
//    fs["RegionWithCountingLines0"] >> buffRegionWithCountingLines0;
//    fs["RegionForLane0"] >> buffRegionForLane0_0;
//    fs["RegionForLane1"] >> buffRegionForLane1_0;
//    fs["RegionForLane2"] >> buffRegionForLane2_0;
//    //    std::cout << "mROI: " << mROI << std::endl;
//    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(buffProcessingRegion0)));
//    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(buffVehicleDetectionRegion0)));
//    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(buffRegionWithCountingLines0)));
//    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(buffRegionForLane0_0)));
//    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(buffRegionForLane1_0)));
//    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(buffRegionForLane2_0)));

//    //    buffProcessingRegion1 = matToQVector(buffProcessingRegion0);
//    //    std::cout << "my buff buffProcessingRegion1 : " << buffProcessingRegion1;
//    fs.release();
}

void MainWindow::readHAROI(){
    cv::FileStorage fs;
    fs.open("ROI_ha.roi", cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        ui->lineEdit_ip->setText("");
    }
    fs["ProcessingRegion0"] >> buffProcessingRegion0;

    //    std::cout << "mROI: " << mROI << std::endl;
    ui->lineEdit_ha->setText(string_to_qstring(matToString(buffProcessingRegion0)));
    fs.release();
}

void MainWindow::readIPROI(){
    cv::FileStorage fs;
    fs.open("ROI_ip.roi", cv::FileStorage::READ);
    if (!fs.isOpened())
    {
//        QMessageBox::critical(nullptr, "Error", "Config file not found.");
        ui->lineEdit_ip->setText("");
    }
    else{
        fs["ProcessingRegion0"] >> buffProcessingRegion0;

        //    std::cout << "mROI: " << mROI << std::endl;
        ui->lineEdit_ip->setText(string_to_qstring(matToString(buffProcessingRegion0)));
        fs.release();
    }
//    fs["ProcessingRegion0"] >> buffProcessingRegion0;

//    //    std::cout << "mROI: " << mROI << std::endl;
//    ui->lineEdit_ip->setText(string_to_qstring(matToString(buffProcessingRegion0)));
//    fs.release();
}


void MainWindow::readWWROI(){
    cv::FileStorage fs;
    fs.open("ROI_ww.roi", cv::FileStorage::READ);
    if (!fs.isOpened())
    {
//        QMessageBox::critical(nullptr, "Error", "Config file not found.");
        ui->lineEdit_ww->setText("");
    }
    else{
        fs["ProcessingRegion0"] >> buffProcessingRegion0;

        //    std::cout << "mROI: " << mROI << std::endl;
        ui->lineEdit_ww->setText(string_to_qstring(matToString(buffProcessingRegion0)));
        fs.release();
    }
//    fs["ProcessingRegion0"] >> buffProcessingRegion0;

//    //    std::cout << "mROI: " << mROI << std::endl;
//    ui->lineEdit_ww->setText(string_to_qstring(matToString(buffProcessingRegion0)));
//    fs.release();
}

cv::Mat convertQStringToMat(const QString& coordinateString) {
    QString str = coordinateString;

    // Remove brackets and split the string by semicolon
    str = str.mid(1, str.size() - 2);
    QStringList coordinatePairs = str.split(";");

    // Create a cv::Mat to store the coordinates
    cv::Mat coordinates(coordinatePairs.size(), 2, CV_32S);

    // Extract and store the coordinates in the cv::Mat
    for (int i = 0; i < coordinatePairs.size(); ++i) {
        QString coordinatePair = coordinatePairs.at(i);
        QStringList coords = coordinatePair.split(",");
        int x = coords.at(0).trimmed().toInt();
        int y = coords.at(1).trimmed().toInt();
        coordinates.at<int>(i, 0) = x;
        coordinates.at<int>(i, 1) = y;
    }

    return coordinates;
}

void click_event_ta_ProcessingRegion0(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_ProcessingRegion0.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_ProcessingRegion0.size()<<std::endl;

        if (refPt_ProcessingRegion0.size() == 1) {
            cv::Point center_coordinates = refPt_ProcessingRegion0[0];
            cv::circle(image_ProcessingRegion0, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }

        if (refPt_ProcessingRegion0.size() > 1) {
            cv::Point start_point = refPt_ProcessingRegion0[refPt_ProcessingRegion0.size() - 2];
            cv::Point end_point = refPt_ProcessingRegion0[refPt_ProcessingRegion0.size() - 1];
            cv::line(image_ProcessingRegion0, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_ProcessingRegion0.size() == 1){
            cv::Point center_coordinates = refPt_ProcessingRegion0[0];
            cv::circle(image_ProcessingRegion0, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }
        if (refPt_ProcessingRegion0.size() > 1) {
            cv::Point start_point = refPt_ProcessingRegion0[refPt_ProcessingRegion0.size() - 2];
            cv::Point end_point = refPt_ProcessingRegion0[refPt_ProcessingRegion0.size() - 1];
            cv::line(image_ProcessingRegion0, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
        refPt_ProcessingRegion0.pop_back();
    }
}

void click_event_ta_VehicleDetectionRegion0(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_VehicleDetectionRegion0.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_VehicleDetectionRegion0.size()<<std::endl;

        if (refPt_VehicleDetectionRegion0.size() == 1) {
            cv::Point center_coordinates = refPt_VehicleDetectionRegion0[0];
            cv::circle(image_VehicleDetectionRegion0, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }

        if (refPt_VehicleDetectionRegion0.size() > 1) {
            cv::Point start_point = refPt_VehicleDetectionRegion0[refPt_VehicleDetectionRegion0.size() - 2];
            cv::Point end_point = refPt_VehicleDetectionRegion0[refPt_VehicleDetectionRegion0.size() - 1];
            cv::line(image_VehicleDetectionRegion0, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_VehicleDetectionRegion0.size() == 1){
            cv::Point center_coordinates = refPt_VehicleDetectionRegion0[0];
            cv::circle(image_VehicleDetectionRegion0, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }
        if (refPt_VehicleDetectionRegion0.size() > 1) {
            cv::Point start_point = refPt_VehicleDetectionRegion0[refPt_VehicleDetectionRegion0.size() - 2];
            cv::Point end_point = refPt_VehicleDetectionRegion0[refPt_VehicleDetectionRegion0.size() - 1];
            cv::line(image_VehicleDetectionRegion0, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
        refPt_VehicleDetectionRegion0.pop_back();
    }
}


void click_event_ta_RegionWithCountingLines0(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_RegionWithCountingLines0.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_RegionWithCountingLines0.size()<<std::endl;

        if (refPt_RegionWithCountingLines0.size() == 1) {
            cv::Point center_coordinates = refPt_RegionWithCountingLines0[0];
            cv::circle(image_RegionWithCountingLines0, center_coordinates, 5, cv::Scalar(255, 0, 0), 5);
        }

        if (refPt_RegionWithCountingLines0.size() > 1) {
            cv::Point start_point = refPt_RegionWithCountingLines0[refPt_RegionWithCountingLines0.size() - 2];
            cv::Point end_point = refPt_RegionWithCountingLines0[refPt_RegionWithCountingLines0.size() - 1];
            cv::line(image_RegionWithCountingLines0, start_point, end_point, cv::Scalar(255, 0, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_RegionWithCountingLines0.size() == 1){
            cv::Point center_coordinates = refPt_RegionWithCountingLines0[0];
            cv::circle(image_RegionWithCountingLines0, center_coordinates, 5, cv::Scalar(255, 0, 0), 5);
        }
        if (refPt_RegionWithCountingLines0.size() > 1) {
            cv::Point start_point = refPt_RegionWithCountingLines0[refPt_RegionWithCountingLines0.size() - 2];
            cv::Point end_point = refPt_RegionWithCountingLines0[refPt_RegionWithCountingLines0.size() - 1];
            cv::line(image_RegionWithCountingLines0, start_point, end_point, cv::Scalar(255, 0, 0), 9);
        }
        refPt_RegionWithCountingLines0.pop_back();
    }
}

void click_event_ta_RegionForLane0(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_RegionForLane0.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_RegionForLane0.size()<<std::endl;

        if (refPt_RegionForLane0.size() == 1) {
            cv::Point center_coordinates = refPt_RegionForLane0[0];
            cv::circle(image_RegionForLane0, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }

        if (refPt_RegionForLane0.size() > 1) {
            cv::Point start_point = refPt_RegionForLane0[refPt_RegionForLane0.size() - 2];
            cv::Point end_point = refPt_RegionForLane0[refPt_RegionForLane0.size() - 1];
            cv::line(image_RegionForLane0, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_RegionForLane0.size() == 1){
            cv::Point center_coordinates = refPt_RegionForLane0[0];
            cv::circle(image_RegionForLane0, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }
        if (refPt_RegionForLane0.size() > 1) {
            cv::Point start_point = refPt_RegionForLane0[refPt_RegionForLane0.size() - 2];
            cv::Point end_point = refPt_RegionForLane0[refPt_RegionForLane0.size() - 1];
            cv::line(image_RegionForLane0, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
        refPt_RegionForLane0.pop_back();
    }
}


void click_event_ta_RegionForLane1(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_RegionForLane1.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_RegionForLane1.size()<<std::endl;

        if (refPt_RegionForLane1.size() == 1) {
            cv::Point center_coordinates = refPt_RegionForLane1[0];
            cv::circle(image_RegionForLane1, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }

        if (refPt_RegionForLane1.size() > 1) {
            cv::Point start_point = refPt_RegionForLane1[refPt_RegionForLane1.size() - 2];
            cv::Point end_point = refPt_RegionForLane1[refPt_RegionForLane1.size() - 1];
            cv::line(image_RegionForLane1, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_RegionForLane1.size() == 1){
            cv::Point center_coordinates = refPt_RegionForLane1[0];
            cv::circle(image_RegionForLane1, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }
        if (refPt_RegionForLane1.size() > 1) {
            cv::Point start_point = refPt_RegionForLane1[refPt_RegionForLane1.size() - 2];
            cv::Point end_point = refPt_RegionForLane1[refPt_RegionForLane1.size() - 1];
            cv::line(image_RegionForLane1, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
        refPt_RegionForLane1.pop_back();
    }
}

void click_event_ta_RegionForLane2(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_RegionForLane2.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_RegionForLane2.size()<<std::endl;

        if (refPt_RegionForLane2.size() == 1) {
            cv::Point center_coordinates = refPt_RegionForLane2[0];
            cv::circle(image_RegionForLane2, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }

        if (refPt_RegionForLane2.size() > 1) {
            cv::Point start_point = refPt_RegionForLane2[refPt_RegionForLane2.size() - 2];
            cv::Point end_point = refPt_RegionForLane2[refPt_RegionForLane2.size() - 1];
            cv::line(image_RegionForLane2, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_RegionForLane2.size() == 1){
            cv::Point center_coordinates = refPt_RegionForLane2[0];
            cv::circle(image_RegionForLane2, center_coordinates, 5, cv::Scalar(0, 0, 255), 5);
        }
        if (refPt_RegionForLane2.size() > 1) {
            cv::Point start_point = refPt_RegionForLane2[refPt_RegionForLane2.size() - 2];
            cv::Point end_point = refPt_RegionForLane2[refPt_RegionForLane2.size() - 1];
            cv::line(image_RegionForLane2, start_point, end_point, cv::Scalar(0, 0, 255), 9);
        }
        refPt_RegionForLane2.pop_back();
    }
}

void click_event_ha(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_ha.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_ha.size()<<std::endl;

        if (refPt_ha.size() == 1) {
            cv::Point center_coordinates = refPt_ha[0];
            cv::circle(image_ha, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }

        if (refPt_ha.size() > 1) {
            cv::Point start_point = refPt_ha[refPt_ha.size() - 2];
            cv::Point end_point = refPt_ha[refPt_ha.size() - 1];
            cv::line(image_ha, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_ha.size() == 1){
            cv::Point center_coordinates = refPt_ha[0];
            cv::circle(image_ha, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }
        if (refPt_ha.size() > 1) {
            cv::Point start_point = refPt_ha[refPt_ha.size() - 2];
            cv::Point end_point = refPt_ha[refPt_ha.size() - 1];
            cv::line(image_ha, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
        refPt_ha.pop_back();
    }
}

void click_event_ip(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_ip.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_ip.size()<<std::endl;

        if (refPt_ip.size() == 1) {
            cv::Point center_coordinates = refPt_ip[0];
            cv::circle(image_ip, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }

        if (refPt_ip.size() > 1) {
            cv::Point start_point = refPt_ip[refPt_ip.size() - 2];
            cv::Point end_point = refPt_ip[refPt_ip.size() - 1];
            cv::line(image_ip, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_ip.size() == 1){
            cv::Point center_coordinates = refPt_ip[0];
            cv::circle(image_ip, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }
        if (refPt_ip.size() > 1) {
            cv::Point start_point = refPt_ip[refPt_ip.size() - 2];
            cv::Point end_point = refPt_ip[refPt_ip.size() - 1];
            cv::line(image_ip, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
        refPt_ip.pop_back();
    }
}

void click_event_ww(int event, int x, int y, int flags, void* param) {

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point Point(x, y);
        refPt_ww.push_back(Point);

        std::cout<<"refPtrefPt = "<<refPt_ww.size()<<std::endl;

        if (refPt_ww.size() == 1) {
            cv::Point center_coordinates = refPt_ww[0];
            cv::circle(image_ww, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }

        if (refPt_ww.size() > 1) {
            cv::Point start_point = refPt_ww[refPt_ww.size() - 2];
            cv::Point end_point = refPt_ww[refPt_ww.size() - 1];
            cv::line(image_ww, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
    }

    if(event == cv::EVENT_RBUTTONDOWN){
        if (refPt_ww.size() == 1){
            cv::Point center_coordinates = refPt_ww[0];
            cv::circle(image_ww, center_coordinates, 5, cv::Scalar(0, 255, 0), 5);
        }
        if (refPt_ww.size() > 1) {
            cv::Point start_point = refPt_ww[refPt_ww.size() - 2];
            cv::Point end_point = refPt_ww[refPt_ww.size() - 1];
            cv::line(image_ww, start_point, end_point, cv::Scalar(0, 255, 0), 9);
        }
        refPt_ww.pop_back();
    }
}

//void MainWindow::on_pushButton_3_clicked()
//{
//    // Set thread4Running to true
//    thread4Running = true;

//    // Open IP camera using RTSP protocol
//    videoCapture.open(qstringToString(ui->lineEdit->text()));

//    // Check if the camera is opened successfully
//    if (!videoCapture.isOpened())
//    {
//        std::cout << "Error: Could not open camera!" << std::endl;
//        return;
//    }

//    // Start camera thread
//    cam_thread = std::thread([&]() {
//        while (thread4Running) {
//            cv::Mat frame;
//            videoCapture >> frame;

//            // Add the frame to the buffer
//            myArrayBuffer.append(frame);
//            myArrayBuffer.resize(10);
//            if (myArrayBuffer.size() == 5){
//                myArrayBuffer.clear();
//            }
//            else{
//                myArrayBuffer.append(frame);
//            }

//            // Convert OpenCV Mat to Qt QImage
//            QImage qImg = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
//            qImg = qImg.rgbSwapped();

//            // Resize the QImage to the label size
//            qImg = qImg.scaled(ui->label_cam->size(), Qt::KeepAspectRatio);

//            // Display the QImage on the label
//            ui->label_cam->setPixmap(QPixmap::fromImage(qImg));
//            ui->label_cam->setAlignment(Qt::AlignCenter);
//            ui->label_cam->update();
//        }
//    });

//    ui->textBrowser->append("Camera is connected.");
//}

void MainWindow::closeEvent(QCloseEvent *event){
    // Stop camera thread
    thread4Running = false;
    if (cam_thread.joinable())
        cam_thread.join();

    // Release videoCapture resources
    videoCapture.release();

    ui->textBrowser->append("Camera is disconnect.");
}


//void MainWindow::on_pushButton_4_clicked()
//{
//    // Stop camera thread
//    thread4Running = false;
//    if (cam_thread.joinable())
//        cam_thread.join();

//    // Release videoCapture resources
//    videoCapture.release();

//    ui->textBrowser->append("Camera is disconnect.");
//}


void MainWindow::on_btn_start_cam_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Please setup IP camera.");
    }
    else{
        //    ui->textBrowser->append("Connecting ......");
        // Set thread4Running to true
        thread4Running = true;


        // Open IP camera using RTSP protocol
        videoCapture.open(qstringToString(ui->lineEdit->text()));

        // Check if the camera is opened successfully
        if (!videoCapture.isOpened())
        {
//            std::cout << "Error: Could not open camera!" << std::endl;
//            return;
            QMessageBox::critical(nullptr, "Error", "Could not open camera.");
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

        ui->btn_start_cam->setEnabled(false);

        ui->textBrowser->append("Camera is connected.");
    }
}


void MainWindow::on_btn_stop_cam_clicked()
{
    // Stop camera thread
    thread4Running = false;
    if (cam_thread.joinable())
        cam_thread.join();

    // Release videoCapture resources
    videoCapture.release();

    ui->btn_start_cam->setEnabled(true);

    ui->textBrowser->append("Camera is disconnect.");
}


void MainWindow::on_btn_output_path_clicked()
{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.*)"));
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QDir::homePath());
    QSettings settings("QtAICenter.ini",QSettings::Format::IniFormat);
//    if (!fileName.isEmpty()) {
//        QFileInfo fileInfo(fileName);
//        QString filePath = fileInfo.absolutePath();
        ui->lineEdit_2->setText(folderPath);
        settings.setValue("Sigle_Value_2",folderPath);
//    }

    ui->textBrowser->append("Output path was set");
}


void MainWindow::on_btn_get_oringin_ta_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Files (*.*)");
    QSettings settings("QtAICenter.ini", QSettings::Format::IniFormat);
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString filePath = fileInfo.absoluteFilePath();
        // Set the file path in a QLineEdit or similar widget
        ui->lineEdit_get_origin_ta->setText(filePath);
        settings.setValue("Sigle_Value_3", filePath);
    }

    ui->textBrowser->append("Path of TA config file was set");
}


void MainWindow::on_btn_set_ip_clicked()
{
    QSettings settings("QtAICenter.ini",QSettings::Format::IniFormat);
    settings.setValue("Sigle_Value",ui->lineEdit->text());
    ui->lineEdit->setText(settings.value("Sigle_Value").toString());

    ui->textBrowser->append("IP was set");
}


void MainWindow::on_btn_ProcessingRegion0_clicked()
{
    if (myArrayBuffer.empty() == true) {
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
            // return -1;
    }
    else{
        image_ProcessingRegion0 = myArrayBuffer.last();
        //        QMessageBox::information(nullptr, "Message", "Button clicked!");
        refPt_ProcessingRegion0.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_ProcessingRegion0);

        while (true) {
            cv::imshow("Image", image_ProcessingRegion0);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_ProcessingRegion0(refPt_ProcessingRegion0.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_ProcessingRegion0.size(); ++i) {
            refPtMat_ProcessingRegion0.at<int>(i, 0) = static_cast<int>(refPt_ProcessingRegion0[i].x);
            refPtMat_ProcessingRegion0.at<int>(i, 1) = static_cast<int>(refPt_ProcessingRegion0[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_ProcessingRegion0.at<int>(refPt_ProcessingRegion0.size(), 0) = static_cast<int>(refPt_ProcessingRegion0[0].x);
        refPtMat_ProcessingRegion0.at<int>(refPt_ProcessingRegion0.size(), 1) = static_cast<int>(refPt_ProcessingRegion0[0].y);

        std::cout << refPt_ProcessingRegion0;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_ProcessingRegion0;

//        ui->textBrowser->append(string_to_qstring(matToString(refPtMat_ProcessingRegion0)));
//        ui->textBrowser->append(string_to_qstring(matToString_new(refPtMat_ProcessingRegion0)));

        cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        cv::FileStorage fs0;
        fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        if (!fs0.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs0["ProcessingRegion0"] >> mROI0;
        fs0["VehicleDetectionRegion0"] >> mROI1;
        fs0["RegionWithCountingLines0"] >> mROI2;
        fs0["RegionForLane0"] >> mROI3;
        fs0["RegionForLane1"] >> mROI4;
        fs0["RegionForLane2"] >> mROI5;

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< refPtMat_ProcessingRegion0;
        ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(refPtMat_ProcessingRegion0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << mROI1;
        ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << mROI2;
        ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // this value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0"<< mROI3;
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        fs1 << "RegionForLane1" << mROI4;
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));

        fs1 << "RegionForLane2" << mROI5;
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs1.release();
        fs0.release();
        ui->textBrowser->append("ta roi was save");
    }
}


void MainWindow::on_bnt_VehicleDetectionRegion0_clicked()
{
    if (myArrayBuffer.empty() == true) {
        //       std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
            // return -1;
    }
    else{
        image_VehicleDetectionRegion0 = myArrayBuffer.last();
        refPt_VehicleDetectionRegion0.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_VehicleDetectionRegion0);

        while (true) {
            cv::imshow("Image", image_VehicleDetectionRegion0);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_VehicleDetectionRegion0(refPt_VehicleDetectionRegion0.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_VehicleDetectionRegion0.size(); ++i) {
            refPtMat_VehicleDetectionRegion0.at<int>(i, 0) = static_cast<int>(refPt_VehicleDetectionRegion0[i].x);
            refPtMat_VehicleDetectionRegion0.at<int>(i, 1) = static_cast<int>(refPt_VehicleDetectionRegion0[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_VehicleDetectionRegion0.at<int>(refPt_VehicleDetectionRegion0.size(), 0) = static_cast<int>(refPt_VehicleDetectionRegion0[0].x);
        refPtMat_VehicleDetectionRegion0.at<int>(refPt_VehicleDetectionRegion0.size(), 1) = static_cast<int>(refPt_VehicleDetectionRegion0[0].y);

        std::cout << refPt_VehicleDetectionRegion0;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_VehicleDetectionRegion0;

        cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        cv::FileStorage fs0;
        fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        if (!fs0.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs0["ProcessingRegion0"] >> mROI0;
        fs0["VehicleDetectionRegion0"] >> mROI1;
        fs0["RegionWithCountingLines0"] >> mROI2;
        fs0["RegionForLane0"] >> mROI3;
        fs0["RegionForLane1"] >> mROI4;
        fs0["RegionForLane2"] >> mROI5;

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< mROI0;
        ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << refPtMat_VehicleDetectionRegion0;
        ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(refPtMat_VehicleDetectionRegion0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << mROI2;
        ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // this value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0"<< mROI3;
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        fs1 << "RegionForLane1" << mROI4;
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));

        fs1 << "RegionForLane2" << mROI5;
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs1.release();
        fs0.release();
        ui->textBrowser->append("ta roi was save");
    }
}


void MainWindow::on_btn_RegionWithCountingLines0_clicked()
{
    if (myArrayBuffer.empty() == true) {
        //        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }
    else{
        image_RegionWithCountingLines0 = myArrayBuffer.last();
        refPt_RegionWithCountingLines0.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_RegionWithCountingLines0);

        while (true) {
            cv::imshow("Image", image_RegionWithCountingLines0);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_RegionWithCountingLines0(refPt_RegionWithCountingLines0.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_RegionWithCountingLines0.size(); ++i) {
            refPtMat_RegionWithCountingLines0.at<int>(i, 0) = static_cast<int>(refPt_RegionWithCountingLines0[i].x);
            refPtMat_RegionWithCountingLines0.at<int>(i, 1) = static_cast<int>(refPt_RegionWithCountingLines0[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_RegionWithCountingLines0.at<int>(refPt_RegionWithCountingLines0.size(), 0) = static_cast<int>(refPt_RegionWithCountingLines0[0].x);
        refPtMat_RegionWithCountingLines0.at<int>(refPt_RegionWithCountingLines0.size(), 1) = static_cast<int>(refPt_RegionWithCountingLines0[0].y);

        std::cout << refPt_RegionWithCountingLines0;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_RegionWithCountingLines0;

//        ui->textBrowser->append(string_to_qstring(matToString(refPtMat_RegionWithCountingLines0)));
//        ui->textBrowser->append(string_to_qstring(matToString_new(refPtMat_RegionWithCountingLines0)));

        cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        cv::FileStorage fs0;
        fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        if (!fs0.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs0["ProcessingRegion0"] >> mROI0;
        fs0["VehicleDetectionRegion0"] >> mROI1;
        fs0["RegionWithCountingLines0"] >> mROI2;
        fs0["RegionForLane0"] >> mROI3;
        fs0["RegionForLane1"] >> mROI4;
        fs0["RegionForLane2"] >> mROI5;

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< mROI0;
        ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << mROI1;
        ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << refPtMat_RegionWithCountingLines0;
        ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(refPtMat_RegionWithCountingLines0)));

        // this value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0"<< mROI3;
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        fs1 << "RegionForLane1" << mROI4;
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));

        fs1 << "RegionForLane2" << mROI5;
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs1.release();
        fs0.release();
        ui->textBrowser->append("ta roi was save");

        qDebug() << "ta lane0 was edit";
        //    qDebug() <<  matToString(refPtMat_RegionForLane0_edit);
    }
}


void MainWindow::on_btn_RegionForLane0_clicked()
{
    if (myArrayBuffer.empty() == true) {
        //        std::cout << "Failed to load the image." << std::endl;
        // return -1;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
    }
    else{
        image_RegionForLane0 = myArrayBuffer.last();
        refPt_RegionForLane0.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_RegionForLane0);

        while (true) {
            cv::imshow("Image", image_RegionForLane0);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_RegionForLane0(refPt_RegionForLane0.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_RegionForLane0.size(); ++i) {
            refPtMat_RegionForLane0.at<int>(i, 0) = static_cast<int>(refPt_RegionForLane0[i].x);
            refPtMat_RegionForLane0.at<int>(i, 1) = static_cast<int>(refPt_RegionForLane0[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_RegionForLane0.at<int>(refPt_RegionForLane0.size(), 0) = static_cast<int>(refPt_RegionForLane0[0].x);
        refPtMat_RegionForLane0.at<int>(refPt_RegionForLane0.size(), 1) = static_cast<int>(refPt_RegionForLane0[0].y);

        std::cout << refPt_RegionForLane0;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_RegionForLane0;

        //    refPtMat_RegionForLane0_edit = refPtMat_RegionForLane0;
        //    refPtMat_RegionForLane0_edit.append(refPtMat_RegionForLane0);


        //    cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        //    cv::FileStorage fs0;
        //    fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        //    if (!fs0.isOpened())
        //    {
        //        std::cerr << "Config file not found." << std::endl;
        //    }
        //    fs0["ProcessingRegion0"] >> mROI0;
        //    fs0["VehicleDetectionRegion0"] >> mROI1;
        //    fs0["RegionWithCountingLines0"] >> mROI2;
        //    fs0["RegionForLane0"] >> mROI3;
        //    fs0["RegionForLane1"] >> mROI4;
        //    fs0["RegionForLane2"] >> mROI5;

        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fs2;
        fs2.open("ROI_ta_lane_back.roi", cv::FileStorage::READ);
        if (!fs2.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs2["RegionForLane0"] >> bLane0;
        fs2["RegionForLane1"] >> bLane1;
        fs2["RegionForLane2"] >> bLane2;

        //    cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "ProcessingRegion0"<< mROI0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "VehicleDetectionRegion0" << mROI1;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "RegionWithCountingLines0" << mROI2;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));


        //    fs1 << "numRegionForLane" << 3;
        //    fs1 << "RegionForLane0" << refPtMat_RegionForLane0;
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(refPtMat_RegionForLane0)));

        //    // this value
        //    fs1 << "RegionForLane1"<< bLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(bLane1)));


        //    fs1 << "RegionForLane2" << bLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(bLane2)));


        //    fs1.release();
        //    fs0.release();
        //    ui->textBrowser->append("ta roi was save");


        cv::FileStorage fs3("ROI_ta_lane_back.roi", cv::FileStorage::WRITE);

        fs3 << "numRegionForLane" << 3;
        fs3 << "RegionForLane0" << refPtMat_RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(refPtMat_RegionForLane0)));

        // this value
        fs3 << "RegionForLane1"<< bLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs3 << "RegionForLane2" << bLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs2.release();
        fs3.release();
    }
}

void MainWindow::on_btn_RegionForLane1_clicked()
{
    if (myArrayBuffer.empty() == true) {
        //        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }

    else{
        image_RegionForLane1 = myArrayBuffer.last();
        refPt_RegionForLane1.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_RegionForLane1);

        while (true) {
            cv::imshow("Image", image_RegionForLane1);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_RegionForLane1(refPt_RegionForLane1.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_RegionForLane1.size(); ++i) {
            refPtMat_RegionForLane1.at<int>(i, 0) = static_cast<int>(refPt_RegionForLane1[i].x);
            refPtMat_RegionForLane1.at<int>(i, 1) = static_cast<int>(refPt_RegionForLane1[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_RegionForLane1.at<int>(refPt_RegionForLane1.size(), 0) = static_cast<int>(refPt_RegionForLane1[0].x);
        refPtMat_RegionForLane1.at<int>(refPt_RegionForLane1.size(), 1) = static_cast<int>(refPt_RegionForLane1[0].y);

        std::cout << refPt_RegionForLane1;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_RegionForLane1;

        //    refPtMat_RegionForLane1_edit = refPtMat_RegionForLane1;
        //    refPtMat_RegionForLane1_edit.append(refPtMat_RegionForLane1);


        //    cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        //    cv::FileStorage fs0;
        //    fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        //    if (!fs0.isOpened())
        //    {
        //        std::cerr << "Config file not found." << std::endl;
        //    }
        //    fs0["ProcessingRegion0"] >> mROI0;
        //    fs0["VehicleDetectionRegion0"] >> mROI1;
        //    fs0["RegionWithCountingLines0"] >> mROI2;
        //    fs0["RegionForLane0"] >> mROI3;
        //    fs0["RegionForLane1"] >> mROI4;
        //    fs0["RegionForLane2"] >> mROI5;

        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fs2;
        fs2.open("ROI_ta_use.roi", cv::FileStorage::READ);
        if (!fs2.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs2["RegionForLane0"] >> bLane0;
        fs2["RegionForLane1"] >> bLane1;
        fs2["RegionForLane2"] >> bLane2;

        //    cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "ProcessingRegion0"<< mROI0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "VehicleDetectionRegion0" << mROI1;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "RegionWithCountingLines0" << mROI2;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));


        //    fs1 << "numRegionForLane" << 3;
        //    fs1 << "RegionForLane0" << bLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(bLane0)));

        //    // this value
        //    fs1 << "RegionForLane1"<< refPtMat_RegionForLane1;
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(refPtMat_RegionForLane1)));


        //    fs1 << "RegionForLane2" << bLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(bLane2)));


        //    fs1.release();
        //    fs0.release();
        //    ui->textBrowser->append("ta roi was save");

        cv::FileStorage fs3("ROI_ta_lane_back.roi", cv::FileStorage::WRITE);

        fs3 << "numRegionForLane" << 3;
        fs3 << "RegionForLane0" << bLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fs3 << "RegionForLane1"<< refPtMat_RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(refPtMat_RegionForLane1)));


        fs3 << "RegionForLane2" << bLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs2.release();
        fs3.release();
    }
}


void MainWindow::on_btn_RegionForLane2_clicked()
{
    //    stop_cam();

    if (myArrayBuffer.empty() == true) {
        //        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }
    else{
        image_RegionForLane2 = myArrayBuffer.last();
        refPt_RegionForLane2.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ta_RegionForLane2);

        while (true) {
            cv::imshow("Image", image_RegionForLane2);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat_RegionForLane2(refPt_RegionForLane2.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_RegionForLane2.size(); ++i) {
            refPtMat_RegionForLane2.at<int>(i, 0) = static_cast<int>(refPt_RegionForLane2[i].x);
            refPtMat_RegionForLane2.at<int>(i, 1) = static_cast<int>(refPt_RegionForLane2[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat_RegionForLane2.at<int>(refPt_RegionForLane2.size(), 0) = static_cast<int>(refPt_RegionForLane2[0].x);
        refPtMat_RegionForLane2.at<int>(refPt_RegionForLane2.size(), 1) = static_cast<int>(refPt_RegionForLane2[0].y);

        std::cout << refPt_RegionForLane2;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat_RegionForLane2;

        //    refPtMat_RegionForLane2_edit = refPtMat_RegionForLane2;

        //    cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
        //    cv::FileStorage fs0;
        //    fs0.open("ROI_ta_use.roi", cv::FileStorage::READ);
        //    if (!fs0.isOpened())
        //    {
        //        std::cerr << "Config file not found." << std::endl;
        //    }
        //    fs0["ProcessingRegion0"] >> mROI0;
        //    fs0["VehicleDetectionRegion0"] >> mROI1;
        //    fs0["RegionWithCountingLines0"] >> mROI2;
        //    fs0["RegionForLane0"] >> mROI3;
        //    fs0["RegionForLane1"] >> mROI4;
        //    fs0["RegionForLane2"] >> mROI5;

        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fs2;
        fs2.open("ROI_ta_use.roi", cv::FileStorage::READ);
        if (!fs2.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs2["RegionForLane0"] >> bLane0;
        fs2["RegionForLane1"] >> bLane1;
        fs2["RegionForLane2"] >> bLane2;

        //    cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "ProcessingRegion0"<< mROI0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "VehicleDetectionRegion0" << mROI1;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        //    fs1 << "numProcessingRegion" << 1;
        //    fs1 << "RegionWithCountingLines0" << mROI2;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));


        //    fs1 << "numRegionForLane" << 3;
        //    fs1 << "RegionForLane0" << bLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(bLane0)));

        //    // this value
        //    fs1 << "RegionForLane1"<< bLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(bLane1)));


        //    fs1 << "RegionForLane2" << refPtMat_RegionForLane2;
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        //    fs1.release();
        //    fs0.release();
        //    ui->textBrowser->append("ta roi was save");

        cv::FileStorage fs3("ROI_ta_lane_back.roi", cv::FileStorage::WRITE);

        fs3 << "numRegionForLane" << 3;
        fs3 << "RegionForLane0" << bLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fs3 << "RegionForLane1"<< bLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs3 << "RegionForLane2" << refPtMat_RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fs2.release();
        fs3.release();
    }
}

void MainWindow::drawTAROI(){
    //    std::string sFolderPath = "D:\\AI-Center\\DMT\\myROIs\\resource\\input\\sample-camera-ROIs\\camera01\\";
    std::string sLocation = qstringToString(ui->lineEdit_2->text())+"/";
    cv::Mat matImage =  myArrayBuffer.last();

    std::cout << "-----Draw TA ROI-----" << std::endl;
    cv::FileStorage fs2;
    fs2.open("ROI_ta_use.roi", cv::FileStorage::READ);
    if (!fs2.isOpened())
    {
        QMessageBox::critical(nullptr, "Error", "Config file not found.");
    }
    cv::Mat mROI;
    fs2["ProcessingRegion0"] >> mROI;
    std::cout << "mROI: " << mROI << std::endl;

    cv::Mat mLine;
    fs2["RegionWithCountingLines0"] >> mLine;

    int iNumLaneRegions = 0;
    fs2["numRegionForLane"] >> iNumLaneRegions;
    std::vector< std::vector<cv::Point> > vvTmpPoint;
    std::vector<cv::Point> vTmpPoint;

    std::time_t t = std::time(nullptr); // get current time
    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d%H%M%S", std::localtime(&t)); // format time as a string
    std::string sOutputFile = sLocation + "_" + buffer + "_my_ta_roi_new.jpg"; // add time to output file name

    cv::Mat mImage;
    matImage.copyTo(mImage);
    cv::Mat mOverlay;
    double alpha = 0.3;
    mImage.copyTo(mOverlay);

    for (int k = 0; k < mROI.rows; k++)
    {
        vTmpPoint.push_back(cv::Point(mROI.at<int>(k, 0), mROI.at<int>(k, 1)));
        std::cout << "vTmpPoint: " << vTmpPoint.at(k) << std::endl;
    }
    vvTmpPoint.push_back(vTmpPoint);
    cv::fillPoly( mOverlay, vvTmpPoint, cv::Scalar(0,255,0,0.3));
    cv::addWeighted(mOverlay, alpha, mImage, 1 - alpha, 0, mImage);  // blending the overlay (with alpha opacity) with the source image (with 1-alpha opacity)


    vTmpPoint.clear();
    for (int k=0; k < mLine.rows; k++)
    {
        vTmpPoint.push_back(cv::Point(mLine.at<int>(k, 0), mLine.at<int>(k, 1)));
    }

    for (int k=0; k < vTmpPoint.size(); k++)
    {
        if (k < (vTmpPoint.size()-1))
        {
            cv::line(mImage, vTmpPoint.at(k), vTmpPoint.at(k+1), cv::Scalar(255,0,0), 3, 0 );
        }
    }

    for (int i = 0; i < iNumLaneRegions; i++)
    {
        std::stringstream ss;
        ss << "RegionForLane" << i;
        cv::Mat mLaneROI;

        fs2[ss.str()] >> mLaneROI;

        vTmpPoint.clear();
        for (int k = 0; k < mLaneROI.rows; k++)
        {
            vTmpPoint.push_back(cv::Point(mLaneROI.at<int>(k, 0), mLaneROI.at<int>(k, 1)));
        }

        for (int k=0; k < vTmpPoint.size(); k++)
        {
            if (k < (vTmpPoint.size()-1))
            {
                cv::line(mImage, vTmpPoint.at(k), vTmpPoint.at(k+1), cv::Scalar(0,0,255), 3, 0 );
            }
            if (k == (vTmpPoint.size()-1))
            {
                cv::line(mImage, vTmpPoint.at(k), vTmpPoint.at(0), cv::Scalar(0,0,255), 3, 0 );
            }

        }

    }

    cv::imwrite(sOutputFile, mImage);

    mROI.release();
    mOverlay.release();
    mImage.release();

    ui->textBrowser->append("TA Image was save");
}

void MainWindow::drawHAROI(){
    std::string sLocation = qstringToString(ui->lineEdit_2->text())+"/";
    cv::Mat matImage =  myArrayBuffer.last();
    std::cout << "-----Draw HA ROI-----" << std::endl;
    cv::FileStorage fs2;

    // ROI file
    fs2.open("ROI_ha.roi", cv::FileStorage::READ);
    if (!fs2.isOpened())
    {
        std::cerr << "Config file not found." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Config file not found.");
    }
    else{
        std::cout << "ROI File is alive" << std::endl;
    }
    cv::Mat mROI;
    fs2["ProcessingRegion0"] >> mROI;
    std::cout << "mROI: " << mROI << std::endl;

    std::vector< std::vector<cv::Point> > vvTmpPoint;
    std::vector<cv::Point> vTmpPoint;

    std::time_t t = std::time(nullptr); // get current time
    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d%H%M%S", std::localtime(&t)); // format time as a string
    std::string sOutputFile = sLocation + "_" + buffer + "_ha_roi_new.jpg"; // add time to output file name

    cv::Mat mImage;
    matImage.copyTo(mImage);
    cv::Mat mOverlay;
    double alpha = 0.3;
    mImage.copyTo(mOverlay);

    for (int k = 0; k < mROI.rows; k++)
    {
        vTmpPoint.push_back(cv::Point((int)mROI.at<int>(k, 0), (int)mROI.at<int>(k, 1)));
    }
    vvTmpPoint.push_back(vTmpPoint);


    cv::fillPoly( mOverlay, vvTmpPoint, cv::Scalar(0,255,0,0.3));

    cv::addWeighted(mOverlay, alpha, mImage, 1 - alpha, 0, mImage);  // blending the overlay (with alpha opacity) with the source image (with 1-alpha opacity)


    cv::imwrite(sOutputFile, mImage);

    mROI.release();
    mOverlay.release();
    mImage.release();

    std::cout << "Image was draw ROI" << std::endl;
    ui->textBrowser->append("HA Image was save");
}

void MainWindow::drawIPROI(){
    std::string sLocation =  qstringToString(ui->lineEdit_2->text())+"/";
    cv::Mat matImage =  myArrayBuffer.last();
    std::cout << "-----Draw IP ROI-----" << std::endl;
    cv::FileStorage fs2;
    fs2.open("ROI_ip.roi", cv::FileStorage::READ);
    if (!fs2.isOpened())
    {
        std::cerr << "Config file not found." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Config file not found.");
    }
    cv::Mat mROI;
    fs2["ProcessingRegion0"] >> mROI;
    std::cout << "mROI: " << mROI << std::endl;

    std::vector< std::vector<cv::Point> > vvTmpPoint;
    std::vector<cv::Point> vTmpPoint;

    std::time_t t = std::time(nullptr); // get current time
    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d%H%M%S", std::localtime(&t)); // format time as a string
    std::string sOutputFile = sLocation + "_" + buffer + "_ip_roi_new.jpg"; // add time to output file nam

    cv::Mat mImage;
    matImage.copyTo(mImage);
    cv::Mat mOverlay;
    double alpha = 0.3;
    mImage.copyTo(mOverlay);

    for (int k = 0; k < mROI.rows; k++)
    {
        vTmpPoint.push_back(cv::Point((int)mROI.at<int>(k, 0), (int)mROI.at<int>(k, 1)));
    }
    vvTmpPoint.push_back(vTmpPoint);


    cv::fillPoly( mOverlay, vvTmpPoint, cv::Scalar(0,255,0,0.3));

    cv::addWeighted(mOverlay, alpha, mImage, 1 - alpha, 0, mImage);  // blending the overlay (with alpha opacity) with the source image (with 1-alpha opacity)


    cv::imwrite(sOutputFile, mImage);

    mROI.release();
    mOverlay.release();
    mImage.release();

    ui->textBrowser->append("IP Image was save");
}

void MainWindow::drawWWROI(){

    std::string sLocation = qstringToString(ui->lineEdit_2->text())+"/";
    cv::Mat matImage =  myArrayBuffer.last();

    std::cout << "-----Draw WW ROI-----" << std::endl;
    cv::FileStorage fs2;
    fs2.open("ROI_ww.roi", cv::FileStorage::READ);
    if (!fs2.isOpened())
    {
        std::cerr << "Config file not found." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Config file not found.");
    }
    cv::Mat mROI;
    fs2["ProcessingRegion0"] >> mROI;
    std::cout << "mROI: " << mROI << std::endl;

    cv::Mat mLine;
    fs2["RegionWithCountingLines0"] >> mLine;

    int iNumLaneRegions = 0;
    fs2["numRegionForLane"] >> iNumLaneRegions;
    std::vector< std::vector<cv::Point> > vvTmpPoint;
    std::vector<cv::Point> vTmpPoint;

    std::time_t t = std::time(nullptr); // get current time
    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d%H%M%S", std::localtime(&t)); // format time as a string
    std::string sOutputFile = sLocation + "_" + buffer + "_ww_roi_new.jpg"; // add time to output file nam

    cv::Mat mImage;
    matImage.copyTo(mImage);
    cv::Mat mOverlay;
    double alpha = 0.3;
    mImage.copyTo(mOverlay);

    for (int k = 0; k < mROI.rows; k++)
    {
        vTmpPoint.push_back(cv::Point((int)mROI.at<int>(k, 0), (int)mROI.at<int>(k, 1)));
    }
    vvTmpPoint.push_back(vTmpPoint);


    cv::fillPoly( mOverlay, vvTmpPoint, cv::Scalar(0,255,0,0.3));

    cv::addWeighted(mOverlay, alpha, mImage, 1 - alpha, 0, mImage);  // blending the overlay (with alpha opacity) with the source image (with 1-alpha opacity)


    cv::imwrite(sOutputFile, mImage);

    mROI.release();
    mOverlay.release();
    mImage.release();

    ui->textBrowser->append("WW Image was save");
}


void MainWindow::on_btn_draw_buff_ta_clicked()
{
    QString ProcessingRegion0 = ui->lineEdit_ProcessingRegion0->text();
    QString VehicleDetectionRegion0 = ui->lineEdit_VehicleDetectionRegion0->text();
    QString RegionWithCountingLines0 = ui->lineEdit_RegionWithCountingLines0->text();
    QString RegionForLane0 = ui->lineEdit_RegionForLane0->text();
    QString RegionForLane1 = ui->lineEdit_RegionForLane1->text();
    QString RegionForLane2 = ui->lineEdit_RegionForLane2->text();

    if(myArrayBuffer.empty() == true){
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start.");
    }
    else{
        if(ui->lineEdit_2->text().isEmpty()){
            QMessageBox::critical(nullptr, "Error", "Please setup output path.");
        }
        else if(ProcessingRegion0.isEmpty() or VehicleDetectionRegion0.isEmpty() or RegionWithCountingLines0.isEmpty() or RegionForLane0.isEmpty() or RegionForLane1.isEmpty() or RegionForLane2.isEmpty()){
            QMessageBox::critical(nullptr, "Error", "TA's value is not complete. Please fill the value.");
        }
        else{
            cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

            cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

            cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

            cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

            cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

            cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

            // Print the cv::Mat
            std::cout << coordinates_ProcessingRegion0 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::cout << coordinates_VehicleDetectionRegion0 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::cout << coordinates_RegionWithCountingLines0 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::cout << coordinates_RegionForLane0 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::cout << coordinates_RegionForLane1 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::cout << coordinates_RegionForLane2 << std::endl;
            std::cout << "-----------------------------" << std::endl;

            //    if(){

            //    }

            cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);
            //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs1.isOpened())
            {
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }
            fs1 << "numProcessingRegion" << 1;
            fs1 << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
            //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

            fs1 << "numProcessingRegion" << 1;
            fs1 << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
            //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

            fs1 << "numProcessingRegion" << 1;
            fs1 << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
            //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));


            fs1 << "numRegionForLane" << 3;
            fs1 << "RegionForLane0" << coordinates_RegionForLane0;
            //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

            // this value
            fs1 << "RegionForLane1"<< coordinates_RegionForLane1;
            //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


            fs1 << "RegionForLane2" << coordinates_RegionForLane2;
            //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

            cv::FileStorage fs2(qstringToString(ui->lineEdit_2->text()+"/"+"ROI_ta_use.roi"), cv::FileStorage::WRITE);
            //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs2.isOpened())
            {
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }
            fs2 << "numProcessingRegion" << 1;
            fs2 << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
            //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

            fs1 << "numProcessingRegion" << 1;
            fs1 << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
            //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

            fs2 << "numProcessingRegion" << 1;
            fs2 << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
            //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));


            fs2 << "numRegionForLane" << 3;
            fs2 << "RegionForLane0" << coordinates_RegionForLane0;
            //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

            // this value
            fs2 << "RegionForLane1"<< coordinates_RegionForLane1;
            //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


            fs2 << "RegionForLane2" << coordinates_RegionForLane2;
            //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

            fs1.release();
            fs2.release();

            drawTAROI();
        }
    }
}


void MainWindow::on_btn_write_ta_clicked()
{
    //    ui->textBrowser->append("ta roi lane_back default was save");
    QString myPath = ui->lineEdit_get_origin_ta->text();
    cv::Mat mROI0, mROI1, mROI2, mROI3, mROI4, mROI5;
    if(myPath.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Please get TA ROI file.");
    }
    else{
        cv::FileStorage fs(qstringToString(myPath), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "Failed to open file." << std::endl;
            QMessageBox::critical(nullptr, "Error", "Failed to open file.");
        }
        fs["ProcessingRegion0"] >> mROI0;
        fs["VehicleDetectionRegion0"] >> mROI1;
        fs["RegionWithCountingLines0"] >> mROI2;


        int iNumLaneRegions = 0;
        fs["numRegionForLane"] >> iNumLaneRegions;
        std::vector<std::vector<cv::Point>> vvTmpPoint;
        std::vector<cv::Point> vTmpPoint;

        for (int i = 0; i < iNumLaneRegions; i++)
        {
            std::stringstream ss;
            ss << "RegionForLane" << i;
            cv::Mat mLaneROI;

            fs[ss.str()] >> mLaneROI;

            vTmpPoint.clear();
            for (int k = 0; k < mLaneROI.rows; k++)
            {
                cv::Point point(mLaneROI.at<int>(k, 0), mLaneROI.at<int>(k, 1));
                vTmpPoint.push_back(point);
            }

            vvTmpPoint.push_back(vTmpPoint);
        }

        // Store the collected points in separate cv::Mat variables
        if (vvTmpPoint.size() > 0)
        {
            mROI3 = cv::Mat(vvTmpPoint[0]);
        }
        if (vvTmpPoint.size() > 1)
        {
            mROI4 = cv::Mat(vvTmpPoint[1]);
        }
        if (vvTmpPoint.size() > 2)
        {
            mROI5 = cv::Mat(vvTmpPoint[2]);
        }

        qDebug() << string_to_qstring(matToString(mROI3));
        qDebug() << "---------------------------------------------";
        qDebug() << string_to_qstring(matToString(mROI4));
        qDebug() << "---------------------------------------------";
        qDebug() << string_to_qstring(matToString(mROI5));
        qDebug() << "--------------------END----------------------";


        //    std::cout << "-------------END------------------" << std::endl;

        //    cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);

        //    fs1  << "numProcessingRegion" << 1;
        //    fs1 << "ProcessingRegion0"<< mROI0;
        ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        //    fs1  << "numVehicleDetectionRegion" << 1;
        //    fs1 << "VehicleDetectionRegion0"<< mROI1;
        ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        //    fs1  << "numRegionWithCountingLines" << 1;
        //    fs1 << "RegionWithCountingLines0"<< mROI2;
        ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        //    fs1  << "numRegionForLane" << 3;
        //    fs1 << "RegionForLane0"<< mROI3;
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        //    fs << "RegionForLane1"<< mROI4;
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));

        //    fs1 << "RegionForLane2"<< mROI4;
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        // Back up lane data
        //    cv::Mat bLane0, bLane1, bLane2;
        //    cv::FileStorage fs2;
        //    fs2.open("ROI_ta_lane_back.roi", cv::FileStorage::READ);
        //    if (!fs2.isOpened())
        //    {
        //       QMessageBox::critical(nullptr, "Error", "Config file not found.");
        //    }
        //    fs2["RegionForLane0"] >> bLane0;
        //    fs2["RegionForLane1"] >> bLane1;
        //    fs2["RegionForLane2"] >> bLane2;

        cv::FileStorage fs3("ROI_ta_lane_back.roi", cv::FileStorage::WRITE);

        fs3 << "numRegionForLane" << 3;
        fs3 << "RegionForLane0" << mROI3;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(refPtMat_RegionForLane0)));

        // this value
        fs3 << "RegionForLane1"<< mROI4;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs3 << "RegionForLane2" << mROI5;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(mROI5)));

        fs.release();
        //    fs1.release();
        fs3.release();
        ui->textBrowser->append("ta roi default was save");
    }
}


void MainWindow::on_btn_del_lane0_stateChanged(int arg1)
{


    //    cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

    if(arg1){
        //        testCheck++;
        ui->btn_del_lane0->setText("Distable lane0");
        ui->btn_RegionForLane0->setEnabled(false);
        ui->lineEdit_RegionForLane0->setEnabled(false);

        QString str;
        //        qDebug() << "testCheck button" << testCheck ;

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        //    cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

        cv::Mat RegionForLane0(1, 2, CV_32S);
        int RegionForLane0Data[] = {0,0};
        memcpy(RegionForLane0.data, RegionForLane0Data, sizeof(RegionForLane0Data));
        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(RegionForLane0)));

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fs1.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0" << RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fs1 << "RegionForLane1"<< coordinates_RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs1 << "RegionForLane2" << coordinates_RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fs1.release();
        qDebug() << "isEnable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane0_edit.last());



    }
    else{
        ui->btn_RegionForLane0->setEnabled(true);
        ui->lineEdit_RegionForLane0->setEnabled(true);
        ui->btn_del_lane0->setText("Enable lane0");

        //        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(refPtMat_RegionForLane0_edit.last())));
        qDebug() << "isDistable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane0_edit.last());

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        //    cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fsB;
        fsB.open("ROI_ta_lane_back.roi", cv::FileStorage::READ);
        if (!fsB.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsB["RegionForLane0"] >> bLane0;
        fsB["RegionForLane1"] >> bLane1;
        fsB["RegionForLane2"] >> bLane2;

        ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(bLane0)));

        cv::FileStorage fsW("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fsW.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsW << "numProcessingRegion" << 1;
        fsW << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fsW << "numProcessingRegion" << 1;
        fsW << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fsW << "numProcessingRegion" << 1;
        fsW << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fsW << "numRegionForLane" << 3;
        fsW << "RegionForLane0" << bLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fsW << "RegionForLane1"<< coordinates_RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fsW << "RegionForLane2" << coordinates_RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fsW.release();


    }
    //    refPtMat_RegionForLane0_edit.release();
    //    qDebug() <<  matToString(refPtMat_RegionForLane0_edit);



}


void MainWindow::on_btn_del_lane1_stateChanged(int arg1)
{
    //    cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

    if(arg1){
        ui->btn_del_lane1->setText("Distable lane1");
        ui->btn_RegionForLane1->setEnabled(false);
        ui->lineEdit_RegionForLane1->setEnabled(false);

//        QString str;
//        qDebug() << "testCheck button" << testCheck ;

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        //        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

        cv::Mat RegionForLane1(1, 2, CV_32S);
        int RegionForLane1Data[] = {0,0};
        memcpy(RegionForLane1.data, RegionForLane1Data, sizeof(RegionForLane1Data));
        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(RegionForLane1)));

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fs1.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0" << coordinates_RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fs1 << "RegionForLane1"<< RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs1 << "RegionForLane2" << coordinates_RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fs1.release();
        qDebug() << "isEnable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane1_edit.last());
    }
    else{
        ui->btn_RegionForLane1->setEnabled(true);
        ui->lineEdit_RegionForLane1->setEnabled(true);
        ui->btn_del_lane1->setText("Enable lane1");

        //        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(refPtMat_RegionForLane1_edit.last())));
        qDebug() << "isDistable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane1_edit.last());

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        //        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());


        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fsB;
        fsB.open("ROI_ta_lane_back.roi", cv::FileStorage::READ);
        if (!fsB.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsB["RegionForLane0"] >> bLane0;
        fsB["RegionForLane1"] >> bLane1;
        fsB["RegionForLane2"] >> bLane2;

        ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(bLane1)));

        cv::FileStorage fsW("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fsW.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsW << "numProcessingRegion" << 1;
        fsW << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fsW << "numProcessingRegion" << 1;
        fsW << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fsW << "numProcessingRegion" << 1;
        fsW << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fsW << "numRegionForLane" << 3;
        fsW << "RegionForLane0" << coordinates_RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fsW << "RegionForLane1"<< bLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fsW << "RegionForLane2" << coordinates_RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fsW.release();

    }
    //    refPtMat_RegionForLane1_edit.release();
}


void MainWindow::on_btn_del_lane2_stateChanged(int arg1)
{
    if(arg1){
        ui->btn_del_lane2->setText("Distable lane2");
        ui->btn_RegionForLane2->setEnabled(false);
        ui->lineEdit_RegionForLane2->setEnabled(false);

//        QString str;
//        qDebug() << "testCheck button" << testCheck ;

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        //        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

        cv::Mat RegionForLane2(1, 2, CV_32S);
        int RegionForLane2Data[] = {0,0};
        memcpy(RegionForLane2.data, RegionForLane2Data, sizeof(RegionForLane2Data));
        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(RegionForLane2)));

        cv::FileStorage fs1("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fs1.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fs1 << "numProcessingRegion" << 1;
        fs1 << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fs1 << "numRegionForLane" << 3;
        fs1 << "RegionForLane0" << coordinates_RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fs1 << "RegionForLane1"<< coordinates_RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fs1 << "RegionForLane2" << RegionForLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fs1.release();
        qDebug() << "isEnable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane2_edit);
    }
    else{
        ui->btn_RegionForLane2->setEnabled(true);
        ui->lineEdit_RegionForLane2->setEnabled(true);
        ui->btn_del_lane2->setText("Enable lane2");


        //        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2_edit)));
        qDebug() << "isDistable";
        //        qDebug() <<  matToString(refPtMat_RegionForLane2_edit);

        // Back up lane data
        cv::Mat bLane0, bLane1, bLane2;
        cv::FileStorage fsB;
        fsB.open("ROI_ta_lane_back.roi", cv::FileStorage::READ);
        if (!fsB.isOpened())
        {
            std::cerr << "Config file not found." << std::endl;
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsB["RegionForLane0"] >> bLane0;
        fsB["RegionForLane1"] >> bLane1;
        fsB["RegionForLane2"] >> bLane2;

        ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(bLane2)));

        cv::Mat coordinates_ProcessingRegion0 = convertQStringToMat(ui->lineEdit_ProcessingRegion0->text());

        cv::Mat coordinates_VehicleDetectionRegion0 = convertQStringToMat(ui->lineEdit_VehicleDetectionRegion0->text());

        cv::Mat coordinates_RegionWithCountingLines0 = convertQStringToMat(ui->lineEdit_RegionWithCountingLines0->text());

        cv::Mat coordinates_RegionForLane0 = convertQStringToMat(ui->lineEdit_RegionForLane0->text());

        cv::Mat coordinates_RegionForLane1 = convertQStringToMat(ui->lineEdit_RegionForLane1->text());

        cv::Mat coordinates_RegionForLane2 = convertQStringToMat(ui->lineEdit_RegionForLane2->text());

        cv::FileStorage fsW("ROI_ta_use.roi", cv::FileStorage::WRITE);
        //    fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fsW.isOpened())
        {
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }
        fsW << "numProcessingRegion" << 1;
        fsW << "ProcessingRegion0"<< coordinates_ProcessingRegion0;
        //    ui->lineEdit_ProcessingRegion0->setText(string_to_qstring(matToString(mROI0)));

        fsW << "numProcessingRegion" << 1;
        fsW << "VehicleDetectionRegion0" << coordinates_VehicleDetectionRegion0;
        //    ui->lineEdit_VehicleDetectionRegion0->setText(string_to_qstring(matToString(mROI1)));

        fsW << "numProcessingRegion" << 1;
        fsW << "RegionWithCountingLines0" << coordinates_RegionWithCountingLines0;
        //    ui->lineEdit_RegionWithCountingLines0->setText(string_to_qstring(matToString(mROI2)));

        // New Value
        fsW << "numRegionForLane" << 3;
        fsW << "RegionForLane0" << coordinates_RegionForLane0;
        //    ui->lineEdit_RegionForLane0->setText(string_to_qstring(matToString(mROI3)));

        // this value
        fsW << "RegionForLane1"<< coordinates_RegionForLane1;
        //    ui->lineEdit_RegionForLane1->setText(string_to_qstring(matToString(mROI4)));


        fsW << "RegionForLane2" << bLane2;
        //    ui->lineEdit_RegionForLane2->setText(string_to_qstring(matToString(refPtMat_RegionForLane2)));

        fsW.release();


    }
    //    refPtMat_RegionForLane2_edit.release();
}


//void MainWindow::on_pushButton_clicked()
//{

//}


void MainWindow::on_btn_cfg_ha_clicked()
{
    if (myArrayBuffer.empty()) {
        //        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }
    else{
        image_ha = myArrayBuffer.last();
        refPt_ha.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ha);

        while (true) {
            cv::imshow("Image", image_ha);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat(refPt_ha.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_ha.size(); ++i) {
            refPtMat.at<int>(i, 0) = static_cast<int>(refPt_ha[i].x);
            refPtMat.at<int>(i, 1) = static_cast<int>(refPt_ha[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat.at<int>(refPt_ha.size(), 0) = static_cast<int>(refPt_ha[0].x);
        refPtMat.at<int>(refPt_ha.size(), 1) = static_cast<int>(refPt_ha[0].y);

        std::cout << refPt_ha;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat;


        cv::FileStorage fs("ROI_ha.roi", cv::FileStorage::WRITE);
        fs  << "numProcessingRegion" << 1;
        fs << "ProcessingRegion0"<< refPtMat;
        fs.release();

//        ui->textBrowser->append(string_to_qstring(matToString(refPtMat)));
        ui->lineEdit_ha->setText(string_to_qstring(matToString(refPtMat)));
        std::cout << "Test ROI";

        ui->textBrowser->append("ROI ha value was set");
    }
}


void MainWindow::on_btn_get_oringin_ha_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Files (*.*)");
    QSettings settings("QtAICenter.ini", QSettings::Format::IniFormat);
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString filePath = fileInfo.absoluteFilePath();
        // Set the file path in a QLineEdit or similar widget
        ui->lineEdit_get_origin_ha->setText(filePath);
        settings.setValue("Sigle_Value_4", filePath);
    }

    ui->textBrowser->append("Path of HA config file was set");
}


void MainWindow::on_btn_draw_ha_clicked()
{
    QString ha_roi = ui->lineEdit_ha->text();

    if(myArrayBuffer.empty() == true){
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start.");
    }
    else{
        if(ui->lineEdit_2->text().isEmpty()){
            QMessageBox::critical(nullptr, "Error", "Please setup output path.");
        }
        else if(ha_roi.isEmpty()){
            QMessageBox::critical(nullptr, "Error", "WW's value is not complete. Please fill the value.");
        }
        else{
            cv::Mat coordinates_ha = convertQStringToMat(ui->lineEdit_ha->text());

            cv::FileStorage fs1("ROI_ha.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs1.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs1 << "numProcessingRegion" << 1;
            fs1 << "ProcessingRegion0" << coordinates_ha;


            cv::FileStorage fs2(qstringToString(ui->lineEdit_2->text())+"/"+"ROI_ha.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs2.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs2 << "numProcessingRegion" << 1;
            fs2 << "ProcessingRegion0" << coordinates_ha;

            fs1.release();

            fs2.release();

            drawWWROI();
        }
    }
}


void MainWindow::on_btn_set_ha_roi_clicked()
{

    QString myPath = ui->lineEdit_get_origin_ha->text();
    cv::Mat mROI0;

    if(myPath.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Please get HA ROI file.");
    }
    else{
        cv::FileStorage fs(qstringToString(myPath), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "Failed to open file." << std::endl;
            QMessageBox::critical(nullptr, "Message", "Failed to open file.");
        }
        else{
            fs["ProcessingRegion0"] >> mROI0;

            ui->lineEdit_ha->setText(string_to_qstring(matToString(mROI0)));

            fs.release();

            ui->textBrowser->append("ha roi default was saved");
        }
    }
}


void MainWindow::on_btn_cfg_ip_clicked()
{
    if (myArrayBuffer.empty()) {
        //        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }
    else{
        image_ip = myArrayBuffer.last();
        refPt_ip.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ip);

        while (true) {
            cv::imshow("Image", image_ip);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat(refPt_ip.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_ip.size(); ++i) {
            refPtMat.at<int>(i, 0) = static_cast<int>(refPt_ip[i].x);
            refPtMat.at<int>(i, 1) = static_cast<int>(refPt_ip[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat.at<int>(refPt_ip.size(), 0) = static_cast<int>(refPt_ip[0].x);
        refPtMat.at<int>(refPt_ip.size(), 1) = static_cast<int>(refPt_ip[0].y);

        std::cout << refPt_ip;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat;


        cv::FileStorage fs("ROI_ip.roi", cv::FileStorage::WRITE);
        fs  << "numProcessingRegion" << 1;
        fs << "ProcessingRegion0"<< refPtMat;
        fs.release();

        ui->textBrowser->append(string_to_qstring(matToString(refPtMat)));
        ui->lineEdit_ip->setText(string_to_qstring(matToString(refPtMat)));
        std::cout << "Test ROI";

        ui->textBrowser->append("ROI ip value was set");
    }
}


void MainWindow::on_btn_get_oringin_ip_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Files (*.*)");
    QSettings settings("QtAICenter.ini", QSettings::Format::IniFormat);
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString filePath = fileInfo.absoluteFilePath();
        // Set the file path in a QLineEdit or similar widget
        ui->lineEdit_get_origin_ip->setText(filePath);
        settings.setValue("Sigle_Value_5", filePath);
    }

    ui->textBrowser->append("Path of IP config file was set");
}


void MainWindow::on_btn_draw_ip_clicked()
{
    QString ip_roi = ui->lineEdit_ip->text();

    if(myArrayBuffer.empty() == true){
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start.");
    }
    else{
        if(ui->lineEdit_2->text().isEmpty()){
            QMessageBox::critical(nullptr, "Error", "Please setup output path.");
        }
        else if(ip_roi.isEmpty()){
            QMessageBox::critical(nullptr, "Error", "IP's value is not complete. Please fill the value.");
        }
        else{
            cv::Mat coordinates_ip = convertQStringToMat(ui->lineEdit_ip->text());

            cv::FileStorage fs1(qstringToString(ui->lineEdit_2->text())+"/"+"ROI_ip.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs1.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs1 << "numProcessingRegion" << 1;
            fs1 << "ProcessingRegion0" << coordinates_ip;

            cv::FileStorage fs2("ROI_ip.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs2.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs2 << "numProcessingRegion" << 1;
            fs2 << "ProcessingRegion0" << coordinates_ip;

            fs1.release();

            fs2.release();

            drawIPROI();
        }
    }
}


void MainWindow::on_btn_set_ip_roi_clicked()
{
//    cv::FileStorage fs("ROI_ip.roi", cv::FileStorage::WRITE);

//    cv::Mat processingRegion(5, 2, CV_32S);
//    int processingRegionData[] = {459, 235, 1238, 199, 1915, 609, 1916, 1078, 537, 1078};
//    memcpy(processingRegion.data, processingRegionData, sizeof(processingRegionData));
//    fs << "numProcessingRegion" << 1;
//    fs << "ProcessingRegion0" << processingRegion;
//    ui->lineEdit_ip->setText(string_to_qstring(matToString(processingRegion)));

//    fs.release();
//    ui->textBrowser->append("ip roi default was saved");
    QString myPath = ui->lineEdit_get_origin_ip->text();
    cv::Mat mROI0;

    if(myPath.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Please get IP ROI file.");
    }
    else{
        cv::FileStorage fs(qstringToString(myPath), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "Failed to open file." << std::endl;
            QMessageBox::critical(nullptr, "Message", "Failed to open file.");
        }
        else{
            fs["ProcessingRegion0"] >> mROI0;

            ui->lineEdit_ip->setText(string_to_qstring(matToString(mROI0)));

            fs.release();

            ui->textBrowser->append("ip roi default was saved");
        }
    }
}


void MainWindow::on_btn_cfg_ww_clicked()
{
    if (myArrayBuffer.empty()) {
        std::cout << "Failed to load the image." << std::endl;
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start");
        // return -1;
    }
    else{
        image_ww = myArrayBuffer.last();
        refPt_ww.clear();

        cv::namedWindow("Image", 0);
        cv::setMouseCallback("Image", click_event_ww);

        while (true) {
            cv::imshow("Image", image_ww);
            if (cv::waitKey(1) == 32) // Press Esc to exit the loop
                break;
        }

        cv::destroyAllWindows();

        cv::Mat refPtMat(refPt_ww.size(), 2, CV_32S);
        for (size_t i = 0; i < refPt_ww.size(); ++i) {
            refPtMat.at<int>(i, 0) = static_cast<int>(refPt_ww[i].x);
            refPtMat.at<int>(i, 1) = static_cast<int>(refPt_ww[i].y);
        }

        // Append refPt[0].x and refPt[0].y to the last row of refPtMat
        refPtMat.at<int>(refPt_ww.size(), 0) = static_cast<int>(refPt_ww[0].x);
        refPtMat.at<int>(refPt_ww.size(), 1) = static_cast<int>(refPt_ww[0].y);

        std::cout << refPt_ww;

        std::cout << "--------------------------" << std::endl;

        std::cout << refPtMat;


        cv::FileStorage fs("ROI_ww.roi", cv::FileStorage::WRITE);
        fs  << "numProcessingRegion" << 1;
        fs << "ProcessingRegion0"<< refPtMat;
        fs.release();

        ui->textBrowser->append(string_to_qstring(matToString(refPtMat)));
        ui->lineEdit_ww->setText(string_to_qstring(matToString(refPtMat)));
        std::cout << "Test ROI";

        ui->textBrowser->append("ROI ww value was set");
    }
}


void MainWindow::on_btn_get_oringin_ww_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Files (*.*)");
    QSettings settings("QtAICenter.ini", QSettings::Format::IniFormat);
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString filePath = fileInfo.absoluteFilePath();
        // Set the file path in a QLineEdit or similar widget
        ui->lineEdit_get_origin_ww->setText(filePath);
        settings.setValue("Sigle_Value_6", filePath);
    }

    ui->textBrowser->append("Path of WW config file was set");
}


void MainWindow::on_btn_draw_ww_clicked()
{
    QString ww_roi = ui->lineEdit_ww->text();

    if(myArrayBuffer.empty() == true){
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start.");
    }
    else{
        if(ui->lineEdit_2->text().isEmpty()){
          QMessageBox::critical(nullptr, "Error", "Please setup output path.");
        }
        else if(ww_roi.isEmpty()){
            QMessageBox::critical(nullptr, "Error", "WW's value is not complete. Please fill the value.");
        }
        else{
            cv::Mat coordinates_ww = convertQStringToMat(ui->lineEdit_ww->text());

            cv::FileStorage fs1("ROI_ww.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs1.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs1 << "numProcessingRegion" << 1;
            fs1 << "ProcessingRegion0" << coordinates_ww;

            cv::FileStorage fs2(qstringToString(ui->lineEdit_2->text())+"/"+"ROI_ww.roi", cv::FileStorage::WRITE);
            // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
            if (!fs2.isOpened()) {
                std::cerr << "Config file not found." << std::endl;
                QMessageBox::critical(nullptr, "Error", "Config file not found.");
            }

            fs2 << "numProcessingRegion" << 1;
            fs2 << "ProcessingRegion0" << coordinates_ww;

            fs1.release();

            fs2.release();

            drawWWROI();
        }
    }
}


void MainWindow::on_btn_set_ww_roi_clicked()
{
//    cv::FileStorage fs("ROI_ww.roi", cv::FileStorage::WRITE);

//    cv::Mat processingRegion(5, 2, CV_32S);
//    int processingRegionData[] = {459, 235, 1238, 199, 1915, 609, 1916, 1078, 537, 1078};
//    memcpy(processingRegion.data, processingRegionData, sizeof(processingRegionData));
//    fs << "numProcessingRegion" << 1;
//    fs << "ProcessingRegion0" << processingRegion;
//    ui->lineEdit_ww->setText(string_to_qstring(matToString(processingRegion)));

//    fs.release();
//    ui->textBrowser->append("ww roi default was saved");
    QString myPath = ui->lineEdit_get_origin_ww->text();
    cv::Mat mROI0;

    if(myPath.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Please get WW ROI file.");
    }
    else{
        cv::FileStorage fs(qstringToString(myPath), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cout << "Failed to open file." << std::endl;
            QMessageBox::critical(nullptr, "Message", "Failed to open file.");
        }
        else{
            fs["ProcessingRegion0"] >> mROI0;

            ui->lineEdit_ww->setText(string_to_qstring(matToString(mROI0)));

            fs.release();

            ui->textBrowser->append("ww roi default was saved");
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    if(myArrayBuffer.empty() == true){
        QMessageBox::critical(nullptr, "Error", "Image is empty please click start.");
    }
    else{
        cv::Mat coordinates_ww = convertQStringToMat(ui->lineEdit_ww->text());

        cv::FileStorage fs1("ROI_ww.roi", cv::FileStorage::WRITE);
        // fs1.open("XX_ta_Buff_new.roi", cv::FileStorage::READ);
        if (!fs1.isOpened()) {
            std::cerr << "Config file not found." << std::endl;
            QMessageBox::critical(nullptr, "Error", "Config file not found.");
        }

        fs1 << "numProcessingRegion" << 1;
        fs1 << "ProcessingRegion0" << coordinates_ww;

        fs1.release();

        drawWWROI();
    }
}


void MainWindow::on_btn_save_img_clicked()
{
    if (myArrayBuffer.empty() == true) {
        QMessageBox::critical(nullptr, "Message", "Image is empty please click start");
            // return -1;
    }
    else{
        on_btn_draw_buff_ta_clicked();
        on_btn_draw_ha_clicked();
        on_btn_draw_ip_clicked();
        on_btn_draw_ww_clicked();

//        ui->textBrowser->append("----TA HA IP WW images were save----");
    }
}

