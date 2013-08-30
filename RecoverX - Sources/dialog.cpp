#include "dialog.h"
#include "ui_dialog.h"
#include "license.h"
#include "ui_license.h"

#include <QtGui>

QString dialogboolallow;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ui->label_2->setText("");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    close();
}

void Dialog::on_pushButton_2_clicked()
{
    QProcess kill;
    kill.start("bbin\\adb kill-server");
    kill.waitForFinished();

    ui->label_2->setText("Adb server killed");
}

void Dialog::on_pushButton_3_clicked()
{
    QProcess bootloader;
    bootloader.start("bbin\\adb reboot-bootloader");
    bootloader.waitForFinished();

    ui->label_2->setText("Device into fastboot mode");
}

void Dialog::on_label_4_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://forum.xda-developers.com/member.php?u=4617223"));
}

void Dialog::on_label_5_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/changelog.html"));
}

void Dialog::on_label_6_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
}

void Dialog::on_label_7_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca"));
}

void Dialog::on_label_9_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/form.php"));
}

void Dialog::on_label_10_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/bugreport.php"));
}

void Dialog::on_pushButton_4_clicked()
{
    License lcs;
    lcs.exec();
}
