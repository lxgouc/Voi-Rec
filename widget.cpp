#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSound>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->IATButton,SIGNAL(clicked()),this,SLOT(iatexec()));
    connect(ui->TTSButton,SIGNAL(clicked()),this,SLOT(ttsexec()));
    //connect(ui->***Button,SIGNAL(clicked()),this,SLOT(***exec()));
}

Widget::~Widget()
{
    delete ui;
    samplethread.quit();
    samplethread.wait();
}

void Widget::iatthreadinit()
{
     Iatwork *iatsample=new Iatwork;
     iatsample->moveToThread(&samplethread);
     connect(&samplethread,SIGNAL(started()),iatsample,SLOT(iat_record_sample()));
     connect(&samplethread,SIGNAL(finished()),iatsample,SLOT(deleteLater()));
     connect(iatsample,SIGNAL(iattaskdone()),this,SLOT(showdata()));
}

void Widget::ttsthreadinit()
{
    Ttswork *ttssample=new Ttswork;
    ttssample->moveToThread(&samplethread);
    connect(&samplethread,SIGNAL(started()),this,SLOT(gettext()));
    connect(&samplethread,SIGNAL(finished()),ttssample,SLOT(deleteLater()));
    connect(this,SIGNAL(plaintext(const QString)),ttssample,SLOT(tts_sample(const QString)));
    connect(ttssample,SIGNAL(ttstaskdone()),this,SLOT(playaudio()));
}

void Widget::iatexec()
{
    if(ui->IATButton->text()=="IAT ON")
    {
       iatthreadinit();
       samplethread.start();
       ui->IATButton->setText("IAT OFF");
    }
    else
    {
        samplethread.quit();
        samplethread.wait();
        ui->IATButton->setText("IAT ON");
        ui->StateDisplay->clear();
    }

}

void Widget::ttsexec()
{if(ui->TTSButton->text()=="TTS ON")
    {
       ttsthreadinit();
       samplethread.start();
       ui->TTSButton->setText("TTS OFF");
    }
    else
    {
        samplethread.quit();
        samplethread.wait();
        ui->TTSButton->setText("TTS ON");
        ui->StateDisplay->clear();
    }

}

void Widget::gettext()
{
    const QString text=ui->StateDisplay->toPlainText();
    emit plaintext(text);
}

void Widget::showdata()
{
    ui->ResultDisplay->setText(QString(redata));
    ui->IATButton->setText("IAT ON");
    samplethread.quit();
    samplethread.wait();
}

void Widget::playaudio()
{
    samplethread.quit();
    samplethread.wait();
    QSound::play("/home/lxg/ouc/Voi-Rec/audio.wav");
    ui->TTSButton->setText("TTS ON");
}

int Widget::upload_userwords()
{
    char*			userwords	=	NULL;
    size_t			len			=	0;
    size_t			read_len	=	0;
    FILE*			fp			=	NULL;
    int				ret			=	-1;

    fp = fopen("userwords.txt", "rb");
    if (NULL == fp)
    {
        printf("\nopen [userwords.txt] failed! \n");
        goto upload_exit;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    userwords = (char*)malloc(len + 1);
    if (NULL == userwords)
    {
        printf("\nout of memory! \n");
        goto upload_exit;
    }

    read_len = fread((void*)userwords, 1, len, fp);
    if (read_len != len)
    {
        printf("\nread [userwords.txt] failed!\n");
        goto upload_exit;
    }
    userwords[len] = '\0';

    MSPUploadData("userwords", userwords, len, "sub = uup, dtt = userword", &ret); //ÉÏ´«ÓÃ»§´Ê±í
    if (MSP_SUCCESS != ret)
    {
        printf("\nMSPUploadData failed ! errorCode: %d \n", ret);
        goto upload_exit;
    }

upload_exit:
    if (NULL != fp)
    {
        fclose(fp);
        fp = NULL;
    }
    if (NULL != userwords)
    {
        free(userwords);
        userwords = NULL;
    }

    return ret;
}




