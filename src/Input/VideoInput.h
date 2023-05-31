// RGBピクセルデータの格納用
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}RGB;

// ２次元のRGBピクセルデータの格納用
typedef struct {
    RGB ***pixel;    // ピクセルデータのポインタ
    int wight;      // ピクセルデータの幅
    int height;     // ピクセルデータの高さ
    int64_t numFrames;   // フレーム数
}PixImage;

char* GetVideoPath();

void VideoToBit(PixImage *pixCtx,char* videoPath,int width,int height);