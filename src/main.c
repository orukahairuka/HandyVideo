/*
 * HandyGraphicを利用して動画編集ができるアプリケーションを作成する
 * mp4拡張子の動画ファイルに対して、再生、編集、出力ができることを目標とする
 * 2023/05/10 Kawa_09
 */

#include <stdio.h>
#include <stdlib.h>
#include <handy.h>

#include <libavutil/imgutils.h>

#include "./Input/VideoInput.h"
#include "./ImageDraw/ImageDraw.h"

#define WIDWIDTHSIZE 600
#define WIDHEIGTHSIZE 600

int main() {
    char* videoPath = NULL;
    int frameIndex = 0;

    // ピクセルデータ格納用の構造体の宣言
    PixImage pixCtx;

    // 動画のパスを受け取る
    videoPath = GetVideoPath();
    printf("%s\n", videoPath);// デバッグ用
    getchar();// デバッグ用。キー入力を待つ
    system("clear");// ターミナルを綺麗にする

    // 受け取ったパス先の動画から連続静止画を出力
    VideoToBit(&pixCtx,videoPath,WIDWIDTHSIZE,WIDHEIGTHSIZE);

    HgWOpen(500, 500, WIDWIDTHSIZE, WIDHEIGTHSIZE);

    // ビットマップをHandyGraphicで描画する
    while (pixCtx.numFrames >= frameIndex) {
        for (int y = 0; y < pixCtx.height; y++) {
            for (int x = 0; x < pixCtx.wight; x++) {
                // RGBでピクセルデータのセットをする
                // 大きさ1 1 の塗りつぶされたボックスを描画する
                // 描画の位置を少しずつ変える
            }
        }
        frameIndex++;
    }
    HgGetChar();
    HgCloseAll();

    // videoPathのメモリを解放する
    free(videoPath);

    // pixCtxのメモリを解放する
    for(int i=0; i < pixCtx.height; i++) {
        free(pixCtx.pixel[i]);
    }
    free(pixCtx.pixel);

    return 0;
}
