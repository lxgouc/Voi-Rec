#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <QThread>
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "iatwork.h"
#include "ttswork.h"
#include "speech_recognizer.h"

#define FRAME_LEN	640;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    static int upload_userwords();

    static void show_result(char *string, char is_over);

    void iatthreadinit();

    void ttsthreadinit();

public slots:
    void iatexec();

    void showdata();

    void ttsexec();

    void gettext();

    void playaudio();

    void getstatedata(const QString);

signals:
    void plaintext(const QString);

public:

private:
    Ui::Widget *ui;
    QThread samplethread;

};

#endif // WIDGET_H
