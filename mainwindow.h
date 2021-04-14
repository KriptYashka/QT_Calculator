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
    void set_number(QString num);
    void setSign(QString num);
    void get_digit(QString str, int select_digit);
    QString sum_digit(int select);

    void on_btn_0_clicked();
    void on_btn_1_clicked();
    void on_btn_2_clicked();
    void on_btn_3_clicked();
    void on_btn_4_clicked();
    void on_btn_5_clicked();
    void on_btn_6_clicked();
    void on_btn_7_clicked();
    void on_btn_8_clicked();
    void on_btn_9_clicked();

    void on_btn_plus_clicked();
    void on_btn_minus_clicked();
    void on_btn_multiply_clicked();
    void on_btn_divide_clicked();
    void on_btn_equal_clicked();
    void on_btn_dot_clicked();

    void on_btn_clear_clicked();
    void on_btn_back_clicked();
    void on_btn_sign_clicked();

    void disable_btn(bool flag);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
