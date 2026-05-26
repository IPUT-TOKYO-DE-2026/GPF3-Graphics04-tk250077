#include "FrameBufferEmulator.h"


//  buff フレームバッファの先頭アドレス
//  width, height フレームバッファの高さと横幅
//  radius 円の半径
//  centerX, centerY 中心座標(X, Y)
//  color 描画色（B,G,Rの配列）
void drawFilledCircle(unsigned char* buff, int width, int height, int radius, int centerX, int centerY, unsigned char color[3])
{
	const int squaredRadius = radius * radius; // 半径の二乗
	for (int y = 0; y < height; y++) { // 縦方向のループ
		int squaredY = y - centerY;	// Y軸の中心からの距離
		squaredY *= squaredY;  // 二乗しておく
		for (int x = 0; x < width; x++) { // 横方向のループ
			int squaredX = x - centerX;	// X軸の中心からの距離
			squaredX *= squaredX;  // 二乗しておく
			if (squaredY + squaredX <= squaredRadius) { // 距離が半径以下ならば（二乗どうしで比較）
				// 現在のX,Yで示す位置は円の内側として色を置く
				*buff++ = color[0];  // B
				*buff++ = color[1];  // G
				*buff++ = color[2];  // R
			}
			else {
				buff += 3;	// 現在のX,Yで示す位置は円の外側（色は置かず次のピクセルに移る）
			}
		}
	}
}

int centerX; // 円の中心座標X
int centerY; // 円の中心座標Y
int radius;  // 円の半径
int colorindex;

// 初期化処理（最初に1回だけ呼び出される）
void FrameBufferEmulator::initUser()
{
	// フレームバッファの中心座標を求める
	centerX = width / 2;
	centerY = height / 2;
	radius = 100; // 初期の半径
	colorindex = 0;
}

// 描画処理（毎フレーム呼び出される）
void FrameBufferEmulator::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
	//リストを作成
	const int color_number = 5;

	unsigned char colors[color_number][3] = {
		{100, 60, 255 },//青
		{99, 248, 255},//空色
		{255, 0, 103},//赤？
		{255, 255, 103},//黄色
		{255, 112 ,221},//うすピンク
	};
	if (keyTrigger == SDLK_LEFT) {
		colorindex = (colorindex - 1 + color_number)% color_number;
	}
	if (keyTrigger == SDLK_RIGHT) {
		colorindex = (colorindex + 1) % color_number;

	}


	if (keyTrigger == SDLK_UP) { // 上矢印キーが押されたら
		radius++;  // 半径を大きくする
	}
	if (keyTrigger == SDLK_DOWN) {
		radius--;
	}

	unsigned char currentColor[3];

	currentColor[0] = colors[colorindex][0]; // B
	currentColor[1] = colors[colorindex][1]; // G
	currentColor[2] = colors[colorindex][2]; // R

	drawFilledCircle(buff, width, height, radius, centerX, centerY, currentColor); // 円を描画する
}
