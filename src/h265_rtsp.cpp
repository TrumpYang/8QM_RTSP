#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <iostream>
#include <cstdlib> // for atoi

// ========== 统一参数，设置默认值 ==========
int h264_entropy_mode = 0;
int video_bitrate_mode = 1;
int h264_i_frame_period = 30;
int video_b_frames = 0;
int video_gop_size = 2;
int video_bitrate = 1500000;     
  // 10 Mbps
int video_peak_bitrate = 400000000;   // 20 Mbps
   

void add_stream(GstRTSPMountPoints *mounts, const char *path, const char *device) {
    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    gchar *launch_str = g_strdup_printf(
        "( v4l2src device=%s ! "
        "video/x-raw,format=NV12,width=1280,height=720 ! "
        "v4l2h264enc output-io-mode=dmabuf-import "
        "extra-controls=\"controls,"
        "h264_entropy_mode=%d,"
        "video_bitrate_mode=%d,"
        "h264_i_frame_period=%d,"
        "video_b_frames=%d,"
        "video_gop_size=%d,"
        "video_bitrate=%d,"
        "video_peak_bitrate=%d; "
        "tune=zerolatency\" ! "
        "rtph264pay name=pay0 pt=96 config-interval=1 )",
        device,
        h264_entropy_mode,
        video_bitrate_mode,
        h264_i_frame_period,
        video_b_frames,
        video_gop_size,
        video_bitrate,
        video_peak_bitrate
    );

    gst_rtsp_media_factory_set_launch(factory, launch_str);
    gst_rtsp_mount_points_add_factory(mounts, path, factory);

    g_free(launch_str);
}

void print_current_settings() {
    std::cout << "======== 当前编码器参数设置 ========" << std::endl;
    std::cout << "h264_entropy_mode = " << h264_entropy_mode << std::endl;
    std::cout << "video_bitrate_mode = " << video_bitrate_mode << std::endl;
    std::cout << "h264_i_frame_period = " << h264_i_frame_period << std::endl;
    std::cout << "video_b_frames = " << video_b_frames << std::endl;
    std::cout << "video_gop_size = " << video_gop_size << std::endl;
    std::cout << "video_bitrate = " << video_bitrate << " bps" << std::endl;
    std::cout << "video_peak_bitrate = " << video_peak_bitrate << " bps" << std::endl;
    std::cout << "====================================" << std::endl;
}

int main(int argc, char *argv[]) {
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    // 支持命令行参数传入
    // 参数顺序: 码率 i帧周期 GOP大小 是否有B帧（0=无B帧）
    if (argc >= 5) {
        video_bitrate = std::atoi(argv[1]);
        h264_i_frame_period = std::atoi(argv[2]);
        video_gop_size = std::atoi(argv[3]);
        video_b_frames = std::atoi(argv[4]);
        // 码率模式如果想自动变化（比如CBR/VBR），可以自己加逻辑
        // video_peak_bitrate = 200000000;
        // video_bitrate_mode = 1; // 固定码率 (CBR)
        std::cout << "[INFO] 成功加载命令行参数!" << std::endl;
    } else {
        std::cout << "[INFO] 使用默认编码参数 VBR." << std::endl;
        std::cout << "用法示例: ./rtsp_server <码率bps> <I帧周期> <GOP大小> <是否有B帧0/1>" << std::endl;
        std::cout << "比如: ./rtsp_server 10000000 30 30 0" << std::endl;
    }
    // 打印当前参数
    print_current_settings();
    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);
    server = gst_rtsp_server_new();
    gst_rtsp_server_set_service(server, "8001");
    mounts = gst_rtsp_server_get_mount_points(server);
    // 添加四路流
    add_stream(mounts, "/cam4", "/dev/video4");
    add_stream(mounts, "/cam5", "/dev/video5");
    add_stream(mounts, "/cam6", "/dev/video6");
    add_stream(mounts, "/cam7", "/dev/video7");
    g_object_unref(mounts);
    gst_rtsp_server_attach(server, NULL);
    //8.148.71.61 是服务器地址 已经通过FRP进行了穿透 8001 映射到了6001
    g_print("RTSP server is running at:\n");
    g_print("  rtsp://8.148.71.61:6001/cam4\n");
    g_print("  rtsp://8.148.71.61:6001/cam5\n");
    g_print("  rtsp://8.148.71.61:6001/cam6\n");
    g_print("  rtsp://8.148.71.61:6001/cam7\n");
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    g_object_unref(server);
    return 0;
}
