/*
 * 動画ファイルの受け取り関係の処理をする
 * 2023/06/14 Kawa_09
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#include "VideoInput.h"

#define FILE_PATH_BUF 256

// 動画のパスをターミナルから受け取る（仮）
char* GetVideoPath() {
    char* input = NULL;
    char* videoPath = NULL;


    // 文字列の保存用にメモリを確保
    input = (char*)malloc(sizeof(char) * FILE_PATH_BUF);
    videoPath = (char*)malloc(sizeof(char) * FILE_PATH_BUF);


    // メモリの確保に失敗したら終了
    if( input == NULL) {
        fprintf(stderr, "input：文字列配列の作成に失敗しました\n");
    }else {
        fprintf(stderr, "input：文字列配列の作成に成功しました\n");
    }
    if( videoPath == NULL) {
        fprintf(stderr, "videoPath：文字列配列の作成に失敗しました\n");
    }else {
        fprintf(stderr, "videoPath：文字列配列の作成に成功しました\n");
    }

    // inputに動画までの絶対パスを受け取る
    printf("動画の絶対パスを入力してください\n");
    scanf("%s", input);
    getchar();// デバッグ用。scanf後のreturnキー入力の残りをとる

    // videoPathにルートディレクトリを表す/と、動画までの絶対パスを付与する
    strcpy(videoPath,"/");
    strcat(videoPath,input);

    // inputのメモリを解放する
    free(input);

    return videoPath;
}

// ffmpegを使用して動画からRGBピクセルデータを取得する
void VideoToBit(PixFrameData *pixCtx,char* videoPath,int width,int height) {

    // スケーリングコンテキストの確保
    struct SwsContext *swsCtx  = NULL;

    // フレームの読み込みループ
    AVFormatContext *fmtCtx = NULL;
    AVCodecContext  *decCtx = NULL;
    AVPacket  pkt;
    int ret;

    // 入力ファイルのオープン
    ret = avformat_open_input(&fmtCtx, videoPath, NULL, NULL);
    if(ret < 0) {
       fprintf(stderr, "could not open input file\n");
       exit(1);
    }

    // ストリーム情報の取得
    ret = avformat_find_stream_info(fmtCtx, NULL);
    if(ret < 0) {
        fprintf(stderr, "could not open stream information\n");
        exit(1);
    }

    // デコーダーの探索
    AVCodec *dec = NULL;
    int videoStreamIndex;
    videoStreamIndex = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO,
                                           -1,-1,&dec,0);
    if(videoStreamIndex < 0){
        fprintf(stderr, "Could not find video strea\n");
        exit(1);
    }
    // デコーダーコンテキストの確保
    decCtx = avcodec_alloc_context3(dec);

    // デコーダーコンテキストの設定
    ret = avcodec_parameters_to_context(decCtx,
                                        fmtCtx->streams[videoStreamIndex]->codecpar);
    if(ret < 0){
        fprintf(stderr, "Could not copy codec parameters\n");
        exit(1);
    }

    // デコーダーのオープン
    ret = avcodec_open2(decCtx, dec, NULL);
    if(ret < 0){
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    // パケットとフレームの確保
    pkt.data = NULL;
    pkt.size = 0;
    // 入力フレームと出力フレームの確保
    AVFrame *inFrame = NULL;
    AVFrame *outFrame = NULL;

    // 入力フレームと出力フレームのフォーマットとサイズの設定
    int inWidth = fmtCtx->streams[videoStreamIndex]->codecpar->width; // 入力フレームの幅
    int inHeight = fmtCtx->streams[videoStreamIndex]->codecpar->height; // 入力フレームの高さ
    enum AVPixelFormat inPixFmt = AV_PIX_FMT_YUV420P; // 入力フレームのピクセルフォーマット

    int outWidth = width; // 出力フレームの幅
    int outHeight = height; // 出力フレームの高さ
    enum AVPixelFormat outPixFmt = AV_PIX_FMT_RGB24; // 出力フレームのピクセルフォーマット

    int64_t numFrames; // データ格納用構造体のフレーム数

    // スケーリングコンテキストの初期化
    swsCtx = sws_getContext(inWidth,inHeight,inPixFmt,
                            outWidth,outHeight,outPixFmt,
                            SWS_BILINEAR,NULL,NULL,NULL);

    // 入力フレームの確保と初期化
    inFrame = av_frame_alloc();
    inFrame->width = inWidth;
    inFrame->height = inHeight;
    inFrame->format = inPixFmt;
    av_image_alloc(inFrame->data, inFrame->linesize,
                   inFrame->width, inFrame->height,
                   inFrame->format, 32);

    // 出力フレームの確保と初期化
    outFrame = av_frame_alloc();
    outFrame->width = outWidth;
    outFrame->height = outHeight;
    outFrame->format = outPixFmt;
    av_image_alloc(outFrame->data, outFrame->linesize,
                   outFrame->width, outFrame->height,
                   outFrame->format, 32);

    // データ格納用の構造体の初期化
    numFrames = fmtCtx->streams[videoStreamIndex]->duration;
    pixCtx->numFrames = numFrames;

    // pixCtxのメモリを確保する
    (*pixCtx).pix = (RGB***) malloc((*pixCtx).numFrames * sizeof(RGB**));
    for(int i = 0; i < (*pixCtx).numFrames; i++) {
        (*pixCtx).pix[i] = (RGB**) malloc(outHeight * sizeof(RGB*));
        for(int j = 0; j < outHeight; j++) {
            (*pixCtx).pix[i][j] = (RGB*) malloc(outWidth * sizeof(RGB));
        }
    }
    int frameIndex = 0;

    while(1) {
        // パケットの読み込み
        ret = av_read_frame(fmtCtx, &pkt);
        if (ret < 0) {
            break; // EOF or error
        }

        // パケットがビデオストリームに属するかチェック
        if (pkt.stream_index == videoStreamIndex) {
            // パケットの送信
            ret = avcodec_send_packet(decCtx, &pkt);
            if (ret < 0) {
                fprintf(stderr, "Error sending packet\n");
                exit(1);
            }

            while (ret >= 0) {
                // フレームの受信
                ret = avcodec_receive_frame(decCtx, inFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break; // フレームがないか、終了した場合はループを抜ける
                } else if (ret < 0) {
                    fprintf(stderr, "Error receiving frame\n");
                    exit(1);
                }

                // 入力フレームから出力フレームへの変換
                sws_scale(swsCtx,
                          (const uint8_t *const *) inFrame->data,
                          inFrame->linesize,
                          0,
                          inHeight,
                          outFrame->data,
                          outFrame->linesize);

                // 出力フレームからRGBピクセルデータの取得
                uint8_t *rgbData = outFrame->data[0]; // RGBプレーンの先頭アドレス
                int rgbLinesize = outFrame->linesize[0]; // RGBプレーンの１行あたりのバイト数

                // RGBプレーンを操作してピクセルデータを処理する
                for (int y = 0; y < outHeight; y++) {
                    for (int x = 0; x < outWidth; x++) {
                        uint8_t r = rgbData[y * rgbLinesize + x * 3]; // (x,y)座標の赤成分
                        uint8_t g = rgbData[y * rgbLinesize + x * 3 + 1]; //  (x,y)座標の緑成分
                        uint8_t b = rgbData[y * rgbLinesize + x * 3 + 2]; //  (x,y)座標の青成分

                        // 操作して得られたピクセルデータを構造体に格納する
                        pixCtx->pix[frameIndex][y][x].r = (float)r/255;
                        pixCtx->pix[frameIndex][y][x].g = (float)g/255;
                        pixCtx->pix[frameIndex][y][x].b = (float)b/255;
                    }
                }

                frameIndex++;
            }

        }
        // パケットを解放する
        av_packet_unref(&pkt);

    }
    // リソースを解放する
    avcodec_free_context(&decCtx);
    avformat_close_input(&fmtCtx);
    av_freep(&inFrame->data[0]);
    av_freep(&outFrame->data[0]);
    av_frame_free(&inFrame);
    av_frame_free(&outFrame);
    sws_freeContext(swsCtx);
}// end of VideoToBit()

// RGBピクセルデータの並び替えを行う
void ChangePixFrame(PixFrameData *pixCtx,int width,int height){
    int frameIndex = 0;
    int hgSizeHeight = 2*height;
    RGB *tmp = NULL;

    // tmpのメモリを確保する
    tmp = (RGB*) malloc(sizeof(RGB));

    // 上下を入れ替える
    while(frameIndex < pixCtx->numFrames){
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                (*tmp).r = pixCtx->pix[frameIndex][y][x].r;
                (*tmp).g = pixCtx->pix[frameIndex][y][x].g;
                (*tmp).b = pixCtx->pix[frameIndex][y][x].b;
                pixCtx->pix[frameIndex][y][x].r = pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].r;
                pixCtx->pix[frameIndex][y][x].g = pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].g;
                pixCtx->pix[frameIndex][y][x].b = pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].b;
                pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].r = (*tmp).r;
                pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].g = (*tmp).g;
                pixCtx->pix[frameIndex][hgSizeHeight - y - 1][x].b = (*tmp).b;
            }
        }
        if(frameIndex % 100 == 0)printf("%d\n", frameIndex);
        frameIndex++;
    }

    // tmpのメモリを解放する
    free(tmp);
}// end of ChangePixFrame()

// RGBピクセルデータをフレームごとに一次元配列を持つ構造体に代入する
void PushPixDraw(){

}// end of PushPixDraw()
