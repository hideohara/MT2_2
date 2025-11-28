#include <Novice.h>
#include <Math.h>
#include <assert.h>

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

// 3x3の行列を表す
struct Matrix3x3 {
	float m[3][3];
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

// 行列の表示 3x3
void MatrixScreenPrintf(int x, int y, Matrix3x3 matrix) {
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
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

// 3. 行列の積（3x3での積）
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2)
{
	Matrix3x3 result = {};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
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
Matrix3x3 MakeRotateMatrix(float theta)
{
	Matrix3x3 result = {};
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[0][2] = 0.0f;

	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[1][2] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;

	return result;
}

// スケール（拡縮）行列の作成関数
Matrix3x3 MakeScaleMatrix(Vector2 scale)
{
	Matrix3x3 result = {};
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;

	return result;
}




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

Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1.0f;
	return result;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

// ------------------------------------


//Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate) {
//	float cosine = cosf(rotate);
//	float sine = sinf(rotate);
//	Matrix3x3 result;
//	result.m[0][0] = scale.x * cosine;
//	result.m[0][1] = scale.x * sine;
//	result.m[0][2] = 0.0f;
//	result.m[1][0] = -scale.y * sine;
//	result.m[1][1] = scale.y * cosine;
//	result.m[1][2] = 0.0f;
//	result.m[2][0] = translate.x;
//	result.m[2][1] = translate.y;
//	result.m[2][2] = 1.0f;
//	return result;
//}

// アフィン変換
Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate)
{
	Matrix3x3 result = {};
	
	Matrix3x3 scaleMatrix = MakeScaleMatrix(scale);
	Matrix3x3 rotateMatrix = MakeRotateMatrix(rotate);
	Matrix3x3 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return result;
}


// 2x2逆行列を求める
Matrix2x2 Inverse(Matrix2x2 matrix)
{
	Matrix2x2 result = {};
	float determinant = matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0];

	assert(determinant != 0);

	float determinantRecp = 1.0f / determinant;

	result.m[0][0] = matrix.m[1][1] * determinantRecp;
	result.m[0][1] = -matrix.m[0][1] * determinantRecp;
	result.m[1][0] = -matrix.m[1][0] * determinantRecp;
	result.m[1][1] = matrix.m[0][0] * determinantRecp;

	return result;
}

// 3x3逆行列を求める
Matrix3x3 Inverse(Matrix3x3 matrix)
{
	Matrix3x3 result = {};
	float determinant = matrix.m[0][0] * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) - matrix.m[0][1] * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) + matrix.m[0][2] * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);

	assert(determinant != 0);

	float determinantRecp = 1.0f / determinant;

	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) * determinantRecp;
	result.m[0][1] = -(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) * determinantRecp;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]) * determinantRecp;

	result.m[1][0] = -(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) * determinantRecp;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]) * determinantRecp;
	result.m[1][2] = -(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) * determinantRecp;

	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]) * determinantRecp;
	result.m[2][1] = -(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) * determinantRecp;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]) * determinantRecp;

	return result;
}

// 2x2転置行列を求める
Matrix2x2 Transpose(Matrix2x2 matrix)
{
	Matrix2x2 m1{};

	m1.m[0][0] = matrix.m[0][0];
	m1.m[0][1] = matrix.m[1][0];
	m1.m[1][0] = matrix.m[0][1];
	m1.m[1][1] = matrix.m[1][1];

	return m1;
}

// 3x3転置行列を求める
Matrix3x3 Transpose(Matrix3x3 matrix)
{
	Matrix3x3 m2{};

	m2.m[0][0] = matrix.m[0][0];
	m2.m[0][1] = matrix.m[1][0];
	m2.m[0][2] = matrix.m[2][0];
	m2.m[1][0] = matrix.m[0][1];
	m2.m[1][2] = matrix.m[2][1];
	m2.m[1][1] = matrix.m[1][1];
	m2.m[2][0] = matrix.m[0][2];
	m2.m[2][1] = matrix.m[1][2];
	m2.m[2][2] = matrix.m[2][2];

	return m2;
}

//正射影行列
//受け取った左上から右下までの範囲を-1～1までの範囲(正規化デバイス座標系)に変換する行列
Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom) {
	Matrix3x3 result{};
	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[2][0] = (left + right) / (left - right);
	result.m[2][1] = (top + bottom) / (bottom - top);
	result.m[2][2] = 1;
	//資料p12を参考に中身を埋める
	//分母、分子の中で計算がある場合は()で囲うの忘れずに
	return result;
}

//ビューポート行列
//左上の開始位置と幅、高さを使用してスクリーン座標系でのどの位置を左上としたどこまでの範囲に描画するかを決める
Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height) {

	//資料p16を参考に中身を埋める
	Matrix3x3 result{};
	result.m[0][0] = width / 2;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = -(height / 2);
	result.m[1][2] = 0;
	result.m[2][0] = left + (width / 2);
	result.m[2][1] = top + (height / 2);
	result.m[2][2] = 1;
	return result;
}

