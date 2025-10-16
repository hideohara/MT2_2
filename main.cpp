#include <Novice.h>
#include <Math.h>

const char kWindowTitle[] = "GC1C_99_オオハラ_ヒデフミ";


// 2x2の行列を表す
struct Matrix2x2
{
	float m[2][2];
};
// 2次元ベクトルを表す
struct Vector2
{
	float x;
	float y;
};

static const int kRowHeight = 20;
static const int kColumnWidth = 64;  // 44

// 行列の表示
void MatrixScreenPrintf(int x, int y, Matrix2x2 matrix) {
	for (int row = 0; row < 2; ++row) {
		for (int column = 0; column < 2; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + row * kRowHeight, "%.02f", matrix.m[row][column]);
		}
	}
}

// ベクトルの表示
void VectorScreenPrintf(int x, int y, Vector2 vector) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
}

// 1. 行列の加法
Matrix2x2 Add(Matrix2x2 matrix1, Matrix2x2 matrix2)
{
	Matrix2x2 result = {};

	result.m[0][0] = matrix1.m[0][0] + matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] + matrix2.m[0][1];
	result.m[1][0] = matrix1.m[1][0] + matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] + matrix2.m[1][1];

	return result;
}


// 2. 行列の減法
Matrix2x2 Subtract(Matrix2x2 matrix1, Matrix2x2 matrix2)
{
	Matrix2x2 result = {};

	result.m[0][0] = matrix1.m[0][0] - matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] - matrix2.m[0][1];
	result.m[1][0] = matrix1.m[1][0] - matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] - matrix2.m[1][1];

	return result;
}

// 3. 行列の積
Matrix2x2 Multiply(Matrix2x2 matrix1, Matrix2x2 matrix2)
{
	Matrix2x2 result = {};

	result.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0];
	result.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1];
	result.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1];

	return result;
}

// 4. ベクトルと行列の積
Vector2 Multiply(Vector2 vector, Matrix2x2 matrix)
{
	Vector2 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1];

	return result;
}

// 回転行列の作成関数
Matrix2x2 MakeRotateMatrix(float theta)
{
	Matrix2x2 result = {};

	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);

	return result;
}

// スケール（拡縮）行列の作成関数
//Matrix2x2 MakeScaleMatrix(Vector2 scale) {
//	Matrix2x2 result;
//
//	result.m[0][0] = scale.x;
//	result.m[0][1] = 0;
//	result.m[1][0] = 0;
//	result.m[1][1] = scale.y;
//
//	return result;
//}

// スクリーン座標系へ変換する関数 (前期に作成済)
Vector2 ToScreen(const Vector2* world) {
	// 今回のワールド座標系からスクリーン座標系は
	// 原点位置がyに500ずれていて、y軸が反転
	const Vector2 kWorldToScreenTranslate = { 0.0f, 500.f };
	const Vector2 kWorldToScreenScale = { 1.0f, -1.0f };
	return {
	  (world->x * kWorldToScreenScale.x) + kWorldToScreenTranslate.x,
	  (world->y * kWorldToScreenScale.y) + kWorldToScreenTranslate.y };
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Matrix2x2 m1;
	m1.m[0][0] = 1.0f;
	m1.m[0][1] = 2.0f;
	m1.m[1][0] = 3.0f;
	m1.m[1][1] = 4.0f;
	Matrix2x2 m2;
	m2.m[0][0] = 5.0f;
	m2.m[0][1] = 6.0f;
	m2.m[1][0] = 7.0f;
	m2.m[1][1] = 8.0f;
	Vector2 v = { 10, 20 };

	// 中心の座標
	Vector2 rectCenter = { 400, 100 };

	// サイズ
	Vector2 rectSize = { 200, 100 };


	// 角度の変数
	float theta = 0.0f;

	// テクスチャーの読み込み
	int textureHandle = Novice::LoadTexture("white1x1.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix2x2 resultAdd = Add(m1, m2);
		Matrix2x2 resultSubtract = Subtract(m1, m2);
		Matrix2x2 resultMultiply = Multiply(m1, m2);
		Vector2 resultVector = Multiply(v, m1);

		// 矩形(四角形)の4頂点の作成
		Vector2 leftTop = { -rectSize.x / 2, rectSize.y / 2 };     // 左上
		Vector2 rightTop = { rectSize.x / 2, rectSize.y / 2 };     	// 右上
		Vector2 leftBottom = { -rectSize.x / 2, -rectSize.y / 2 }; 	// 左下
		Vector2 rightBottom = { rectSize.x / 2, -rectSize.y / 2 }; 	// 右下

		// 角度を増やす
		theta += 1.0f/60.0f;

		// 回転行列の作成
		Matrix2x2 rotateMatrix = MakeRotateMatrix(theta);

		// 矩形(四角形)の回転
		leftTop = Multiply(leftTop, rotateMatrix);
		rightTop = Multiply(rightTop, rotateMatrix);
		leftBottom = Multiply(leftBottom, rotateMatrix);
		rightBottom = Multiply(rightBottom, rotateMatrix);

		// 矩形(四角形)の平行移動
		leftTop.x += rectCenter.x;
		leftTop.y += rectCenter.y;
		rightTop.x += rectCenter.x;
		rightTop.y += rectCenter.y;
		leftBottom.x += rectCenter.x;
		leftBottom.y += rectCenter.y;
		rightBottom.x += rectCenter.x;
		rightBottom.y += rectCenter.y;

		// 矩形(四角形)をスクリーン座標へ変換
		leftTop = ToScreen(&leftTop);
		rightTop = ToScreen(&rightTop);
		leftBottom = ToScreen(&leftBottom);
		rightBottom = ToScreen(&rightBottom);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, kRowHeight * 0, resultAdd);
		MatrixScreenPrintf(0, kRowHeight * 2 + 10, resultSubtract);
		MatrixScreenPrintf(0, kRowHeight * 4 + 20, resultMultiply);
		VectorScreenPrintf(0, kRowHeight * 6 + 30, resultVector);

		// 矩形(四角形)を描画
		Novice::DrawQuad(
			int(leftTop.x), int(leftTop.y),
			int(rightTop.x), int(rightTop.y),
			int(leftBottom.x), int(leftBottom.y),
			int(rightBottom.x), int(rightBottom.y),
			0, 0, 1, 1, textureHandle, WHITE);

		// 矩形(四角形)を描画
		//Vector2 scale{ 2.0f, 4.0f };
		//Matrix2x2 scaleMatrix = MakeScaleMatrix(scale);
		//MatrixScreenPrintf(0, 0, scaleMatrix);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
