#include "videoaudiodialog.h"
#include "ui_videoaudiodialog.h"
#include<QDebug>

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

void VideoAudioDialog::setinfo(bool audio)
{
    if(audio){
        ui->lb_image->setPixmap(QPixmap(":/tupian/audio.png"));
        ui->lb_minImg->setStyleSheet("background:transparent;");
    }else{
        ui->lb_image->setPixmap(QPixmap(":/tupian/video.png"));
        ui->lb_minImg->setPixmap(QPixmap(":/tupian/video.png"));
    }
}

void VideoAudioDialog::setimage(QImage &img)
{
    //TODO
}

void VideoAudioDialog::on_cb_audio_clicked()
{
    if(!ui->cb_audio->isChecked()){//勾了
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

