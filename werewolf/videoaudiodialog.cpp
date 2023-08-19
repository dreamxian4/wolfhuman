#include "videoaudiodialog.h"
#include "ui_videoaudiodialog.h"
#include<QDebug>
#include<QPainter>

VideoAudioDialog::VideoAudioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoAudioDialog)
{
    ui->setupUi(this);
}

VideoAudioDialog::~VideoAudioDialog()
{
    delete ui;
}

void VideoAudioDialog::setinfo(bool audio,int id,QString name)
{
    m_id=id;
    if(audio){
        ui->wdg_bigShow->slot_setInfo(name,true);
        ui->wdg_small_show->hide();
    }else{
        ui->wdg_bigShow->slot_setInfo(name,false);
        ui->wdg_small_show->show();
        ui->wdg_small_show->slot_setInfo("我",false);
    }
}

void VideoAudioDialog::slot_setBigImage(QImage &img)
{
    ui->wdg_bigShow->slot_setImage(img);
}

void VideoAudioDialog::slot_setSmallImage(QImage &img)
{
    ui->wdg_small_show->slot_setImage(img);
}

void VideoAudioDialog::slot_stoptime()
{
    ui->wdg_bigShow->slot_stop();
    ui->wdg_small_show->slot_stop();
}

void VideoAudioDialog::on_cb_audio_clicked()
{
    if(!ui->cb_audio->isChecked()){//勾了,点了之后，变成不勾
        ui->cb_audio->setChecked(false);
        Q_EMIT SIG_audioClose();
    }else{
        ui->cb_audio->setChecked(true);
        Q_EMIT SIG_audioOpen();
    }
}

void VideoAudioDialog::on_cb_video_clicked()
{
    if(!ui->cb_video->isChecked()){//勾了,点了之后，变成不勾
        ui->cb_video->setChecked(false);
        Q_EMIT SIG_videoClose();
    }else{
        ui->cb_video->setChecked(true);
        Q_EMIT SIG_videoOpen();
    }
}


void VideoAudioDialog::on_pb_poweroff_clicked()
{
    Q_EMIT SIG_quitAu(m_id);
}

