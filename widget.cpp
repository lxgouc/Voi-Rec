#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    iatsample=new Iatwork;
    iatsample->moveToThread(&IATthread);
    connect(ui->IATButton,SIGNAL(clicked()),this,SLOT(iatexec()));
    //connect(ui->***Button,SIGNAL(clicked()),this,SLOT(***exec()));
    //connect(ui->***Button,SIGNAL(clicked()),this,SLOT(***exec()));
    connect(&IATthread,SIGNAL(finished()),iatsample,SLOT(deleteLater()));
}

Widget::~Widget()
{
    delete ui;
    IATthread.quit();
    IATthread.wait();
}

void Widget::iatexec()
{
    connect(&IATthread,SIGNAL(started()),iatsample,SLOT(iat_record_sample()));
    IATthread.start();
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




