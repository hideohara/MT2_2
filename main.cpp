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

// 線形補間
// Vector2の掛け算の式が無い人は以下の関数を使用する
Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
	//	return t * a + (1.0f - t) * b;　と同じ内容
	Vector2 result;
	result.x = t * a.x + (1.0f - t) * b.x;
	result.y = t * a.y + (1.0f - t) * b.y;
	return result;
}


//Vector2 Lerp(const Vector2& p0, const Vector2& p1, float t) {
//	//	return t * a + (1.0f - t) * b;　と同じ内容
//	Vector2 result;
//	result.x = (1 - t) * p0.x + t * p1.x;//
//	result.y = (1 - t) * p0.y + t * p1.y;
//	return result;
//}


//Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
//	return t * a + (1.0f - t) * b;
//}



// 2次ベジェ曲線
Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	Vector2 p0p1 = Lerp(p0, p1, t);
	Vector2 p1p2 = Lerp(p1, p2, t);
	Vector2 p = Lerp(p0p1, p1p2, t);
	return p;
	//資料p10を参考に中身を埋める
	//最後のpをreturnすること。
}





// --------------------------------------------------

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	// 縦長で初期化
	Novice::Initialize("Title", 480, 960);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	struct Ball {
		Vector2 position;      // ボールの位置
		Vector2 velocity;      // ボールの速度
		Vector2 acceleration;  // ボールの加速度
		float mass;            // ボールの質量
		float radius;          // ボールの半径
		unsigned int color;    // ボールの色
	};

	// 重力加速度を入れる
	const float gravity = -9.8f;

	// 2個のオブジェクト（実体）を作成し、値を設定する
	Ball ball0 = { 160.0f,960.0f, 0.0f,5.0f, 0.0f, gravity, 1.0f, 10.0f, WHITE };//空気抵抗有り
	Ball ball1 = { 320.0f,960.0f, 0.0f,5.0f, 0.0f, gravity, 1.0f, 10.0f, RED };    //空気抵抗なし

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 加速度は1秒あたりどの程度速度を変化させるかなので、
		// 60fps前提であれば、1/60秒分速度を変化させる必要がある
		ball1.velocity.y += ball1.acceleration.y / 60.0f;

		// 速度は1秒あたりどの程度位置を変化させるかなので、
		// 60fps前提であれば、1/60秒分位置を変化させる必要がある
		ball1.position.y += ball1.velocity.y / 60.0f;


		// kを定義
		float k = 0.2f;

		// 空気抵抗airResistanceは、速度に比例して逆方向に発生する
		Vector2 airResistance = {
		k * -ball0.velocity.x, k * -ball0.velocity.y };
		// 加速度とはa=F/mであるから、空気抵抗による加速度は
		Vector2 airResistanceAcceleration = { airResistance.x / ball0.mass, airResistance.y / ball0.mass };

		// まず現時点での加速度を求める
		ball0.acceleration.y = gravity + airResistanceAcceleration.y;
		// 加速度は1秒あたりどの程度速度を変化させるかなので、60fps前提であれば、1/60秒分速度を変化させる必要がある
		ball0.velocity.y += (ball0.acceleration.y / 60.0f);
		// 速度は1秒あたりどの程度位置を変化させるかなので、60fps前提であれば、1/60秒分位置を変化させる必要がある
		ball0.position.y += (ball0.velocity.y / 60.0f);



		///
		/// ↓更新処理ここから
		///

		//1.矩形のworldMatrixを作製
		//Matrix3x3 worldMatrix = MakeAffineMatrix(scale, theta, rectCenter);
		//Matrix3x3 worldMatrix = MakeTranslateMatrix(rectCenter);

		//2bカメラのWorldMatrixを作成
		//作成方法はworldMatrixとほぼ同じだが、positionだけ違うことに注意
		Matrix3x3 cameraMatrix = MakeAffineMatrix(Vector2{ 1.0f, 1.0f }, 0.0f, Vector2{ 240.0f, 480.0f });

		//カメラのworldMatrixの逆行列(ビュー行列)を作成
		Matrix3x3 viewMatrix = Inverse(cameraMatrix);

		//3.確認課題資料p2の手順3の通りに引数を入れる
		Matrix3x3 orthoMatrix = MakeOrthographicMatrix(
			-240.0f, 480.0f, 240.0f, -480.0f);

		//4.確認課題資料p2の手順4の通りに引数を入れる
		Matrix3x3 viewportMatrix = MakeViewportMatrix(0, 0, 480.0f, 960.0f);


		//5.ワールドからビューポート行列までの行列全てを合成する
		Matrix3x3 wvpVpMatrix = Multiply(viewMatrix, orthoMatrix);
		wvpVpMatrix = Multiply(wvpVpMatrix, viewportMatrix);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 2つの円の位置を変換
		Vector2 position0 = Transform(ball0.position, wvpVpMatrix);
		Vector2 position1 = Transform(ball1.position, wvpVpMatrix);

		// 2つの円を描画
		Novice::DrawEllipse(int(position0.x), int(position0.y), 
			int(ball0.radius), int(ball0.radius), 0, ball0.color, kFillModeSolid);
		Novice::DrawEllipse(int(position1.x), int(position1.y), 
			int(ball1.radius), int(ball1.radius), 0, ball1.color, kFillModeSolid);



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
