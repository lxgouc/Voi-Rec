#ifndef TTSWORK_H
#define TTSWORK_H

#include <QObject>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

typedef int SR_DWORD;
typedef short int SR_WORD ;

typedef struct _wave_pcm_hdr
{
    char            riff[4];                // = "RIFF"
    int				size_8;                 // = FileSize - 8
    char            wave[4];                // = "WAVE"
    char            fmt[4];                 // = "fmt "
    int				fmt_size;				// = 下一个结构体的大小 : 16

    short int       format_tag;             // = PCM : 1
    short int       channels;               // = 通道数 : 1
    int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
    int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
    short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
    short int       bits_per_sample;        // = 量化比特数: 8 | 16

    char            data[4];                // = "data";
    int				data_size;              // = 纯数据长度 : FileSize - 44
} wave_pcm_hdr;


class Ttswork : public QObject
{
    Q_OBJECT
public:
    explicit Ttswork(QObject *parent = 0);

    int text_to_speech(const char* src_text, const char* des_path, const char* params);

signals:
    void statedata(const QString);

    void ttstaskdone();

public slots:
    void tts_sample(const QString plaintext);

private:
    wave_pcm_hdr default_wav_hdr =
    {
        { 'R', 'I', 'F', 'F' },
        0,
        {'W', 'A', 'V', 'E'},
        {'f', 'm', 't', ' '},
        16,
        1,
        1,
        16000,
        32000,
        2,
        16,
        {'d', 'a', 't', 'a'},
        0
    };

};

#endif // TTSWORK_H
