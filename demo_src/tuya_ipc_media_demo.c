/*********************************************************************************
  *Copyright(C),2015-2020, 
  *TUYA 
  *www.tuya.comm
  *FileName:    tuya_ipc_media_demo
**********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>  
#include "tuya_ipc_common_demo.h"
#include "tuya_ipc_api.h"
#include "tuya_ring_buffer.h"
#include "tuya_ipc_media_demo.h"
#include "tuya_ipc_media.h"

#include "gst/gst.h"

STATIC IPC_MEDIA_INFO_S s_media_info = {0};
extern CHAR_T s_raw_path[128];

Ring_Buffer_User_Handle_S v264_handle, pcm_handle;
MEDIA_FRAME_S h264_frame = {0}, pcm_frame= {0};
static UINT64_T video_pts = 0, audio_pts = 0;
static GMainLoop* video_loop;
static GMainLoop* audio_loop;


static GstFlowReturn video_new_sample_cb(GstElement *sink, gpointer user_data){
    GstSample* sample = NULL;
    GstBuffer* buffer = NULL;
    GstMapInfo map;

    // const GstStructure* info = NULL;
    // GstCaps* caps = NULL;
    GstFlowReturn ret = GST_FLOW_OK;

    unsigned int FrameLen=0,Frame_start=0;
    unsigned int offset=0;
    unsigned int IsKeyFrame=0;
    

    // equals to gst_app_sink_pull_sample (GST_APP_SINK_CAST (appsink), sample);
    g_signal_emit_by_name (sink, "pull-sample", &sample, &ret);
    if (ret != GST_FLOW_OK) {
        printf ("can't pull video GstSample.");
        return ret;
    }
    
    if (sample) {

        buffer = gst_sample_get_buffer (sample);
        if ( buffer == NULL ) {
            printf ("get buffer is null");
            goto exit;
        }

        // caps = gst_sample_get_caps (sample);
        // if ( caps == NULL ) {
        //     printf ("get caps is null");
        //     goto exit;
        // }

        // info = gst_caps_get_structure (caps, 0);
        // if ( info == NULL ) {
        //     printf ("get info is null");
        //     goto exit;
        // }
        
        gst_buffer_map (buffer, &map, GST_MAP_READ);

        if (map.data == NULL) {
            printf ("appsink buffer data empty\n");
            return GST_FLOW_OK;
        }
        printf("video rx map.date is %02X %02X %02X %02X %02X %02X. map.size is %d, map.maxsize is %d.\n", map.data[0], map.data[1], map.data[2], map.data[3], map.data[4], map.data[5], map.size, map.maxsize);
        // read_one_frame_from_map(map.data, offset, map.size, &IsKeyFrame, &FrameLen, &Frame_start);

        unsigned char NalType = map.data[4] & 0x1f;
        if(NalType == 0x7)  // 0x67
        {
            h264_frame.type = E_VIDEO_I_FRAME;
            // printf ("this is key frame ,FrameLen is %d. ",map.size);
        }
        else if(NalType ==0x1)  //0x41     
        {
            h264_frame.type = E_VIDEO_PB_FRAME;
            // printf ("this is no-key frame ,FrameLen is %d. ",map.size);
        }

        h264_frame.size = map.size;
        int frameRate = 30;
        video_pts += 1000000/frameRate;
        // printf ("this frame start is %d\n",Frame_start);
        h264_frame.p_buf = map.data + Frame_start;
        h264_frame.pts = video_pts;

        /* Send HD video data to the SDK */
        // printf("v264_handel is %d \n",v264_handle);
        ret = TUYA_APP_Put_Frame(v264_handle, &h264_frame);
        // printf("put_frame ret is %d \n",ret);

        // usleep(h264_frame.pts);

    }
    else{
        g_print("sample is NULL \n");
        return ret;
    }

    exit:
    if (buffer) {
        gst_buffer_unmap (buffer, &map);
    }
    if (sample) {
        gst_sample_unref (sample);
    }

    return GST_FLOW_OK;
}

