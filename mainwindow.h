#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
//    void on_pushButton_3_clicked();

//    void on_pushButton_4_clicked();

    void closeEvent(QCloseEvent *event);

    void on_btn_start_cam_clicked();

    void on_btn_stop_cam_clicked();

    void on_btn_output_path_clicked();

    void on_btn_get_oringin_ta_clicked();

    void on_btn_set_ip_clicked();

    void on_btn_ProcessingRegion0_clicked();

    void on_bnt_VehicleDetectionRegion0_clicked();

    void on_btn_RegionWithCountingLines0_clicked();

    void on_btn_RegionForLane0_clicked();

    void on_btn_RegionForLane1_clicked();

    void on_btn_RegionForLane2_clicked();

    void on_btn_draw_buff_ta_clicked();

    void on_btn_write_ta_clicked();

    void on_btn_del_lane0_stateChanged(int arg1);

    void on_btn_del_lane1_stateChanged(int arg1);

    void on_btn_del_lane2_stateChanged(int arg1);

//    void on_pushButton_clicked();

    void on_btn_cfg_ha_clicked();

    void on_btn_get_oringin_ha_clicked();

    void on_btn_draw_ha_clicked();

    void on_btn_set_ha_roi_clicked();

    void on_btn_cfg_ip_clicked();

    void on_btn_get_oringin_ip_clicked();

    void on_btn_draw_ip_clicked();

    void on_btn_set_ip_roi_clicked();

    void on_btn_cfg_ww_clicked();

    void on_btn_get_oringin_ww_clicked();

    void on_btn_draw_ww_clicked();

    void on_btn_set_ww_roi_clicked();

    void on_pushButton_3_clicked();

    void on_btn_save_img_clicked();

private:
    Ui::MainWindow *ui;

    // Convert string to Qstring
    QString string_to_qstring(const std::string& str);

    // Convert Qstring to string
    std::string qstringToString(const QString& qstr);

    void drawTAROI();
    void drawHAROI();
    void drawIPROI();
    void drawWWROI();

    void setCheckbutton();
    void getConfigfile();


    void readTAROI();
    void readHAROI();
    void readIPROI();
    void readWWROI();


    void readINI();
    void readINI_ipCam();
    void readINI_Output_Dir();
    void readINI_ta();
    void readINI_ha();
    void readINI_ip();
    void readINI_ww();
};
#endif // MAINWINDOW_H
