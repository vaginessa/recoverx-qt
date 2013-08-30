#include "license.h"
#include "ui_license.h"

#include <QFile>

License::License(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::License)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

License::~License()
{
    delete ui;
}

void License::on_pushButton_clicked()
{
    close();
}

void License::on_pushButton_2_clicked()
{
    QFile stats("bbin/stats.xml");

    if(stats.open(QIODevice::ReadOnly))
    {
        stats.remove();
        stats.close();
    }
    else
    {
        stats.close();
    }

    exit(0);
}