static GstFlowReturn audio_new_sample_cb(GstElement *sink, gpointer user_data){
    GstSample* sample = NULL;
    GstBuffer* buffer = NULL;
    GstMapInfo map;

    // const GstStructure* info = NULL;
    // GstCaps* caps = NULL;
    GstFlowReturn ret = GST_FLOW_OK;
    

    // equals to gst_app_sink_pull_sample (GST_APP_SINK_CAST (appsink), sample);
    g_signal_emit_by_name (sink, "pull-sample", &sample, &ret);
    if (ret != GST_FLOW_OK) {
        printf ("can't pull audio GstSample.");
        return ret;
    }
    
    if (sample) {

        buffer = gst_sample_get_buffer (sample);
        if ( buffer == NULL ) {
            printf ("get buffer is null");
            goto exit;
        }

        // caps = gst_sample_get_caps (sample);
        // if ( caps == NULL ) {
        //     printf ("get caps is null");
        //     goto exit;
        // }

        // info = gst_caps_get_structure (caps, 0);
        // if ( info == NULL ) {
        //     printf ("get info is null");
        //     goto exit;
        // }
        
        gst_buffer_map (buffer, &map, GST_MAP_READ);

        if (map.data == NULL) {
            printf ("appsink buffer data empty\n");
            return GST_FLOW_OK;
        }
        printf("audio rx map.date is %d %d %d %d %d %d. map.size is %d, map.maxsize is %d .\n", map.data[0], map.data[1], map.data[2], map.data[3], map.data[4], map.data[5], map.size, map.maxsize);

        pcm_frame.type = E_AUDIO_FRAME;
        pcm_frame.size = map.size;
        int frameRate = 25;
        audio_pts += 1000000/frameRate;
        // printf ("this frame start is %d\n",Frame_start);
        pcm_frame.p_buf = map.data;
        pcm_frame.pts = audio_pts;

        /* Send pcm data to the SDK */
        // printf("pcm_handel is %d \n",pcm_handle);
        ret = TUYA_APP_Put_Frame(pcm_handle, &pcm_frame);
        // printf("put_frame ret is %d \n",ret);

    }
    else{
        g_print("sample is NULL \n");
        return ret;
    }

    exit:
    if (buffer) {
        gst_buffer_unmap (buffer, &map);
    }
    if (sample) {
        gst_sample_unref (sample);
    }

    return GST_FLOW_OK;
}


static void
on_stream_status (GstBus     *bus,
                  GstMessage *message,
                  gpointer    user_data)
{
  GstStreamStatusType type;
  GstElement *owner;
  const GValue *val;
  GstTask *task = NULL;

  gst_message_parse_stream_status (message, &type, &owner);

  val = gst_message_get_stream_status_object (message);

  /* see if we know how to deal with this object */
  if (G_VALUE_TYPE (val) == GST_TYPE_TASK) {
    task = g_value_get_object (val);
  }

  switch (type) {
    case GST_STREAM_STATUS_TYPE_CREATE:
      if (task) {
        GstTaskPool *pool;
        
        // pool = g_object_new (TEST_TYPE_RT_POOL, NULL);

        gst_task_set_pool (task, pool);
      }
      break;
    default:
      break;
  }
}

static void
on_error (GstBus     *bus,
          GstMessage *message,
          gpointer    user_data)
{
  g_message ("received ERROR");
  g_main_loop_quit (video_loop);
}

static void
on_eos (GstBus     *bus,
        GstMessage *message,
        gpointer    user_data)
{
  g_main_loop_quit (video_loop);
}



/* called when we get a GstMessage from the source pipeline when we get EOS, we
 * notify the appsrc of it. */
static gboolean
on_appsink_message (GstBus * bus, GstMessage * message, GstElement * pipeline)
{
    GstElement *source;
 
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_EOS:
            g_print ("The source got dry\n");
            source = gst_bin_get_by_name (GST_BIN (pipeline), "testsource");
            //gst_app_src_end_of_stream (GST_APP_SRC (source));
            g_signal_emit_by_name (source, "end-of-stream", NULL);
            gst_object_unref (source);
            break;
        case GST_MESSAGE_ERROR:
            g_print ("Received error\n");
            g_main_loop_quit (video_loop);
            break;
        default:
            break;
    }
    return TRUE;
}


