//--------------------------------------------------------------------------------------
// File: ResultScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class ResultScene
{
	// クラス定数の宣言 -------------------------------------------------
public:

	// 結果画面の表示時間（３秒）
	static constexpr int RESULT_DISPLAY_TIME = 3 * 60;

	// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// タイマー
	int m_timer;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	ResultScene(Game* pGame);

	// デストラクタ
	~ResultScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

};
