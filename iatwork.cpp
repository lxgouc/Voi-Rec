#include "iatwork.h"
#include "speech_recognizer.h"
#include <QDebug>

Iatwork::Iatwork(QObject *parent) : QObject(parent)
{

}

Iatwork::~Iatwork()
{

}

void Iatwork::iat_record_sample()
{
    qDebug()<<"lxg";
    int ret = MSP_SUCCESS;
        int upload_on =	1; /* whether upload the user word */
        /* login params, please do keep the appid correct */
        const char* login_params = "appid = 593fcb2f, work_dir = .";
        int aud_src = 0; /* from mic or file */

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

        /*
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
        */
            emit statedata("Demo recognizing the speech from microphone\n");
            emit statedata("Speak in 15 seconds\n");
            qDebug()<<"lll";
            demo_mic(session_begin_params);
            qDebug()<<"222";

            emit statedata("15 sec passed\n");
    exit:
        MSPLogout(); // Logout...
}
