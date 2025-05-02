//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// クラスの前方宣言
class Game;
class Stage;
class Monster;

// プレイヤー
class Player
{
	// 列挙型の定義 -----------------------------------------------------
public:

	// プレイヤーの状態
	enum class State
	{
		Normal,	// 通常
		Dead,	// 死んだ
		Stop,	// 停止
	};

private:

	// アニメーション番号
	enum class AnimeNo
	{
		Normal_01,	// 通常パターン１
		Normal_02,	// 通常パターン２
		Dead_01,	// 死亡パターン１
		Dead_02,	// 死亡パターン２
		None,		// 表示なし

		AnimeMax
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// プレイヤーのスタート地点
	static constexpr int START_POSITION[2] = { (3 * Common::CHIP_SIZE) << 4, (5 * Common::CHIP_SIZE) << 4 };

	// プレイヤーの速さ
	static constexpr int PLAYER_SPEED = (2 << 4);

	// パワーアップの残り時間
	static constexpr int POWERUP_TIME = 5 * 60;

	// データメンバの宣言 -----------------------------------------------
private:

	// グラフィックハンドル
	int m_ghPlayer;

	// 状態
	State m_state;

	// 位置
	int m_x, m_y;

	// 速度
	int m_vx, m_vy;

	// 進行方向
	int m_dir;

	// true場合は左向き
	bool m_leftFlag;

	// 食べたえさの場所
	int m_foodX, m_foodY;

	// パワーアップの残り時間
	int m_powerTimer;

	// アニメーションNo
	AnimeNo m_animeNo;

	// アニメーション用タイマー
	int m_animeTimer;

	// モンスターを撃退した時のスコア
	int m_repelScore;

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化関数
	void Initialize(int graphicHandle);

	// 更新関数
	void Update(Game* pGame, Stage* pStage, int key, Monster* monster);

	// 描画関数
	void Render();

	// プレイヤーの状態を設定する関数
	void SetState(Player::State state) { m_state = state; }

	// プレイヤーの位置を取得する関数
	int GetPositionX() const { return m_x; }
	int GetPositionY() const { return m_y; }

	// プレイヤーが食べられた時呼ばれる関数
	void Dead();

	// モンスターを撃退した時の得点を加算する関数
	void AddRepelScore(Game* pGame);

private:

	// 速度を設定する関数
	void SetVelocity();

public:

	// デバッグ情報を表示する関数
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};

