#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "amltools.h"
#include "libavutil/log.h"

int amlsysfs_write_string(AVCodecContext *avctx, const char *path, const char *value)
{
    int ret = 0;
    int fd = open(path, O_RDWR, 0644);
    if (fd >= 0)
    {
      ret = write(fd, value, strlen(value));
    	if (ret < 0)
        av_log(avctx, AV_LOG_ERROR, "failed to set %s to %s\n", path, value);
      close(fd);
      return 0;
    }
  return -1;
}

int amlsysfs_write_int(AVCodecContext *avctx, const char *path, int value)
{
    int ret = 0;
    char cmd[64];
    int fd = open(path, O_RDWR, 0644);
    if (fd >= 0)
    {
    	snprintf(cmd, sizeof(cmd), "%d", value);

      ret = write(fd, cmd, strlen(cmd));
    	if (ret < 0)
        av_log(avctx, AV_LOG_ERROR, "failed to set %s to %d\n", path, value);
      close(fd);
      return 0;
    }
  return -1;
}

int64_t amlsysfs_read_int(AVCodecContext *avctx, const char *path, int base)
{
    char val[16];
    int fd = open(path, O_RDWR, 0644);
    if (fd >= 0)
    {
      if (read(fd, val, sizeof(val)) < 0)
        av_log(avctx, AV_LOG_ERROR, "failed to read %s\n", path);
      close(fd);

      return strtoul(val, NULL, base);
    }
  return -1;
}

vformat_t aml_get_vformat(AVCodecContext *avctx)
{
  vformat_t format;
  switch (avctx->codec_id)
  {
    case AV_CODEC_ID_MPEG1VIDEO:
    case AV_CODEC_ID_MPEG2VIDEO:
      format = VFORMAT_MPEG12;
      break;
    case AV_CODEC_ID_H263:
    case AV_CODEC_ID_MPEG4:
    case AV_CODEC_ID_H263P:
    case AV_CODEC_ID_H263I:
    case AV_CODEC_ID_MSMPEG4V2:
    case AV_CODEC_ID_MSMPEG4V3:
    case AV_CODEC_ID_FLV1:
      format = VFORMAT_MPEG4;
      break;
    case AV_CODEC_ID_RV10:
    case AV_CODEC_ID_RV20:
    case AV_CODEC_ID_RV30:
    case AV_CODEC_ID_RV40:
      format = VFORMAT_REAL;
      break;
    case AV_CODEC_ID_H264:
      if ((avctx->width > 1920) || (avctx->height > 1088))
        format = VFORMAT_H264_4K2K;
      else
        format = VFORMAT_H264;
      break;
    case AV_CODEC_ID_MJPEG:
      format = VFORMAT_MJPEG;
      break;
    case AV_CODEC_ID_VC1:
    case AV_CODEC_ID_WMV3:
      format = VFORMAT_VC1;
      break;
    case AV_CODEC_ID_AVS:
    case AV_CODEC_ID_CAVS:
      format = VFORMAT_AVS;
      break;
    case AV_CODEC_ID_HEVC:
      format = VFORMAT_HEVC;
      break;

    default:
      format = -1;
      break;
  }

  return format;
}

