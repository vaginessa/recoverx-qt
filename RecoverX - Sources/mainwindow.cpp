#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "license.h"
#include "ui_license.h"

#include <QtGui>
#include <QString>
#include <QDomDocument>
#include <windows.h>

int stepnumber(0);
int progress(0);
int limit(0);
int unlocked(0);

QString currentversion("1690");
QString attupdate;
QString updateversion;
QString manifest;

QString brand;
QString device;
QString recovery;
QString host;
QString link;
QString type;

QString attdevice;
QString attrecovery;
QString atthost;
QString attlink;
QString atttype;
QString attbetaone;
QString attbetatwo;
QString attbetathree;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    QFile xmlhere("xml.exe");

    if(xmlhere.open(QIODevice::ReadOnly))
    {
        xmlhere.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 6", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlhere.close();
        exit(0);
    }

    // Check if the program is launched by the user for the first time
    QFile stats("bbin/stats.xml");

    if(stats.open(QIODevice::ReadOnly))
    {
        stats.close();
    }
    else
    {
        QFile create("bbin/stats.xml");
        create.open(QFile::WriteOnly);
        create.close();
        stats.close();

        QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/stats.html"));

        License lcs;
        lcs.exec();
    }

    downloadmanifest();

    parsexmlupdate();
    parsexmlbrand();
    parsexmldevice();
    parsexmlrecovery();
    parsexmlbeta();

    if (ui->comboBox->currentText() == "" && ui->comboBox_2->currentText() == "")
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 7", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }

    userdialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downloadmanifest()
{
    // Start xml.exe
    QProcess startxml;
    startxml.start("xml.exe");
    startxml.waitForFinished();
}

void MainWindow::bootloader()
{
    if (ui->checkBox->isChecked())
    {

    }
    else
    {
        if (type == "img")
        {
            int askbootloader = QMessageBox::information(this, "RecoverX", "Does your device have an unlocked bootloader ?", QMessageBox::Yes | QMessageBox::No);

            if (askbootloader == QMessageBox::Yes)
            {
                unlocked = 1;
            }
        }
    }
}

void MainWindow::parsexmlupdate()
{
    QFile xmlfile("bbin/manifest.xml");

    if(xmlfile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        doc.setContent(&xmlfile, false);

        QDomElement root = doc.documentElement();

        root = root.firstChildElement();

        while(!root.isNull())
        {
            if(root.tagName() == "update:list")
            {
                QDomElement updatetag = root.firstChildElement();

                while(!updatetag.isNull())
                {
                    if (updatetag.tagName() == "stable")
                    {
                        attupdate = updatetag.attribute("num");

                        if (attupdate > currentversion)
                        {
                            updateversion = updatetag.text();

                            int askupdate = QMessageBox::information(this, "RecoverX", "An update of RecoverX is available (" + updateversion + "). Do you want to download it ?", QMessageBox::Yes | QMessageBox::No);

                            if (askupdate == QMessageBox::Yes)
                            {
                                QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/download.html"));
                                exit(0);
                            }
                        }
                    }
                    else if (updatetag.tagName() == "manifest")
                    {
                        manifest = updatetag.text();

                        ui->label_4->setText("Manifest version : " + manifest);
                    }
                    updatetag = updatetag.nextSiblingElement();
                }
            }
            root = root.nextSiblingElement();
        }
        xmlfile.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 1", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlfile.close();
        exit(0);
    }
}

void MainWindow::parsexmlbrand()
{
    QFile xmlfile("bbin/manifest.xml");

    if (xmlfile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        doc.setContent(&xmlfile, false);

        QDomElement root = doc.documentElement();

        root = root.firstChildElement();

        while (!root.isNull())
        {
            if (root.tagName() == "brand:list")
            {
                QDomElement brandtag = root.firstChildElement();

                while (!brandtag.isNull())
                {                                       
                    if (brandtag.tagName() == "brand")
                    {
                        brand = brandtag.text();
                        ui->comboBox->addItem(brand);
                    }
                    brandtag = brandtag.nextSiblingElement();
                }
            }
            root = root.nextSiblingElement();
        }
        xmlfile.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 1", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlfile.close();
        exit(0);
    }
}

