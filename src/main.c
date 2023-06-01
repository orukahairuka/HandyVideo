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
    PixFrameData pixCtx;

    // 動画のパスを受け取る
    videoPath = GetVideoPath();
    printf("%s\n", videoPath);// デバッグ用
    getchar();// デバッグ用。キー入力を待つ
    system("clear");// ターミナルを綺麗にする

    // 受け取ったパス先の動画から連続静止画を出力
    printf("動画の読み込み中\n");
    VideoToBit(&pixCtx,videoPath,WIDWIDTHSIZE,WIDHEIGTHSIZE);

    system("clear");// ターミナルを綺麗にする
    printf("完了\n");

    HgOpen(WIDWIDTHSIZE, WIDHEIGTHSIZE);
    HgSetTitle("HandyVideo");

    printf("%lld\n",pixCtx.numFrames);
    // ビットマップをHandyGraphicで描画する
    while (pixCtx.numFrames >= frameIndex) {
        printf("%d\n",frameIndex+1);
        getchar();
        for (int y = 0; y < WIDHEIGTHSIZE; y++) {
            for (int x = 0; x < WIDWIDTHSIZE; x++) {
                // RGBでピクセルデータのセットをする
                HgSetFillColor(HgRGB(pixCtx.pixel[frameIndex][y][x].r,
                                 pixCtx.pixel[frameIndex][y][x].g,
                                 pixCtx.pixel[frameIndex][y][x].b));
                // 大きさ1 1 の塗りつぶされたボックスを描画する
                printf("x:%d y:%d\n",x,y);
                HgBoxFill(x,y,1,1,0);
                // 描画の位置を少しずつ変える
                printf("R:%f G:%f B:%f \n",pixCtx.pixel[frameIndex][y][x].r,pixCtx.pixel[frameIndex][y][x].g,pixCtx.pixel[frameIndex][y][x].b);
            }
            printf("\n");
        }
        frameIndex++;
        if(frameIndex > 0){
            break;
        }
    }
    HgGetChar();
    HgCloseAll();

    // videoPathのメモリを解放する
    free(videoPath);

    // pixCtxのメモリを解放する
    for (int i = 0; i < pixCtx.numFrames; i++) {
        for (int j = 0; j < WIDHEIGTHSIZE; j++) {
            free(pixCtx.pixel[i][j]);
        }
        free(pixCtx.pixel[i]);
    }
    free(pixCtx.pixel);

    return 0;
}
