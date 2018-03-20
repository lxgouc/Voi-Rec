#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "speech_recognizer.h"

#define FRAME_LEN	640

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

private slots:
    void iat_record_sample();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