void MainWindow::parsexmldevice()
{
    ui->comboBox_2->clear();

    QFile xmlfile("bbin/manifest.xml");

    if (xmlfile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        doc.setContent(&xmlfile, false);

        QDomElement root = doc.documentElement();

        root = root.firstChildElement();

        while (!root.isNull())
        {
            if (root.tagName() == "device:list")
            {
                QDomElement devicetag = root.firstChildElement();

                while (!devicetag.isNull())
                {                  
                    if (devicetag.tagName() == "device")
                    {
                        device = devicetag.text();

                        attdevice = devicetag.attribute("brand");

                        if (ui->comboBox->currentText() == attdevice)
                        {
                            ui->comboBox_2->addItem(device);
                        }
                    }
                    devicetag = devicetag.nextSiblingElement();
                }
            }
            root = root.nextSiblingElement();
        }
        xmlfile.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 1", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlfile.close();
        exit(0);
    }


}

void MainWindow::parsexmlrecovery()
{
    ui->radioButton->setChecked(true);
    ui->radioButton_2->hide();
    ui->radioButton_3->hide();

    QFile xmlfile("bbin/manifest.xml");

    if(xmlfile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        doc.setContent(&xmlfile, false);

        QDomElement root = doc.documentElement();

        root = root.firstChildElement();

        while (!root.isNull())
        {
            if (root.tagName() == "recovery:list")
            {
                QDomElement recoverytag = root.firstChildElement();

                while (!recoverytag.isNull())
                {
                    if (recoverytag.tagName() == "recovery1")
                    {
                        attrecovery = recoverytag.attribute("id");

                        if (ui->comboBox_2->currentText() == attrecovery)
                        {
                            recovery = recoverytag.text();
                            ui->radioButton->setText(recovery);

                            attbetaone = recoverytag.attribute("beta");
                        }
                    }
                    else if (recoverytag.tagName() == "recovery2")
                    {
                        attrecovery = recoverytag.attribute("id");

                        if (ui->comboBox_2->currentText() == attrecovery)
                        {
                            ui->radioButton_2->show();
                            recovery = recoverytag.text();
                            ui->radioButton_2->setText(recovery);

                            attbetatwo = recoverytag.attribute("beta");
                        }
                    }
                    else if (recoverytag.tagName() == "recovery3")
                    {
                        attrecovery = recoverytag.attribute("id");

                        if (ui->comboBox_2->currentText() == attrecovery)
                        {
                            ui->radioButton_3->show();
                            recovery = recoverytag.text();
                            ui->radioButton_3->setText(recovery);

                            attbetathree = recoverytag.attribute("beta");
                        }
                    }
                    recoverytag = recoverytag.nextSiblingElement();
                }
            }
            root = root.nextSiblingElement();
        }
        xmlfile.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 1", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlfile.close();
        exit(0);
    }
}

void MainWindow::parsexmlinfo()
{
    QFile xmlfile("bbin/manifest.xml");

    if(xmlfile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        doc.setContent(&xmlfile, false);

        QDomElement root = doc.documentElement();

        root = root.firstChildElement();

        while (!root.isNull())
        {
            if (root.tagName() == "info:list")
            {
                QDomElement infotag = root.firstChildElement();

                while (!infotag.isNull())
                {
                    if (ui->radioButton->isChecked())
                    {
                        if (infotag.tagName() == "host1")
                        {
                            atthost = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atthost)
                            {
                                host = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "link1")
                        {
                            attlink = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == attlink)
                            {
                                link = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "type1")
                        {
                            atttype = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atttype)
                            {
                                type = infotag.text();
                            }
                        }
                    }
                    else if (ui->radioButton_2->isChecked())
                    {
                        if(infotag.tagName() == "host2")
                        {
                            atthost = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atthost)
                            {
                                ui->radioButton_2->show();
                                host = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "link2")
                        {
                            attlink = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == attlink)
                            {
                                ui->radioButton_2->show();
                                link = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "type2")
                        {
                            atttype = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atttype)
                            {
                                type = infotag.text();
                            }
                        }
                    }
                    else if (ui->radioButton_3->isChecked())
                    {
                        if(infotag.tagName() == "host3")
                        {
                            atthost = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atthost)
                            {
                                ui->radioButton_3->show();
                                host = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "link3")
                        {
                            attlink = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == attlink)
                            {
                                ui->radioButton_3->show();
                                link = infotag.text();
                            }
                        }
                        else if (infotag.tagName() == "type3")
                        {
                            atttype = infotag.attribute("id");

                            if (ui->comboBox_2->currentText() == atttype)
                            {
                                type = infotag.text();
                            }
                        }
                    }
                    infotag = infotag.nextSiblingElement();
                }
            }
            root = root.nextSiblingElement();
        }
        xmlfile.close();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 1", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        xmlfile.close();
        exit(0);
    }
}

