/*
 * 動画ファイルの受け取り関係の処理をする
 * 2023/05/10 Kawa_09
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "VideoInput.h"

#define BUFSIZE 256

// 動画のパスをターミナルから受け取る（仮）
char* GetVideoPath() {
    char* input = NULL;
    char* videoPath = NULL;


    // 文字列の保存用にメモリを確保
    input = (char*)malloc(sizeof(char) * BUFSIZE);
    videoPath = (char*)malloc(sizeof(char) * BUFSIZE);


    // メモリの確保に失敗したら終了
    if( input == NULL) {
        printf("input：文字列配列の作成に失敗しました\n");
    }else {
        printf("input：文字列配列の作成に成功しました\n");
    }
    if( videoPath == NULL) {
        printf("videoPath：文字列配列の作成に失敗しました\n");
    }else {
        printf("videoPath：文字列配列の作成に成功しました\n");
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

// ffmpegを使用して動画から連続静止画を出力する
void videoToPng(char* videoPath) {
    char* buf=NULL;

    // メモリを確保する
    buf = (char*)malloc(sizeof(char) * BUFSIZE);

    // 静止画出力用のフォルダを作成する
    system("mkdir output_png");

    // ffmpegのコマンドを実行できるよう、bufにコマンドを書き込む
    sprintf(buf, "ffmpeg -i %s -vcodec png ./../output_png//output_%%06d.png", videoPath);

    // 書き込まれたコマンドをsystemから実行する
    system(buf);

    // bufのメモリを解放する
    free(buf);
}