/* called when we get a GstMessage from the sink pipeline when we get EOS, we
 * exit the mainloop and this testapp. */
static gboolean
on_appsrc_message (GstBus * bus, GstMessage * message, GMainLoop * loop)
{
    /* nil */
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_EOS:
            g_print ("Finished playback\n");
            g_main_loop_quit (loop);
            break;
        case GST_MESSAGE_ERROR:
            g_print ("Received error\n");
            g_main_loop_quit (loop);
            break;
        default:
            break;
    }
    return TRUE;
}


int *gst_media_pipeline_video()
{
    v264_handle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_VIDEO_MAIN, E_RBUF_WRITE);
    printf("v264_handel is %d \n",v264_handle);

    GstBuffer *buffer;
    // GstMapInfo map;

    gchar *device_name = NULL, *caps = NULL;
    gchar *string = NULL;

    GstElement *pipeline, *v4l2_source, *tee, *zmq_queue, *audio_resample;
    GstElement *video_queue, *video_scale, *video_rate, *video_convert, *video_encoder, *h264_sink;

    GstPad *tee_zmq_pad, *queue_video_pad;

    GstCaps *video_caps_rate, *video_caps_scale, *video_caps_h264;
    // GMainLoop *main_loop;  /* GLib's Main Loop */
    GstBus *bus;

/* Initialize GStreamer */
    // gst_init (&argc, &argv);
    gst_init (NULL, NULL);
    video_loop = g_main_loop_new (NULL, FALSE);

/* Create the elements */
    device_name = g_strdup ("usb-Sonix_Technology_Co.__Ltd._USB_2.0_Camera_SN0001-video-index0");
    caps = g_strdup ("video/x-raw,width=640,height=360,framerate=30/1");

    string =
      g_strdup_printf
      ("v4l2src device=/dev/v4l/by-id/%s ! videorate ! capsfilter caps=\"video/x-raw,width=1280,height=720,framerate=30/1\" ! videoscale ! capsfilter caps=\"%s\" ! mpph264enc header-mode=each-idr rc-mode=0 bps-max=512000 gop=30 ! capsfilter caps=\"video/x-h264, stream-format=byte-stream\" ! appsink name=h264_sink",
      device_name, caps);

    pipeline = gst_parse_launch (string, NULL);

    g_free (device_name);
    g_free (string);


    if (pipeline == NULL) {
        g_print ("Bad sink\n");
        return -1;
    }


/* Create the empty pipeline */
    bus = gst_element_get_bus (pipeline);
    gst_bus_enable_sync_message_emission (bus);
    gst_bus_add_watch (bus, (GstBusFunc) on_appsink_message, pipeline);


    h264_sink = gst_bin_get_by_name (GST_BIN (pipeline), "h264_sink");

    if (!pipeline || !h264_sink ) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }


    g_object_set (h264_sink, "emit-signals", TRUE, NULL);
    g_signal_connect (h264_sink, "new-sample", G_CALLBACK (video_new_sample_cb), NULL);

/* setup */
    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
    g_main_loop_run (video_loop);

/* unset */
    tuya_ipc_ring_buffer_close(v264_handle);

    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    gst_object_unref (bus);
    g_main_loop_unref (video_loop);

    return 0;

}

int *gst_media_pipeline_audio()
{
    pcm_handle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_AUDIO_MAIN, E_RBUF_WRITE);
    printf("pcm_handel is %d \n",pcm_handle);

    GstBuffer *buffer;
    // GstMapInfo map;

    gchar *device_name = NULL, *caps = NULL;
    gchar *string = NULL;

    GstElement *pipeline, *tee, *zmq_queue, *pcm_sink;

    GstPad *tee_zmq_pad, *queue_video_pad;

    GstCaps *audio_caps_pcm;
    // GMainLoop *main_loop;  /* GLib's Main Loop */
    GstBus *bus;

