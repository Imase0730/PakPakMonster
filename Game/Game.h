//--------------------------------------------------------------------------------------
// File: Game.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Common.h"
#include "TitleScene/TitleScene.h"
#include "GameplayScene/GameplayScene.h"
#include "ResultScene/ResultScene.h"

// ゲーム
class Game
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// シーンID
	enum class SceneID
	{
		None = -1,
		Title,		// タイトル
		GamePlay,	// ゲームプレイ中
		Result,		// 結果画面
	};

	// 難易度
	enum class Level
	{
		Easy,		// 簡単
		Difficult,	// 難しい

		LevelMax,
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// ゲームタイトル
	static constexpr const wchar_t* TITLE = L"パクパクモンスター";

	// 文字のサイズ
	static constexpr int FONT_SIZE = 20;

	// データメンバの宣言 -----------------------------------------------
private:

	// フレームタイマー
	GameLib::FrameTimer m_frameTimer;

	// キー情報
	int m_key;
	int m_oldKey;

	SceneID m_currentSceneID;	// 現在のシーンID
	SceneID m_requestedSceneID;	// 変更要求のシーンID

	// シーンオブジェクト
	TitleScene m_titleScene;		// タイトルシーン
	GameplayScene m_gameplayScene;	// ゲームプレイシーン
	ResultScene m_resultScene;		// 結果画面シーン

	// 描画先のグラフィックハンドル
	int m_ghScreen;

	// 背景色
	Common::Color m_bgColor;


	// シーン共通で使用する変数の宣言 -----------------------------------------------
private:

	// 難易度
	Level m_level;

	// ハイスコア
	int m_highScore[static_cast<int>(Level::LevelMax)];

	// 得点
	int m_score;

	// 残機数
	int m_rest;
	
	// グラフィックハンドル
	int m_ghNumber;			// 数字
	int m_ghResult;			// 背景

	// アクセサの宣言 -------------------------------------------------
public:

	// 背景色
	void SetBgColor(Common::Color color) { m_bgColor = color; }

	// 難易度
	void SetLevel(Level level) { m_level = level; }
	Level GetLevel() const { return m_level; }

	// ハイスコア
	void SetHighScore(Level level, int score)
	{
		m_highScore[static_cast<int>(level)] = score;
	}
	int GetHighScore(Level level) const
	{
		return m_highScore[static_cast<int>(level)];
	}

	// 得点
	void SetScore(int score) { m_score = score; }
	int GetScore() const { return m_score; }
	void AddScore(int score){ m_score += score;	}

	// 残機数
	void SetRest(int rest) { m_rest = rest; }
	int GetRest() const { return m_rest; }
	void SubtractRest() { m_rest--; }

	// ハイスコアの更新関数
	void UpdateHighScore()
	{
		if (GetHighScore(m_level) < m_score) SetHighScore(m_level, m_score);
	}

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Game();

	// デストラクタ
	~Game();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

	// シーン変更の要求
	void RequestSceneChange(SceneID nextSceneID);

	// 結果画面の背景の描画
	void DrawResult() const;

	// スコア表示関数
	void DrawScore(int x, int y, int score, int keta, Common::Color color, int dis) const;

private:

	// 開始シーンの設定
	void SetStartScene(SceneID startSceneID);

	// シーンの変更
	void ChangeScene();

	// 現在のシーンの初期化処理
	void InitializeCurrentScene();

	// 現在のシーンの更新処理
	void UpdateCurrentScene(int keyCondition, int keyTrigger);

	// 現在のシーンの描画処理
	void RenderCurrentScene();

	// 現在のシーンの終了処理
	void FinalizeCurrentScene();

public:

	// デバッグ情報を表示する関数
	void DisplayDebugInformation(int offetX, int offsetY) const;

};