void MainWindow::parsexmlbeta()
{
    if (ui->radioButton->isChecked())
    {
        if (attbetaone == "true")
        {
            ui->label_2->show();
        }
        else
        {
            ui->label_2->hide();
        }
    }
    else if (ui->radioButton_2->isChecked())
    {
        if (attbetatwo == "true")
        {
            ui->label_2->show();
        }
        else
        {
            ui->label_2->hide();
        }
    }
    else if (ui->radioButton_3->isChecked())
    {
        if (attbetathree == "true")
        {
            ui->label_2->show();
        }
        else
        {
            ui->label_2->hide();
        }
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 5", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
}

void MainWindow::userdialog()
{
    ui->label->setText("Please, make your choice, ");
    ui->label_2->setText("This recovery is currently in beta and may not work correctly");
    ui->label_3->hide();

    ui->progressBar->hide();
}

void MainWindow::downloaddialog()
{
    ui->groupBox->hide();
    ui->groupBox_2->hide();
    ui->groupBox_3->hide();

    ui->label->setText("Please, wait a while, ");
    ui->label_2->setText("Downloading your recovery");
    ui->label_2->show();

    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);

    ui->progressBar->show();
}

void MainWindow::warningdialog()
{  
    ui->label->setText("Please, read carefully, ");

    if (ui->checkBox->isChecked())
    {
        ui->label_3->setText("You have enabled the 'Developper Mode', RecoverX has only downloaded the recovery file (*.img, *.elf, *.zip).\n\nYou can find the recovery installation file in the '/bbin/recovery' folder.\nYou will need to flash or intall it manually on your device.\n\nFor more informations, please visit http://www.ledelete.shost.ca/recoverx/dev.html");

        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setText("Exit");
        stepnumber = 3;
    }
    else
    {
        if (type == "img")
        {
            if (unlocked == 1)
            {
                ui->label_3->setText("You are about to install a recovery on your " + ui->comboBox_2->currentText() + ".\nPlease note that fastboot drivers must be correcty installed on your computer.\n\nYou need to have an unlocked bootloader before continue.\nIf this is the case, please, reboot your device in fastboot mode and plug your device to the computer.\n\nI'm not responsible in case of brick, dammages or lose of data on your device.");
            }
            else
            {
                ui->label_3->setText("You are about to install a recovery on your " + ui->comboBox_2->currentText() + ".\nPlease note that adb drivers must be correcty installed on your computer.\n\nYou also need to have permanent root access, busybox installed and usb debugging activated on your device.\nIf this is the case, please plug your device to the computer and keep your screen on to allow superuser permissions.\n\nI'm not responsible in case of brick, dammages or lose of data on your device.");
            }
        }
        else if (type == "elf")
        {
            ui->label_3->setText("You are about to install a recovery on your " + ui->comboBox_2->currentText() + ".\nPlease note that fastboot drivers must be correcty installed on your computer.\n\nYou need to have an unlocked bootloader before continue.\nIf this is the case, please, reboot your device in fastboot mode and plug your device to the computer.\nNotice that this installation method will change your kernel for an other one.\n\nI'm not responsible in case of brick, dammages or lose of data on your device.");
        }
        else if (type == "zip")
        {
            ui->label_3->setText("You are about to install a recovery on your " + ui->comboBox_2->currentText() + ".\nPlease note that adb drivers must be correcty installed on your computer.\n\nYou also need to have full root access, busybox installed and usb debugging activated on your device.\nIf this is the case, please plug your device to the computer and keep your screen on to allow superuser permissions.\n\nPlease, note that the installation may fail on some devices even if they are rooted\nI'm not responsible in case of brick, dammages or lose of data on your device.");
        }
        else
        {
            int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 5", QMessageBox::Ok | QMessageBox::Help);

            if (error == QMessageBox::Help)
            {
                QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
            }
            exit(0);
        }

        stepnumber = 2;
    }

    ui->label_2->hide();
    ui->label_3->show();

    ui->progressBar->hide();
}