/* Initialize GStreamer */
    // gst_init (&argc, &argv);
    gst_init (NULL, NULL);
    audio_loop = g_main_loop_new (NULL, FALSE);

/* Create the elements */
    device_name = g_strdup ("hw:Camera,0");
    caps = g_strdup ("audio/x-raw,format=S16LE,rate=8000,channels=1");

    string =
      g_strdup_printf
      ("alsasrc device=\"%s\" blocksize=640 ! capsfilter caps=\"%s\" ! appsink name=pcm_sink",
      device_name, caps);

    pipeline = gst_parse_launch (string, NULL);

    g_free (device_name);
    g_free (string);


    if (pipeline == NULL) {
        g_print ("Bad sink\n");
        return -1;
    }


/* Create the empty pipeline */
    bus = gst_element_get_bus (pipeline);
    gst_bus_enable_sync_message_emission (bus);
    gst_bus_add_watch (bus, (GstBusFunc) on_appsink_message, pipeline);


    pcm_sink = gst_bin_get_by_name (GST_BIN (pipeline), "pcm_sink");

    if (!pipeline || !pcm_sink ) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }


    g_object_set (pcm_sink, "emit-signals", TRUE, NULL);
    g_signal_connect (pcm_sink, "new-sample", G_CALLBACK (audio_new_sample_cb), NULL);

/* setup */
    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
    g_main_loop_run (audio_loop);

/* unset */
    tuya_ipc_ring_buffer_close(pcm_handle);

    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    gst_object_unref (bus);
    g_main_loop_unref (audio_loop);

    return 0;

}

int gst_media_pipeline()
{
    pthread_t h264_video_thread;
    pthread_create(&h264_video_thread, NULL, gst_media_pipeline_video, NULL);
    pthread_detach(h264_video_thread);

    pthread_t pcm_audio_thread;
    pthread_create(&pcm_audio_thread, NULL, gst_media_pipeline_audio, NULL);
    pthread_detach(pcm_audio_thread);

    return 0;

}

