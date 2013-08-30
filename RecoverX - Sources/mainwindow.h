#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QHttp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void downloadmanifest();
    void parsexmlupdate();
    void parsexmlbrand();
    void parsexmldevice();
    void parsexmlrecovery();
    void parsexmlinfo();
    void parsexmlbeta();
    void userdialog();
    void downloaddialog();
    void warningdialog();
    void installdialog();

    void installimg();
    void installelf();
    void installflash();
    void installzip();
    void installbusybox();
    void checkdevice();
    void bootloader();
    void waitdevice();
    void delrecovery();
    void value();
    void startdownload();

public slots:
    void requestFinished(int id, bool error);
    
private slots:

    void on_comboBox_activated(const QString &arg1);
    void on_comboBox_2_activated(const QString &arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

private:

    QHttp *http;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