// --------------------------------------------------

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

	//Matrix2x2 m2;
	//m2.m[0][0] = 5.0f;
	//m2.m[0][1] = 6.0f;
	//m2.m[1][0] = 7.0f;
	//m2.m[1][1] = 8.0f;

	Matrix3x3 m2;
	m2.m[0][0] = 1.0f; m2.m[0][1] = 2.0f; m2.m[0][2] = 0.0f;
	m2.m[1][0] = 3.0f; m2.m[1][1] = 4.0f; m2.m[1][2] = 0.0f;
	m2.m[2][0] = 5.0f; m2.m[2][1] = 6.0f; m2.m[2][2] = 1.0f;


	Vector2 v = { 10, 20 };

	// 中心の座標
	//Vector2 rectCenter = { 50, 50 };

	// サイズ
	//Vector2 rectSize = { 100, 100 };

	struct Box {
		Vector2 position;   	// 箱の位置。中心
		Vector2 size;       	// 箱のサイズ
		Vector2 velocity;   	// 箱の速度
		Vector2 acceleration;  // 箱の加速度
		float mass; 		// 箱の質量
		unsigned int color; 	// 箱の色
	};
	
	Box box{
		{50,50},
		{100,100},
		{},
		{},
		1.0f,
		WHITE

	};


	// テクスチャーの読み込み
	int textureHandle = Novice::LoadTexture("white1x1.png");


	// スケール
	Vector2 scale{ 1.0f, 1.0f };

	// カメラのワールド座標を入れる
	Vector2 cameraPosition = { 200,200 };

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

		// 矩形(四角形)の4頂点の作成
		Vector2 leftTop = { -box.size.x / 2, box.size.y / 2 };     // 左上
		Vector2 rightTop = { box.size.x / 2, box.size.y / 2 };     	// 右上
		Vector2 leftBottom = { -box.size.x / 2, -box.size.y / 2 }; 	// 左下
		Vector2 rightBottom = { box.size.x / 2, -box.size.y / 2 }; 	// 右下


		// スペースを押したら移動開始
		if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {
			box.velocity.x = 70.0f;
		}



		// 動いているかどうか
		// != 0.0fでも良いが、動いていないとみなす閾値があると良い
		// 移動量に加速度を加算
		if (fabs(box.velocity.x) > 0.01f || fabs(box.velocity.y) > 0.01f) {
			box.velocity.x += box.acceleration.x / 60.0f;;
			box.velocity.x += box.acceleration.x / 60.0f;;

			// 位置に移動量を加算
			box.position.x += box.velocity.x / 60.0f;;
			box.position.y += box.velocity.y / 60.0f;;
		}

		//1.矩形のworldMatrixを作製
		//Matrix3x3 worldMatrix = MakeAffineMatrix(scale, theta, rectCenter);
		Matrix3x3 worldMatrix = MakeTranslateMatrix(box.position);

		//2bカメラのWorldMatrixを作成
		//作成方法はworldMatrixとほぼ同じだが、positionだけ違うことに注意
		Matrix3x3 cameraMatrix = MakeTranslateMatrix(cameraPosition);

		//カメラのworldMatrixの逆行列(ビュー行列)を作成
		Matrix3x3 viewMatrix = Inverse(cameraMatrix);

		//3.確認課題資料p2の手順3の通りに引数を入れる
		Matrix3x3 orthoMatrix = MakeOrthographicMatrix(-640, 360, 640, -360);

		//4.確認課題資料p2の手順4の通りに引数を入れる
		Matrix3x3 viewportMatrix = MakeViewportMatrix(0, 0, 1280, 720);


		//5.ワールドからビューポート行列までの行列全てを合成する
		Matrix3x3 wvpVpMatrix = Multiply(worldMatrix, viewMatrix);
		wvpVpMatrix = Multiply(wvpVpMatrix, orthoMatrix);
		wvpVpMatrix = Multiply(wvpVpMatrix, viewportMatrix);

		//各ローカル頂点とwvpVpMatrixをTransformする
		leftTop = Transform(leftTop, wvpVpMatrix);
		rightTop = Transform(rightTop, wvpVpMatrix);
		leftBottom = Transform(leftBottom, wvpVpMatrix);
		rightBottom = Transform(rightBottom, wvpVpMatrix);




		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//MatrixScreenPrintf(0, kRowHeight * 0, resultAdd);
		//MatrixScreenPrintf(0, kRowHeight * 2 + 10, resultSubtract);
		//MatrixScreenPrintf(0, kRowHeight * 4 + 20, resultMultiply);
		//VectorScreenPrintf(0, kRowHeight * 6 + 30, resultVector);

		// 矩形(四角形)を描画
		Novice::DrawQuad(
			int(leftTop.x), int(leftTop.y),
			int(rightTop.x), int(rightTop.y),
			int(leftBottom.x), int(leftBottom.y),
			int(rightBottom.x), int(rightBottom.y),
			0, 0, 1, 1, textureHandle, box.color);

		Novice::DrawLine(0, 600, 1280, 600, RED);
		Novice::DrawLine(400, 0, 400, 720, GREEN);

		//Novice::ScreenPrintf(0, 500, "x=%f  y=%f", rectCenter.x, rectCenter.y);
		//Novice::ScreenPrintf(0, 520, "x=%f  y=%f", leftTop.x, leftTop.y);


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
