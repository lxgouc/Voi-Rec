#include "ttswork.h"
#include <QDebug>

Ttswork::Ttswork(QObject *parent) : QObject(parent)
{

}


void Ttswork::tts_sample(const QString src_text)
{
    int         ret                  = MSP_SUCCESS;
        const char* login_params         = "appid = 593fcb2f, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
        /*
        * rdn:           合成音频数字发音方式
        * volume:        合成音频的音量
        * pitch:         合成音频的音调
        * speed:         合成音频对应的语速
        * voice_name:    合成发音人
        * sample_rate:   合成音频采样率
        * text_encoding: 合成文本编码格式
        *
        */
        const char* session_begin_params = "engine_type = local,voice_name=xiaoyan, text_encoding = UTF8, tts_res_path = fo|/home/lxg/ouc/Voi-Rec/bin/msc/res/tts/xiaoyan.jet;fo|/home/lxg/ouc/Voi-Rec/bin/msc/res/tts/common.jet, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
        const char* filename             = "/home/lxg/ouc/Voi-Rec/audio.wav"; //合成的语音文件名称
        /* 用户登录 */
        qDebug()<<src_text;
        string str=src_text.toStdString();
        const char *text=str.c_str();
        qDebug()<<text;
        ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
        if (MSP_SUCCESS != ret)
        {
            printf("MSPLogin failed, error code: %d.\n", ret);
            goto exit ;//登录失败，退出登录
        }

        //printf("\n###########################################################################\n");
        //printf("## 成语音合（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的 ##\n");
        //printf("## 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的  ##\n");
        //printf("## 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。  ##\n");
        //printf("###########################################################################\n\n");

        /* 文本合成 */
        emit statedata(tr("开始合成 ..."));
        ret = text_to_speech(text, filename, session_begin_params);
        if (MSP_SUCCESS != ret)
        {
            printf("text_to_speech failed, error code: %d.\n", ret);
        }
        emit statedata(tr("合成完毕!"));

    exit:
        MSPLogout();
        emit ttstaskdone();

}

int Ttswork::text_to_speech(const char* src_text, const char* des_path, const char* params)
{
    int          ret          = -1;
    FILE*        fp           = NULL;
    const char*  sessionID    = NULL;
    unsigned int audio_len    = 0;
    wave_pcm_hdr wav_hdr      = default_wav_hdr;
    int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
    if (NULL == src_text || NULL == des_path)
    {
        printf("params is error!\n");
        return ret;
    }
    qDebug()<<src_text<<"two";
    fp = fopen(des_path, "wb");
    if (NULL == fp)
    {
        printf("open %s error.\n", des_path);
        return ret;
    }
    /* 开始合成 */
    sessionID = QTTSSessionBegin(params, &ret);
    if (MSP_SUCCESS != ret)
    {
        printf("QTTSSessionBegin failed, error code: %d.\n", ret);
        fclose(fp);
        return ret;
    }
    qDebug()<<src_text<<"three";
    ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
    if (MSP_SUCCESS != ret)
    {
        printf("QTTSTextPut failed, error code: %d.\n",ret);
        QTTSSessionEnd(sessionID, "TextPutError");
        fclose(fp);
        return ret;
    }
    emit statedata(tr("正在合成 ..."));
    fwrite(&wav_hdr, sizeof(wav_hdr) ,1, fp); //添加wav频头，使用采样率为16000
    while (1)
    {
        /* 获取合成音频 */
        const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
        if (MSP_SUCCESS != ret)
            break;
        if (NULL != data)
        {
            fwrite(data, audio_len, 1, fp);
            wav_hdr.data_size += audio_len; //计算data_size大小
        }
        if (MSP_TTS_FLAG_DATA_END == synth_status)
            break;
    }
    if (MSP_SUCCESS != ret)
    {
        printf("QTTSAudioGet failed, error code: %d.\n",ret);
        QTTSSessionEnd(sessionID, "AudioGetError");
        printf("failture");
        fclose(fp);
        return ret;
    }
    /* 修正wav文件头数据的大小 */
    wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

    /* 将修正过的数据写回文件头部,音频文件为wav格式 */
    fseek(fp, 4, 0);
    fwrite(&wav_hdr.size_8,sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
    fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
    fwrite(&wav_hdr.data_size,sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
    fclose(fp);
    fp = NULL;
    /* 合成完毕 */
    ret = QTTSSessionEnd(sessionID, "Normal");
    if (MSP_SUCCESS != ret)
    {
        printf("QTTSSessionEnd failed, error code: %d.\n",ret);
    }

    return ret;
}
