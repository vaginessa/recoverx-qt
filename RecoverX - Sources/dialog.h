#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_label_4_linkActivated(const QString &link);
    void on_label_5_linkActivated(const QString &link);
    void on_label_6_linkActivated(const QString &link);
    void on_label_7_linkActivated(const QString &link);
    void on_label_9_linkActivated(const QString &link);
    void on_label_10_linkActivated(const QString &link);

    void on_pushButton_4_clicked();

private:

    Ui::Dialog *ui;
};

#endif // DIALOG_H
