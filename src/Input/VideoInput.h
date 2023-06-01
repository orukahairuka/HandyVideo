// RGBピクセルデータの格納用
typedef struct {
    double r;
    double g;
    double b;
}RGB;

// フレームごとの２次元のRGBピクセルデータの格納用
typedef struct {
    RGB ***pixel;        // ピクセルデータのポインタ
    int64_t numFrames;   // フレーム数
}PixFrameData;

// RGBピクセルデータの描画処理用
typedef struct {
    double x;           // ピクセルのx座標
    double y;           // ピクセルのy座標
    RGB pixColor;       // ピクセルの色
}PixDraw;

char* GetVideoPath();

void VideoToBit(PixFrameData *pixCtx,char* videoPath,int width,int height);