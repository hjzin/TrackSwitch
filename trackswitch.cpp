/*界面部分*/
#include "trackswitch.h"
#include "ui_trackswitch.h"
#include "QMainWindow"
#include "QMessageBox"
#include "QDir"
#include "QFileDialog"
#include "switch_detect.h"

using namespace cv;
using namespace std;

QString path;
TrackSwitch::TrackSwitch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrackSwitch)
{
    ui->setupUi(this);
}

TrackSwitch::~TrackSwitch()
{
    delete ui;
}

void TrackSwitch::on_toolButton_clicked()
{
    path = QFileDialog::getOpenFileName(this, tr("选择图片"), "/Users/hhzjj/Code/C:C++/Clion/TrackSwitch/image", tr("JPEG Files(*.jpg)"));
    QString StrWidth,StrHeigth;
    QImage* img = new QImage;
    QImage* scaledimg = new QImage;
    img->load(path);
    if (!path.isEmpty())
    {
        ui->comboBox->addItem(path);
        int Owidth=img->width();
        int Oheight=img->height();
        int Fwidth,Fheight;       //缩放后的图片大小
        int Mul;            //记录图片与label大小的比例，用于缩放图片
        if(Owidth/241>=Oheight/300)
            Mul=Owidth/241;
        else
            Mul=Oheight/300;
        Fwidth=Owidth/Mul;
        Fheight=Oheight/Mul;
        *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);
        ui->label_3->setPixmap(QPixmap::fromImage(*scaledimg));
    }
    else {
        QMessageBox::warning(this, tr("选择图片"), tr("您没有选择图片！"));
    }
}

void TrackSwitch::on_pushButton_clicked()
{
    string spath;
    spath = string((const char *)path.toLocal8Bit());
    LSD(spath);
    int is_ballas = 0, is_switch = 0;
    is_ballas = have_ballas();
    is_switch = select();
    if(is_switch == 1 && is_ballas == 1)
        ui->label_4->setText(tr("该场景为有砟道岔场景！"));
    else if (is_switch == 1 && is_ballas == 0) {
        ui->label_4->setText(tr("该场景为无砟道岔场景！"));
    }
    else if (is_switch == 0 && is_ballas == 1) {
        ui->label_4->setText(tr("该场景为有砟单轨场景！"));
    }
    else if(is_switch == 0 && is_ballas == 0)
    {
        ui->label_4->setText(tr("该场景为无砟单轨场景！"));
    }
    clear_arrays();
}