int gst_media_pipeline_video_old(int argc, char *argv[])
{
    v264_handle = tuya_ipc_ring_buffer_open(0, 0, 0, E_RBUF_WRITE);

    GstBuffer *buffer;
    GstMapInfo map;

    GstElement *pipeline, *v4l2_source, *tee, *zmq_queue, *audio_resample;
    GstElement *video_queue, *video_scale, *video_rate, *video_convert, *video_encoder, *h264_sink;

    GstPad *tee_zmq_pad, *queue_video_pad;

    GstCaps *video_caps_rate, *video_caps_scale, *video_caps_h264;
    // GMainLoop *main_loop;  /* GLib's Main Loop */
    GstBus *bus;

/* Initialize GStreamer */
    gst_init (&argc, &argv);
    video_loop = g_main_loop_new (NULL, FALSE);

/* Create the elements */
    v4l2_source = gst_element_factory_make ("v4l2src", "v4l2src0");
    video_scale = gst_element_factory_make ("videoscale", "videoscale0");
    video_rate = gst_element_factory_make ("videorate", "videorate0");
    video_encoder = gst_element_factory_make ("mpph264enc", "mpph264enc0");
    h264_sink = gst_element_factory_make ("appsink", "appsink0");

/* Create the empty pipeline */
    pipeline = gst_pipeline_new ("video_capture");
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
     gst_bus_enable_sync_message_emission (bus);
    gst_bus_add_signal_watch (bus);

    g_signal_connect (bus, "sync-message::stream-status",
      (GCallback) on_stream_status, NULL);
    g_signal_connect (bus, "message::error",
      (GCallback) on_error, NULL);
    g_signal_connect (bus, "message::eos",
      (GCallback) on_eos, NULL);


    if (!pipeline || !v4l2_source || !video_scale || !video_rate || !video_encoder || !h264_sink ) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    gchar *video_caps_text;

    video_caps_text = g_strdup_printf ("video/x-raw,width=1280,height=720,framerate=30/1");
    video_caps_rate = gst_caps_from_string (video_caps_text);
    if(!video_caps_rate){
        g_printerr("gst_caps_from_string: after rate fail\n");
        return -1;
    }
    g_object_set (video_scale, "caps", video_caps_rate, NULL);

    video_caps_text = g_strdup_printf ("video/x-raw,width=640,height=360,framerate=30/1");
    video_caps_scale = gst_caps_from_string (video_caps_text);
    if(!video_caps_scale){
        g_printerr("gst_caps_from_string: after scale fail\n");
        return -1;
    }
    g_object_set (video_encoder, "caps", video_caps_scale, NULL);

    video_caps_text = g_strdup_printf ("video/x-h264, stream-format=byte-stream");
    video_caps_h264 = gst_caps_from_string (video_caps_text);
    if(!video_caps_h264){
        g_printerr("gst_caps_from_string: after h264enc fail\n");
        return -1;
    }
    g_object_set (h264_sink, "caps", video_caps_h264, NULL);

    g_object_set (G_OBJECT (v4l2_source), "device", "/dev/video2", NULL);
    g_object_set (G_OBJECT (video_encoder), "rc-mode", "0", "bps", "480000", "bps-max", "512000", "gop", "60", NULL);


    g_object_set (h264_sink, "emit-signals", TRUE, NULL);
    g_signal_connect (h264_sink, "new-sample", G_CALLBACK (video_new_sample_cb), NULL);

/* setup */
    gst_bin_add_many (GST_BIN (pipeline), v4l2_source, video_rate, video_scale, video_encoder, h264_sink, NULL);
    gst_element_link_many(v4l2_source, video_rate, video_scale, video_encoder, h264_sink, NULL);
    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
    g_main_loop_run (video_loop);

/* unset */
    tuya_ipc_ring_buffer_close(v264_handle);

    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    gst_object_unref (bus);
    g_main_loop_unref (video_loop);

    return 0;

}


