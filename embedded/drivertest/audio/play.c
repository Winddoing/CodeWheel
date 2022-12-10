//未测试
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <alsa/asoundlib.h>

snd_pcm_hw_params_t *params;  /* pcm参数 */
unsigned int channels;
#define goto_error_if_fail(p)                           \
  if (!(p)) {                                           \
    printf("%s:%d " #p "\n", __FUNCTION__, __LINE__);   \
    goto error;                                         \
  }
snd_pcm_t* device_create(int sample,int channel)
{
   
    snd_pcm_t* handle;        /* pcm句柄 */
    int rc = -1;
    channels = channel;

    /* 打开设备 */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK,0);
    if (rc < 0)
    {
        goto error;
    }
    /* 初始化pcm属性 */
    rc = snd_pcm_hw_params_malloc(&params);
    if (rc < 0)
    { 
       goto error;
    }
    snd_pcm_hw_params_any(handle, params);

    /* 设置为交错模式 */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* 设置为双声道 */
    snd_pcm_hw_params_set_channels(handle, params, 2);

     /* 设置数据格式: 有符号16位格式 */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16);

     /* 设置采样率 */
    snd_pcm_hw_params_set_rate_near(handle,params,&sample,0);

    /* 将参数写入设备 */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0)
    { 
        goto error;
    }

     return handle;
error:
    snd_pcm_hw_params_free(params);
    snd_pcm_close(handle);
    return NULL;
}

int device_play(snd_pcm_t* pcm_handle, const void* data, int data_size)
{
   
  snd_pcm_uframes_t frame_num;
  snd_pcm_uframes_t frame_size;    /* 一帧的大小 */
    /* 双声道 */
  unsigned int format_sbits = 16; /* 数据格式: 有符号16位格式 */
  /* 获取一个播放周期的帧数 */
  snd_pcm_hw_params_get_period_size(params, &frame_num, 0);
  frame_size = frame_num * 2; //2 bytes/sample, 2 channels
  for(snd_pcm_uframes_t i = 0; i < data_size;)
  {
   
    int rc = -1;
    /* 播放 */
    rc = snd_pcm_writei(pcm_handle, data + i, frame_num);
    goto_error_if_fail(rc >= 0 || rc == -EPIPE);
    if(rc == -EPIPE)
    {
      /* 让音频设备准备好接收pcm数据 */
      snd_pcm_prepare(pcm_handle); 
    }
    else
    {
   
      i += frame_num * frame_size;
    }
  }
  return 0;
error:
  return -1;
}


void device_destroy(snd_pcm_t* pcm_handle)
{
   
  snd_pcm_drain(pcm_handle);   /* 等待数据全部播放完成 */
  snd_pcm_hw_params_malloc(params);
  snd_pcm_close(pcm_handle);
}

