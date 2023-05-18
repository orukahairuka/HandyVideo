/*
 * HandyGraphicを利用して動画編集ができるアプリケーションを作成する
 * mp4拡張子の動画ファイルに対応しており、再生、編集、出力ができることを目標とする
 * 2023/05/10 Kawa_09
 */

#include <stdio.h>
#include <stdlib.h>
#include <handy.h>

#include "./Input/VideoInput.h"
#include "./ImageDraw/ImageDraw.h"

int main() {
    int i;
    int imageId;
    char* videoPath = NULL;

    // 動画のパスを受け取る
    videoPath = GetVideoPath();
    printf("%s\n", videoPath);// デバッグ用
    getchar();// デバッグ用。キー入力を待つ
    system("clear");// ターミナルを綺麗にする

    // 受け取ったパス先の動画から連続静止画を出力
    videoToPng(videoPath);

    int wid = HgWOpen(500, 500, 600, 600);

    // 画像をHandyGraphicで描画する
    for(i=0;;i++) {
        if ((imageId = setImage(i)) == -1) {
            printf("画像の読み込みに失敗しました\n");
            break;
        }
        HgImagePut(100, 100, imageId, 1.0, 0);
        HgImageUnload(imageId);
    }
    HgGetChar();
    HgCloseAll();

    // videoPathのメモリを解放する
    free(videoPath);

    return 0;
}
