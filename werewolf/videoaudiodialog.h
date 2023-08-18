#ifndef VIDEOAUDIODIALOG_H
#define VIDEOAUDIODIALOG_H

#include <QDialog>

namespace Ui {
class VideoAudioDialog;
}

class VideoAudioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoAudioDialog(QWidget *parent = nullptr);
    ~VideoAudioDialog();
    void setinfo(bool audio);
    void setimage(QImage& img);


signals:
    void SIG_audioOpen();
    void SIG_videoOpen();
    void SIG_audioClose();
    void SIG_videoClose();

private slots:
    void on_cb_audio_clicked();

    void on_cb_video_clicked();

private:
    Ui::VideoAudioDialog *ui;
};

#endif // VIDEOAUDIODIALOG_H
