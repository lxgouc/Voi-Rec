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

public slots:
    void iatexec();

private:
    Ui::Widget *ui;
    QThread IATthread;
    Iatwork *iatsample;

};

#endif // WIDGET_H