void MainWindow::installdialog()
{
    ui->label->setText("Installing your recovery");
    ui->label_2->setText("Waiting to install your recovery");

    ui->label_2->show();
    ui->label_3->hide();

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    ui->progressBar->show();

    if (type == "img")
    {
        if (unlocked == 1)
        {
            installimg();
        }
        else
        {
            installflash();
        }
    }
    else if (type == "elf")
    {
        installelf();
    }
    else if (type == "zip")
    {
        installzip();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 5", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    parsexmldevice();
    parsexmlrecovery();
    parsexmlbeta();
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    parsexmlrecovery();
    parsexmlbeta();
}

void MainWindow::on_pushButton_clicked()
{
    if (stepnumber == 0)
    {
        parsexmlinfo();
        downloaddialog();
        startdownload();
    }
    else if (stepnumber == 1)
    {
        bootloader();
        warningdialog();
    }
    else if (stepnumber == 2)
    {
        installdialog();
    }
    else if (stepnumber ==  3)
    {
        exit(0);
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 5", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int askcancel = QMessageBox::information(this, "RecoverX", "Are you sure you want to close RecoverX ?", QMessageBox::Yes | QMessageBox::No);

    if (askcancel == QMessageBox::Yes)
    {
        exit(0);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    Dialog dia;
    dia.exec();
}

void MainWindow::startdownload()
{
    delrecovery();

    // Download the file

     http = new QHttp(this);

     connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(requestFinished(int,bool)));

     http->setHost(host);
     http->get(link);
}

void MainWindow::requestFinished(int id, bool error)
{
    if(error)
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 2", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
    else
    {
        QFile download("bbin/recovery/recoverx." + type);

        if(download.open(QFile::Append))
        {
            download.write(http->readAll());
        }

        if (download.size() != 0)
        {
            stepnumber = 1;
            ui->pushButton->setEnabled(true);
            ui->label->setText("Download complete");
            ui->label_2->setText("Download complete, you can now continue");
        }
        download.close();
    }
}


void MainWindow::installimg()
{
    QFile checkimg("bbin/recovery/recoverx.img");

    if (checkimg.size() != 0)
    {
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        ui->label_2->setText("Flashing recovery");
        limit = 40;
        value();

        QProcess flashimg;
        flashimg.start("bbin\\fastboot flash recovery bbin\\recovery\\recoverx.img");
        flashimg.waitForFinished();

        delrecovery();


        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Exit");

        stepnumber = 3;

        ui->label->setText("Thank you for using RecoverX");
        ui->label_2->setText("The installation is now complete, please, reboot your device");
        limit = 100;
        value();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 3", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
    checkimg.close();
}

void MainWindow::installelf()
{
    QFile checkelf("bbin/recovery/recoverx.elf");

    if (checkelf.size() != 0)
    {
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        ui->label_2->setText("Flashing recovery");
        limit = 40;
        value();

        QProcess flashelf;
        flashelf.start("bbin\\fastboot flash boot bbin\\recovery\\recoverx.elf");
        flashelf.waitForFinished();

        delrecovery();

        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Exit");

        stepnumber = 3;

        ui->label->setText("Thank you for using RecoverX");
        ui->label_2->setText("The installation is now complete, please, reboot your device");
        limit = 100;
        value();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 3", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
    checkelf.close();
}

void MainWindow::installflash()
{
    QFile checkimg("bbin/recovery/recoverx.img");

    if (checkimg.size() != 0)
    {
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        checkdevice();

        ui->label_2->setText("Push files");
        limit = 10;
        value();

        QProcess pushinstall;
        pushinstall.start("bbin\\adb push bbin\\flash\\install.zip /data/local/tmp");
        pushinstall.waitForFinished();

        QProcess pushflash;
        pushflash.start("bbin\\adb push bbin\\recovery\\recoverx.img /data/local/tmp");
        pushflash.waitForFinished();

        ui->label_2->setText("Unzip files");
        limit = 20;
        value();

        QProcess unzip;
        unzip.start("bbin\\adb shell \"unzip /data/local/tmp/install.zip -d /data/local/tmp\"");
        unzip.waitForFinished();

        ui->label_2->setText("Chmod files to 755");
        limit = 40;
        value();

        QProcess chmodinstall;
        chmodinstall.start("bbin\\adb shell \"chmod 755 /data/local/tmp/install.sh\"");
        chmodinstall.waitForFinished();

        QProcess chmodsh;
        chmodsh.start("bbin\\adb shell \"chmod 755 /data/local/tmp/sh\"");
        chmodsh.waitForFinished();

        ui->label_2->setText("Run installation script");
        limit = 60;
        value();

        QProcess run;
        run.start("bbin\\adb shell su -c \"/data/local/tmp/install.sh\"");
        run.waitForFinished();

        ui->label_2->setText("Clear temp folder");
        limit = 80;
        value();

        QProcess temp;
        temp.start("bbin\\adb shell \"rm -r /data/local/tmp/*\"");
        temp.waitForFinished();

        delrecovery();

        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Exit");

        stepnumber = 3;

        ui->label->setText("Thank you for using RecoverX");
        ui->label_2->setText("The installation is now complete, please, reboot your device");
        limit = 100;
        value();

        QProcess kill;
        kill.start("bbin\\adb kill-server");
        kill.waitForFinished();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 3", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
    checkimg.close();
}

void MainWindow::installzip()
{
    QFile checkzip("bbin/recovery/recoverx.zip");

    if (checkzip.size() != 0)
    {      
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        checkdevice();

        ui->label_2->setText("Push files");
        limit = 10;
        value();

        QProcess push;
        push.start("bbin\\adb push bbin\\recovery\\recoverx.zip /data/local/tmp");
        push.waitForFinished();

        ui->label_2->setText("Unzip files");
        limit = 20;
        value();


        QProcess unzip;
        unzip.start("bbin\\adb shell \"unzip /data/local/tmp/recoverx.zip -d /data/local/tmp\"");
        unzip.waitForFinished();

        ui->label_2->setText("Chmod files to 755");
        limit = 40;
        value();

        QProcess chmodinstall;
        chmodinstall.start("bbin\\adb shell \"chmod 755 /data/local/tmp/install.sh\"");
        chmodinstall.waitForFinished();

        QProcess chmodsh;
        chmodsh.start("bbin\\adb shell \"chmod 755 /data/local/tmp/sh\"");
        chmodsh.waitForFinished();

        ui->label_2->setText("Run installation script");
        limit = 60;
        value();

        QProcess run;
        run.start("bbin\\adb shell su -c \"/data/local/tmp/install.sh\"");
        run.waitForFinished();

        ui->label_2->setText("Clear temp folder");
        limit = 80;
        value();

        QProcess temp;
        temp.start("bbin\\adb shell \"rm -r /data/local/tmp/*\"");
        temp.waitForFinished();

        delrecovery();

        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Exit");

        stepnumber = 3;

        ui->label->setText("Thank you for using RecoverX");
        ui->label_2->setText("The installation is now complete, please, reboot your device");
        limit = 100;
        value();


        QProcess kill;
        kill.start("bbin\\adb kill-server");
        kill.waitForFinished();
    }
    else
    {
        int error = QMessageBox::critical(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 3", QMessageBox::Ok | QMessageBox::Help);

        if (error == QMessageBox::Help)
        {
            QDesktopServices::openUrl(QUrl("http://ledelete.shost.ca/recoverx/help.html"));
        }
        exit(0);
    }
    checkzip.close();
}

void MainWindow::installbusybox()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

    ui->label_2->setText("Push busybox");
    limit = 40;
    value();

    QProcess pushbusybox;
    pushbusybox.start("bbin\\adb push bbin\\busybox\\busybox /data/local/tmp");
    pushbusybox.waitForFinished();

    QProcess pushbusyboxsh;
    pushbusyboxsh.start("bbin\\adb push bbin\\busybox\\busybox.sh /data/local/tmp");
    pushbusyboxsh.waitForFinished();

    QProcess pushsh;
    pushsh.start("bbin\\adb push bbin\\busybox\\sh /data/local/tmp");
    pushsh.waitForFinished();

    ui->label_2->setText("Chmod busybox to 755");
    limit = 60;
    value();

    QProcess chmodbusybox;
    chmodbusybox.start("bbin\\adb shell \"chmod 755 /data/local/tmp/busybox.sh\"");
    chmodbusybox.waitForFinished();

    QProcess chmodbb;
    chmodbb.start("bbin\\adb shell \"chmod 755 /data/local/tmp/sh\"");
    chmodbb.waitForFinished();

    ui->label_2->setText("Installin busybox to /system/xbin");
    limit = 100;
    value();

    QProcess runbb;
    runbb.start("bbin\\adb shell su -c \"/data/local/tmp/busybox.sh\"");
    runbb.waitForFinished();

    QProcess kill;
    kill.start("bbin\\adb kill-server");
    kill.waitForFinished();

    installzip();
}

void MainWindow::checkdevice()
{
    ui->label_2->setText("Starting adb");

    QProcess startadb;
    startadb.start("bbin\\adb start-server");
    startadb.waitForFinished();

    ui->label_2->setText("Wait for the device");

    waitdevice();

    ui->label_2->setText("Check if the device is correctly rooted");

    QProcess searchsubin;
    searchsubin.start("bbin\\adb pull /system/bin/su bbin\\bin\\su");
    searchsubin.waitForFinished();

    QProcess searchsuxbin;
    searchsuxbin.start("bbin\\adb pull /system/xbin/su bbin\\bin\\su");
    searchsuxbin.waitForFinished();

    QFile su("bbin/bin/su");

    if(su.open(QIODevice::ReadOnly))
    {
        su.remove();
    }
    else
    {
        QMessageBox::information(this, "RecoverX", "A protocol error has occurred, RecoverX is unable to work - Error 4\nhttp://http://ledelete.shost.ca/recoverx/help.html");
        exit(0);
    }
    su.close();

    ui->label_2->setText("Check if busybox is installed");

    QProcess searchbusyboxbin;
    searchbusyboxbin.start("bbin\\adb pull /system/bin/busybox bbin\\bin\\busybox");
    searchbusyboxbin.waitForFinished();

    QProcess searchbusyboxxbin;
    searchbusyboxxbin.start("bbin\\adb pull /system/xbin/busybox bbin\\bin\\busybox");
    searchbusyboxxbin.waitForFinished();

    QFile busybox("bbin/bin/busybox");

    if(busybox.open(QIODevice::ReadOnly))
    {
        busybox.remove();
    }
    else
    {
        int askbusybox = QMessageBox::information(this, "RecoverX", "Busybox is not installed on your device, do you want to install it ?", QMessageBox::Yes | QMessageBox::No);

        if (askbusybox == QMessageBox::Yes)
        {
            installbusybox();
        }
        else
        {
            exit(0);
        }
    }
    busybox.close();
}

void MainWindow::waitdevice()
{
    QProcess wait;
    wait.start("bbin\\adb pull /system/build.prop bbin\\bin\\build.prop");
    wait.waitForFinished();

    QFile build("bbin/bin/build.prop");

    if(build.open(QIODevice::ReadOnly))
    {
        build.remove();
    }
    else
    {
        int askdevice = QMessageBox::information(this, "RecoverX", "The device is not connected or your drivers are not correctly installed.", QMessageBox::Ok | QMessageBox::Cancel);

        if (askdevice == QMessageBox::Ok)
        {
            waitdevice();
        }
        else if (askdevice == QMessageBox::Cancel)
        {
            exit(0);
        }
    }
    build.close();
}

void MainWindow::delrecovery()
{
    QFile remove("bbin/recovery/recoverx." + type);

    if(remove.open(QIODevice::ReadOnly))
    {
        remove.remove();
    }
    remove.close();
}

void MainWindow::value()
{
    while (progress <= limit)
    {
        Sleep(20);
        ui->progressBar->setValue(progress);
        progress++;
    }
}

void MainWindow::on_radioButton_clicked()
{
    parsexmlbeta();
}

void MainWindow::on_radioButton_2_clicked()
{
    parsexmlbeta();
}

void MainWindow::on_radioButton_3_clicked()
{
    parsexmlbeta();
}
