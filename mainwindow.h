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
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    // Convert string to Qstring
    QString string_to_qstring(const std::string& str);

    // Convert Qstring to string
    std::string qstringToString(const QString& qstr);
};
#endif // MAINWINDOW_H
