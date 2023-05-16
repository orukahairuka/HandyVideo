/*
 * 連続静止画を読み込む・出力する
 * 2023/05/11 Kawa_09
 */

#include <stdio.h>
#include <handy.h>

#include "ImageDraw.h"

int setImage(int imageNumber) {
    char str[256];
    sprintf(str,"./../output_png/output_%06d.png", imageNumber);
    printf("%s\n", str);
    return HgImageLoad(str);
}

void putImage(wid,x,y,gid,scale,a) {
    if(wid == ) {
        HgImagePut();
    }else {
        HgWImagePut();
    }
    HgImagePut(x,y,gid,scale,a);
}