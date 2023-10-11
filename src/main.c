/*
 * HandyGraphicを利用して動画編集ができるアプリケーションを作成する
 * mp4拡張子の動画ファイルに対して、再生、編集、出力ができることを目標とする
 * 2023/06/14 Kawa09
 */

#include <stdio.h>
#include <stdlib.h>
#include <handy.h>

#include <libavutil/imgutils.h>

#include "./Input/VideoInput.h"
#include "./Draw/PixFrameDraw.h"

#define WID_WIDTH_SIZE 600
#define WID_HEIGHT_SIZE 600

int main() {
    char* videoPath = NULL;
    int frameIndex = 0;

    // ピクセルデータ格納用の構造体
    PixFrameData pixCtx;

    // ピクセルデータの描画処理用の構造体
    PixDraw pixDraw;

    // 動画のパスを受け取る
    videoPath = GetVideoPath();
    printf("%s\n", videoPath);// デバッグ用
    getchar();// デバッグ用。キー入力を待つ
    system("clear");// ターミナルを綺麗にする

    // 受け取ったパス先の動画からRBGのピクセルデータを取得
    printf("動画の読み込み中...\n");
    VideoToBit(&pixCtx,videoPath,WID_WIDTH_SIZE,WID_HEIGHT_SIZE);

    // pixDrawのメモリを確保
    pixDraw.pixColor = (RGB**) malloc((pixCtx.numFrames) * sizeof(RGB*));
    for(int i = 0;i < pixCtx.numFrames; i++) {
        pixDraw.pixColor[i] = (RGB*) malloc((WID_HEIGHT_SIZE*WID_WIDTH_SIZE) * sizeof(RGB));
    }

    system("clear");// ターミナルを綺麗にする
    printf("動画の再生準備中...\n");

    system("clear");// ターミナルを綺麗にする
    printf("完了\n");

    HgOpen(WID_WIDTH_SIZE, WID_HEIGHT_SIZE);
    HgSetTitle("HandyVideo");

    printf("%lld\n",pixCtx.numFrames);
    // ビットマップをHandyGraphicで描画する
    while (pixCtx.numFrames >= frameIndex) {
        printf("%d\n",frameIndex+1);
        getchar();
        for (int y = 0; y < WID_HEIGHT_SIZE; y++) {
            for (int x = 0; x < WID_WIDTH_SIZE; x++) {
                // RGBでピクセルデータのセットをする
                HgSetFillColor(HgRGB(pixCtx.pix[frameIndex][WID_HEIGHT_SIZE - (y+1)][x].r,
                                 pixCtx.pix[frameIndex][y][x].g,
                                 pixCtx.pix[frameIndex][y][x].b));
                // 大きさ1 1 の塗りつぶされたボックスを描画する
                HgBoxFill(x,WID_HEIGHT_SIZE - (y+1),1,1,0);
                // 描画の位置を少しずつ変える
            }
            printf("\n");
        }
        frameIndex++;
        if(frameIndex > 100){
            break;
        }
    }
    HgGetChar();
    HgCloseAll();

    printf("終了処理をしています。そのままで待ってください\n");

    // videoPathのメモリを解放する
    free(videoPath);

    // pixDrawのメモリを解放する
    for(int i = 0; i < pixCtx.numFrames; i++) {
        free(pixDraw.pixColor[i]);
    }
    free(pixDraw.pixColor);

    // pixCtxのメモリを解放する
    for(int i = 0; i < pixCtx.numFrames; i++) {
        for(int j = 0; j < WID_HEIGHT_SIZE; j++) {
            free(pixCtx.pix[i][j]);
        }
        free(pixCtx.pix[i]);
    }
    free(pixCtx.pix);

    return 0;
}