vdec_type_t aml_get_vdec_type(AVCodecContext *avctx)
{
  vdec_type_t dec_type = -1;

  switch (avctx->codec_tag)
  {
    case CODEC_TAG_MJPEG:
    case CODEC_TAG_mjpeg:
    case CODEC_TAG_jpeg:
    case CODEC_TAG_mjpa:
      // mjpeg
      dec_type = VIDEO_DEC_FORMAT_MJPEG;
      break;
    case CODEC_TAG_XVID:
    case CODEC_TAG_xvid:
    case CODEC_TAG_XVIX:
      // xvid
      dec_type = VIDEO_DEC_FORMAT_MPEG4_5;
      break;
    case CODEC_TAG_COL1:
    case CODEC_TAG_DIV3:
    case CODEC_TAG_MP43:
      // divx3.11
      dec_type = VIDEO_DEC_FORMAT_MPEG4_3;
      break;
    case CODEC_TAG_DIV4:
    case CODEC_TAG_DIVX:
      // divx4
      dec_type = VIDEO_DEC_FORMAT_MPEG4_4;
      break;
    case CODEC_TAG_DIV5:
    case CODEC_TAG_DX50:
    case CODEC_TAG_M4S2:
    case CODEC_TAG_FMP4:
      // divx5
      dec_type = VIDEO_DEC_FORMAT_MPEG4_5;
      break;
    case CODEC_TAG_DIV6:
      // divx6
      dec_type = VIDEO_DEC_FORMAT_MPEG4_5;
      break;
    case CODEC_TAG_MP4V:
    case CODEC_TAG_RMP4:
    case CODEC_TAG_MPG4:
    case CODEC_TAG_mp4v:
//    case AV_CODEC_ID_MPEG4:
      // mp4
      dec_type = VIDEO_DEC_FORMAT_MPEG4_5;
      break;
//    case AV_CODEC_ID_H263:
    case CODEC_TAG_H263:
    case CODEC_TAG_h263:
    case CODEC_TAG_s263:
    case CODEC_TAG_F263:
      // h263
      dec_type = VIDEO_DEC_FORMAT_H263;
      break;
    case CODEC_TAG_AVC1:
    case CODEC_TAG_avc1:
    case CODEC_TAG_H264:
    case CODEC_TAG_h264:
 //   case AV_CODEC_ID_H264:
      // h264
      if (aml_get_vformat(avctx) == VFORMAT_H264_4K2K)
        dec_type = VIDEO_DEC_FORMAT_H264_4K2K;
      else
        dec_type = VIDEO_DEC_FORMAT_H264;
      break;
//    case AV_CODEC_ID_RV30:
//    //case CODEC_TAG_RV30:
//      // realmedia 3
//      dec_type = VIDEO_DEC_FORMAT_REAL_8;
//      break;
////    case AV_CODEC_ID_RV40:
//    //case CODEC_TAG_RV40:
//      // realmedia 4
//      dec_type = VIDEO_DEC_FORMAT_REAL_9;
//      break;
    case CODEC_TAG_WMV3:
      // wmv3
      dec_type = VIDEO_DEC_FORMAT_WMV3;
      break;
//    case AV_CODEC_ID_VC1:
    case CODEC_TAG_VC_1:
    case CODEC_TAG_WVC1:
    case CODEC_TAG_WMVA:
      // vc1
      dec_type = VIDEO_DEC_FORMAT_WVC1;
      break;
//    case AV_CODEC_ID_VP6F:
//      // vp6
//      dec_type = VIDEO_DEC_FORMAT_SW;
//      break;
//    case AV_CODEC_ID_CAVS:
//    case AV_CODEC_ID_AVS:
//      // avs
//      dec_type = VIDEO_DEC_FORMAT_AVS;
//      break;
    //case AV_CODEC_ID_HEVC:
    case CODEC_TAG_hvc1:
      // h265
      dec_type = VIDEO_DEC_FORMAT_HEVC;
      break;
    default:

      dec_type = VIDEO_DEC_FORMAT_UNKNOW;
      break;
  }

  // if the tag is unknown, default to main types
  if (dec_type == VIDEO_DEC_FORMAT_UNKNOW)
  {
    av_log(avctx, AV_LOG_WARNING, "Unknown Codec Tag 0x%x, trying to override vdec type\n", avctx->codec_tag);
    switch (aml_get_vformat(avctx))
    {
      case VFORMAT_HEVC:
        dec_type = VIDEO_DEC_FORMAT_HEVC;
      break;

      case VFORMAT_H264_4K2K:
        dec_type = VIDEO_DEC_FORMAT_H264_4K2K;
      break;

      case VFORMAT_H264:
        dec_type = VIDEO_DEC_FORMAT_H264;
      break;

      case VFORMAT_VC1:
        dec_type = VIDEO_DEC_FORMAT_WVC1;
      break;

//      case VFORMAT_MPEG12:
//        dec_type = VIDEO_DEC_FORMAT_UNKNOW;
      break;

      default:
        av_log(avctx, AV_LOG_ERROR, "Failed to override vdec type for codec ID %d\n", avctx->codec_id);
      break;
    }
  }

  return dec_type;
}

const char *aml_get_vformat_name(vformat_t format)
{
  switch(format)
  {
    case VFORMAT_MPEG12:      return "MPEG12";
    case VFORMAT_MPEG4:       return "MPEG4";
    case VFORMAT_H264:        return "H264";
    case VFORMAT_MJPEG:       return "MJPEG";
    case VFORMAT_REAL:        return "REAL";
    case VFORMAT_JPEG:        return "JPEG";
    case VFORMAT_VC1:         return "VC1";
    case VFORMAT_AVS:         return "AVS";
    case VFORMAT_SW:          return "SW";
    case VFORMAT_H264MVC:     return "H264MVC";
    case VFORMAT_H264_4K2K :  return "H264_4K2K";
    case VFORMAT_HEVC :       return "HEVC";
    default :                 return "UNSUPPORTED";
  }
}

const char *aml_get_vdec_name(vdec_type_t vdec)
{
  switch(vdec)
  {
    case VIDEO_DEC_FORMAT_MPEG4_3:  return "MPEG3";
    case VIDEO_DEC_FORMAT_MPEG4_4:  return "MPEG4";
    case VIDEO_DEC_FORMAT_MPEG4_5:  return "MPEG5";
    case VIDEO_DEC_FORMAT_H264:     return "H264";
    case VIDEO_DEC_FORMAT_MJPEG:    return "MJPEG";
    case VIDEO_DEC_FORMAT_MP4:      return "MP4";
    case VIDEO_DEC_FORMAT_H263:     return "H263";
    case VIDEO_DEC_FORMAT_REAL_8:   return "REAL_8";
    case VIDEO_DEC_FORMAT_REAL_9:   return "REAL_9";
    case VIDEO_DEC_FORMAT_WMV3:     return "WMV3";
    case VIDEO_DEC_FORMAT_WVC1:     return "WVC1";
    case VIDEO_DEC_FORMAT_SW:       return "SW";
    case VIDEO_DEC_FORMAT_AVS:      return "AVS";
    case VIDEO_DEC_FORMAT_H264_4K2K:return "H264_4K_2K";
    case VIDEO_DEC_FORMAT_HEVC:     return "HEVC";
    default :                       return "UNSUPPORTED";
  }
}
