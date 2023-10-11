/*
 * ピクセルデータを描画する
 * 2023/06/14 Kawa_09
 */

#include <stdio.h>
#include <handy.h>

#include "PixFrameDraw.h"

int SetImage(int imageNumber) {
    char str[256];
    sprintf(str,"./../output_png/output_%06d.png", imageNumber);
    printf("%s\n", str);
    return HgImageLoad(str);
}

//void putImage(wid,x,y,gid,scale,a) {
//    if(wid == ) {
//        HgImagePut();
//    }else {
//        HgWImagePut();
//    }
//    HgImagePut(x,y,gid,scale,a);
//}