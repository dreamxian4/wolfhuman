#ifndef VIDEOAUDIODIALOG_H
#define VIDEOAUDIODIALOG_H

#include <QDialog>

namespace Ui {
class VideoAudioDialog;
}

class VideoAudioDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_quitAu(int);

public:
    explicit VideoAudioDialog(QWidget *parent = nullptr);
    ~VideoAudioDialog();
    void setinfo(bool audio,int id,QString name);
    void slot_setBigImage(QImage &img);
    void slot_setSmallImage(QImage &img);
    void slot_stoptime();



signals:
    void SIG_audioOpen();
    void SIG_videoOpen();
    void SIG_audioClose();
    void SIG_videoClose();

private slots:
    void on_cb_audio_clicked();

    void on_cb_video_clicked();

    void on_pb_poweroff_clicked();

private:
    Ui::VideoAudioDialog *ui;
    int m_id;
};

#endif // VIDEOAUDIODIALOG_H
