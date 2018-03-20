#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->IATButton,SIGNAL(clicked()),this,SLOT(iat_record_sample()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::iat_record_sample()
{
    int ret = MSP_SUCCESS;
    int upload_on =	1;     /* whether upload the user word */
                          /* login params, please do keep the appid correct */
    const char* login_params = "appid = 593fcb2f, work_dir = .";
    int aud_src = 0;      /* from mic or file */

        /*
        * See "iFlytek MSC Reference Manual"
        */
    const char* session_begin_params =
            "sub = iat, domain = iat, language = zh_cn, "
            "accent = mandarin, sample_rate = 16000, "
            "result_type = plain, result_encoding = utf8";

        /* Login first. the 1st arg is username, the 2nd arg is password
         * just set them as NULL. the 3rd arg is login paramertes
         * */
    ret = MSPLogin(NULL, NULL, login_params);
    if (MSP_SUCCESS != ret)	{
        printf("MSPLogin failed , Error code %d.\n",ret);
        goto exit; // login fail, exit the program
    }

    printf("Want to upload the user words ? \n0: No.\n1: Yes\n");
    scanf("%d", &upload_on);
    if (upload_on)
    {
        printf("Uploading the user words ...\n");
        ret = upload_userwords();
        if (MSP_SUCCESS != ret)
            goto exit;
        printf("Uploaded successfully\n");
    }

    printf("Where the audio comes from?\n"
                "0: From a audio file.\n1: From microphone.\n");
    scanf("%d", &aud_src);
    if(aud_src != 0) {
       printf("Demo recognizing the speech from microphone\n");
       printf("Speak in 15 seconds\n");

       demo_mic(session_begin_params);

        printf("15 sec passed\n");
    } else {
        //printf("Demo recgonizing the speech from a recorded audio file\n");
        //demo_file("wav/iflytek02.wav", session_begin_params);
        }
exit:
   MSPLogout(); // Logout...

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




