//--------------------------------------------------------------------------------------
// File: GameplayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Stage.h"
#include "Player.h"
#include "Monster.h"

// クラスの前方宣言
class Game;

// ゲームプレイシーン
class GameplayScene
{
	// 列挙型の定義 -----------------------------------------------------
private:

	// ポーズした理由
	enum class PauseReason
	{
		None,		// ポーズしていない
		Start,		// ゲームスタート時
		Clear,		// ゲームクリア時
		Repel,		// モンスターを撃退した時
		Dead,		// 死んだ時
		GameOver,	// ゲーム終了時
	};

// クラス定数の宣言 -------------------------------------------------
private:
	
	// キャラクターサイズ（５６ドット）
	static constexpr int CHARACTER_SIZE = 56;

	// ゲームが始まるまでの待ち時間（２秒）
	static constexpr int PAUSE_TIME_START = 2 * 60;

	// ステージクリア時の待ち時間（３秒）
	static constexpr int PAUSE_TIME_CLEAR = 3 * 60;
	
	// プレイヤーが死亡した時の待ち時間（５秒）
	static constexpr int PAUSE_TIME_DEAD = 5 * 60;
	
	// ゲームオーバー時の待ち時間（５秒）
	static constexpr int PAUSE_TIME_GAMEOVER = 5 * 60;

// データメンバの宣言 -----------------------------------------------
private:

	// このシーンを含むゲームオブジェクトへのポインタ
	Game* m_pGame;

	// ステージ
	Stage m_stage;

	// ポーズ中フラグ
	bool m_pause;

	// ポーズした理由
	PauseReason m_pauseReason;

	// ポーズタイマー
	int m_pauseTimer;

	// 遊んでいるステージ数
	int m_stageNumber;

	// グラフィックハンドル
	int m_ghPlayer;		// プレイヤー
	int m_ghMonster;	// モンスター

	// プレイヤー
	Player m_player;

	// モンスター
	Monster m_monster[static_cast<int>(Monster::Type::TypeMax)];

// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	GameplayScene(Game* pGame);

	// デストラクタ
	~GameplayScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(int keyCondition, int keyTrigger);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

private:

	// ゲームが止まった時の処理
	bool Pause();

	// プレイヤーとモンスターの接触判定関数
	bool IsHit(int monster);

	// ゲームスタート時の初期化関数
	void StartInitialize();

	// ステージクリア時の初期化関数
	void ClearInitialize();

public:

	// デバッグ情報を表示する関数
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};