/* Set audio and video properties */
VOID IPC_APP_Set_Media_Info(IPC_MEDIA_INFO_S *media)
{
    memcpy(&s_media_info, media, sizeof(IPC_MEDIA_INFO_S));

    PR_DEBUG("channel_enable:%d %d %d", s_media_info.channel_enable[0], s_media_info.channel_enable[1], s_media_info.channel_enable[2]);

    PR_DEBUG("fps:%u", s_media_info.video_fps[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("gop:%u", s_media_info.video_gop[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("bitrate:%u kbps", s_media_info.video_bitrate[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("video_main_width:%u", s_media_info.video_width[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("video_main_height:%u", s_media_info.video_height[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("video_freq:%u", s_media_info.video_freq[E_IPC_STREAM_VIDEO_MAIN]);
    PR_DEBUG("video_codec:%d", s_media_info.video_codec[E_IPC_STREAM_VIDEO_MAIN]);

    PR_DEBUG("audio_codec:%d", s_media_info.audio_codec[E_IPC_STREAM_AUDIO_MAIN]);
    PR_DEBUG("audio_sample:%d", s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN]);
    PR_DEBUG("audio_databits:%d", s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN]);
    PR_DEBUG("audio_channel:%d", s_media_info.audio_channel[E_IPC_STREAM_AUDIO_MAIN]);
}

IPC_MEDIA_INFO_S *IPC_APP_Get_Media_Info()
{
    return &s_media_info;
}
/*
 * The sample code simulates audio and video by reading and writing files in rawfiles.tar.gz
 */
#define AUDIO_FRAME_SIZE 640
#define AUDIO_FPS 25
#define VIDEO_BUF_SIZE	(1024 * 400) //Maximum frame

/* This is for demo only. Should be replace with real PCM/AAC/G711 encoder output */
void *thread_live_audio(void *arg)
{
    char fullpath[128] = {0};
    sprintf(fullpath, "%s/resource/media/demo_audio.raw", s_raw_path);

	Ring_Buffer_User_Handle_S a_handle = tuya_ipc_ring_buffer_open(0, 0, E_IPC_STREAM_AUDIO_MAIN, E_RBUF_WRITE);
	if(a_handle == NULL)
	{
		pthread_exit(0);
	}

    FILE *aFp = fopen(fullpath, "rb");
    if(aFp == NULL)
    {
        printf("can't read live audio file %s\n",fullpath);
        pthread_exit(0);
    }
    char audioBuf[AUDIO_FRAME_SIZE];
    MEDIA_FRAME_S pcm_frame = {0};
    pcm_frame.type = E_AUDIO_FRAME;

    while(1)
    {
        int size = fread(audioBuf, 1, AUDIO_FRAME_SIZE, aFp);
        if(size < AUDIO_FRAME_SIZE)
        {
            rewind(aFp);
            continue;
        }
        printf("audio-file date is %d %d %d %d %d %d. size is %d.\n", audioBuf[0], audioBuf[1], audioBuf[2], audioBuf[3], audioBuf[4], audioBuf[5], size);
        int frameRate = AUDIO_FPS;
        int sleepTick = 1000000/frameRate;
        static UINT64_T pts = 0;
        pts += sleepTick;
        pcm_frame.size = size;
        pcm_frame.p_buf = audioBuf;
        pcm_frame.pts = pts;
//        TUYA_APP_Put_Frame(E_IPC_STREAM_AUDIO_MAIN,&pcm_frame);
        TUYA_APP_Put_Frame(a_handle,&pcm_frame);


        usleep(sleepTick);
    }
    tuya_ipc_ring_buffer_close(a_handle);   
    pthread_exit(0);
}

int read_one_frame_from_map(unsigned char *pVideoBuf,unsigned int offset,unsigned int BufSize,unsigned int *IskeyFrame,unsigned int 
*FramLen,unsigned int *Frame_start)
{
    int pos = 0;

    unsigned char NalType=0;

    if(BufSize<=5)
    {
        printf("bufSize is too small\n");
        return -1;
    }

        // if(pVideoBuf[pos]==0x00
        //     &&pVideoBuf[pos+1]==0x00
        //     &&pVideoBuf[pos+2]==0x00
        //     &&pVideoBuf[pos+3]==0x01)
        // {
            NalType = pVideoBuf[pos + 4] & 0x1f;
            if(NalType == 0x7 || NalType == 0x5)  // 0x67 0x65
            {
                *IskeyFrame = 1;
                *FramLen = BufSize;
                *Frame_start = offset+pos;
            }
            else if(NalType ==0x1)  //0x41     
            {
                *IskeyFrame = 0;
                *FramLen = BufSize;
                *Frame_start = offset+pos;
            }
        // }
    
    return 0;
}



/* This is for demo only. Should be replace with real H264 encoder output */
int read_one_frame_from_demo_video_file(unsigned char *pVideoBuf,unsigned int offset,unsigned int BufSize,unsigned int *IskeyFrame,unsigned int 
*FramLen,unsigned int *Frame_start)
{
    int pos = 0;
    int bNeedCal = 0;
    unsigned char NalType=0;
    int idx=0;
    if(BufSize<=5)
    {
        printf("bufSize is too small\n");
        return -1;
    }
    for(pos=0;pos <= BufSize-5;pos++)
    {
        if(pVideoBuf[pos]==0x00
            &&pVideoBuf[pos+1]==0x00
            &&pVideoBuf[pos+2]==0x00
            &&pVideoBuf[pos+3]==0x01)
        {
            NalType = pVideoBuf[pos + 4] & 0x1f;
            if(NalType == 0x7)
            {
                if(bNeedCal==1)
                {
                    *FramLen=pos-idx;
                    return 0;
                }
                
                *IskeyFrame = 1;
                *Frame_start = offset+pos;
                bNeedCal=1;
                idx=pos;
            }
            else if(NalType ==0x1)                
            {
               if(bNeedCal)
               {
                  *FramLen=pos-idx;
                  return 0;
               }
               *Frame_start=offset+pos;
               *IskeyFrame = 0;
               idx = pos;
               bNeedCal=1;
            }
        }
    }
    
    return 0;
}

void *thread_live_video(void *arg)
{
    char raw_fullpath[128] = {0};
    char info_fullpath[128] = {0};
    unsigned int FrameLen=0,Frame_start=0;
    unsigned int offset=0;
    unsigned int IsKeyFrame=0;
    unsigned char *pVideoBuf = NULL;
    sprintf(raw_fullpath, "%s/resource/media/demo_video.264", s_raw_path);

    PR_DEBUG("start live video using %s",raw_fullpath);

	Ring_Buffer_User_Handle_S v_handle = tuya_ipc_ring_buffer_open(0, 0, 0, E_RBUF_WRITE);
	if(v_handle == NULL)
	{
		pthread_exit(0);
	}

    FILE *streamBin_fp = fopen(raw_fullpath, "rb");
    if((streamBin_fp == NULL))
    {
        printf("can't read live video file %s\n",raw_fullpath);
        pthread_exit(0);
    }
    fseek(streamBin_fp, 0, SEEK_END);
    UINT_T file_size = ftell(streamBin_fp);
    fseek(streamBin_fp, 0, SEEK_SET);
    pVideoBuf = malloc(file_size);
    fread(pVideoBuf, 1, file_size, streamBin_fp);

    MEDIA_FRAME_S h264_frame = {0};
    while(1)
    {
        offset=Frame_start+FrameLen;
        if(offset>=file_size)
        {
            offset=0;
        }
        // printf("video-file date is %02X %02X %02X %02X %02X %02X. ", *(pVideoBuf+offset), *(pVideoBuf+offset+1), *(pVideoBuf+offset+2), *(pVideoBuf+offset+3), *(pVideoBuf+offset+4), *(pVideoBuf+offset+5));

        read_one_frame_from_demo_video_file(pVideoBuf+offset,offset,file_size-offset,&IsKeyFrame,&FrameLen,&Frame_start);
        //Note: For I frame of H264, SPS/PPS/SEI/IDR should be combined within one frame, and the NALU separator should NOT be deleted.
        if(IsKeyFrame==1)
        {
            h264_frame.type = E_VIDEO_I_FRAME;
            h264_frame.size = FrameLen;
        }
        else
        {
            h264_frame.type = E_VIDEO_PB_FRAME;
            h264_frame.size = FrameLen;
        }
        // printf("size is %d .\n",h264_frame.size);

        int frameRate = 30;
        int sleepTick = 1000000/frameRate;
        static UINT64_T pts = 0;
        pts += sleepTick;

        h264_frame.p_buf = pVideoBuf+Frame_start;
        h264_frame.pts = pts;

        /* Send HD video data to the SDK */
        TUYA_APP_Put_Frame(v_handle, &h264_frame);

        usleep(sleepTick);
    }
    tuya_ipc_ring_buffer_close(v_handle);
    pthread_exit(0);
}

//According to different chip platforms, users need to implement the interface of capture.
int IPC_APP_get_snapshot(char *snap_addr, int *snap_size)
{
    //we use file to simulate
    char snapfile[128];
    *snap_size = 0;
    extern char s_raw_path[];
    printf("get one motion snapshot\n");
    snprintf(snapfile,64,"%s/resource/media/demo_snapshot.jpg",s_raw_path);
    FILE*fp = fopen(snapfile,"r+");
    if(NULL == fp)
    {
        printf("fail to open snap.jpg\n");
        return -1;
    }
    fseek(fp,0,SEEK_END);
    *snap_size = ftell(fp);
    if(*snap_size < 100*1024)
    {
        fseek(fp,0,SEEK_SET);
        fread(snap_addr,*snap_size,1,fp);
    }
    fclose(fp);
    return 0;
}

VOID IPC_APP_Init_Media_Task()
{
#ifdef __HuaweiLite__
    TSK_INIT_PARAM_S stappTask;
    int taskid = -1;
    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_live_video;
    stappTask.uwStackSize  = 0x80000;
    stappTask.pcName = "live_video";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    LOS_TaskCreate((UINT32 *)&taskid, &stappTask);

    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_live_audio;
    stappTask.pcName = "live_video";
    LOS_TaskCreate((UINT32 *)&taskid, &stappTask);
#else
    pthread_t h264_output_thread;
    pthread_create(&h264_output_thread, NULL, thread_live_video, NULL);
    pthread_detach(h264_output_thread);

    pthread_t pcm_output_thread;
    pthread_create(&pcm_output_thread, NULL, thread_live_audio, NULL);
    pthread_detach(pcm_output_thread);
#endif    

    return;
}

/*
---------------------------------------------------------------------------------
code related RingBuffer
---------------------------------------------------------------------------------
*/
OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID)
{
	OPERATE_RET ret = OPRT_OK;

    STATIC BOOL_T s_ring_buffer_inited = FALSE;
    if(s_ring_buffer_inited == TRUE)
    {
        PR_DEBUG("The Ring Buffer Is Already Inited");
        return OPRT_OK;
    }

    IPC_STREAM_E ringbuffer_stream_type;
   // CHANNEL_E channel;
    Ring_Buffer_Init_Param_S param={0};
    for( ringbuffer_stream_type = E_IPC_STREAM_VIDEO_MAIN; ringbuffer_stream_type < E_IPC_STREAM_MAX; ringbuffer_stream_type++ )
    {
        PR_DEBUG("init ring buffer Channel:%d Enable:%d", ringbuffer_stream_type, s_media_info.channel_enable[ringbuffer_stream_type]);
        if(s_media_info.channel_enable[ringbuffer_stream_type] == TRUE)
        {
            if(ringbuffer_stream_type >= E_IPC_STREAM_AUDIO_MAIN)
            {
                param.bitrate = s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN]*s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN]/1024;
                param.fps = s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN];
                param.max_buffer_seconds = 0;
                param.requestKeyFrameCB = NULL;
                PR_DEBUG("audio_sample %d, audio_databits %d, audio_fps %d",s_media_info.audio_sample[E_IPC_STREAM_AUDIO_MAIN],s_media_info.audio_databits[E_IPC_STREAM_AUDIO_MAIN],s_media_info.audio_fps[E_IPC_STREAM_AUDIO_MAIN]);
                ret = tuya_ipc_ring_buffer_init(0,0,ringbuffer_stream_type,&param);
            }
            else
            {
                param.bitrate = s_media_info.video_bitrate[ringbuffer_stream_type];
                param.fps = s_media_info.video_fps[ringbuffer_stream_type];
                param.max_buffer_seconds = 0;
                param.requestKeyFrameCB = NULL;
                PR_DEBUG("video_bitrate %d, video_fps %d",s_media_info.video_bitrate[ringbuffer_stream_type],s_media_info.video_fps[ringbuffer_stream_type]);
                ret = tuya_ipc_ring_buffer_init(0,0,ringbuffer_stream_type, &param);
            }
            if(ret != 0)
            {
                PR_ERR("init ring buffer fails. %d %d", ringbuffer_stream_type, ret);
                return OPRT_MALLOC_FAILED;
            }
            PR_DEBUG("init ring buffer success. channel:%d", ringbuffer_stream_type);
        }
    }

    s_ring_buffer_inited = TRUE;

    return OPRT_OK;
}

OPERATE_RET TUYA_APP_Put_Frame(Ring_Buffer_User_Handle_S handle, IN CONST MEDIA_FRAME_S *p_frame)
{
    PR_TRACE("Put Frame. type:%d size:%u pts:%llu ts:%llu",
             p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);

    OPERATE_RET ret = tuya_ipc_ring_buffer_append_data(handle,p_frame->p_buf, p_frame->size,p_frame->type,p_frame->pts);

    if(ret != OPRT_OK)
    {
        PR_ERR("Put Frame Fail.%d  type:%d size:%u pts:%llu ts:%llu",ret,
                  p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);
    }
    return ret;
}



