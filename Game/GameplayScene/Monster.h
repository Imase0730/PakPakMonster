//--------------------------------------------------------------------------------------
// File: Monster.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// クラスの前方宣言
class Game;
class Stage;

// モンスター
class Monster
{
	// 列挙型の定義 -----------------------------------------------------
public:

	enum class Type
	{
		Type1,		// 追いかけモンスター
		Type2,		// 気まぐれモンスター

		TypeMax
	};

	enum class State
	{
		Normal,		// 通常
		Weak,		// 逃げてる
		Dead,		// 食べられた
		Eat,		// プレイヤーを食べた
		Return,		// 巣に戻っている
		Revive,		// 復活
	};

	enum class AnimeNo
	{
		Normal_01,	// 通常パターン１
		Normal_02,	// 通常パターン２
		Return_01,	// 巣に戻るパターン１
		Return_02,	// 巣に戻るパターン２
		Return_03,	// 巣に戻るパターン３（目だけ）

		AnimeMax
	};

private:

	// モンスターの移動速度の種類
	enum class Speed
	{
		Level1,		// Level1（遅い）
		Level2,		// Level2   ↓
		Level3,		// Level3   ↓
		Level4,		// Level4   ↓
		Level5,		// Level5（速い）
		Weak,		// モンスターが弱っている時
		Return,		// モンスターが巣に戻る時

		SpeedMax
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// アニメーション表示時間（食べられて目になる）
	static constexpr int ANIME_INTERVEL_A[] = { 16, 16, 16, 16, 40 };

	// アニメーション表示時間（復活する）
	static constexpr int ANIME_INTERVEL_B[] = { 32, 32, 32, 32, 40 };

	// モンスターのスタート地点
	static constexpr int START_POSITION[static_cast<int>(Type::TypeMax)][2] =
	{
		{ (3 * Common::CHIP_SIZE) << 4, (2 * Common::CHIP_SIZE + 8) << 4 },	// 追いかけモンスター
		{ (3 * Common::CHIP_SIZE) << 4, (4 * Common::CHIP_SIZE - 8) << 4 },	// 気まぐれモンスター
	};

	// モンスターの移動速度
	static constexpr int SPEED_TABLE[static_cast<int>(Speed::SpeedMax)] = {12, 18, 24, 30, 36, 12, 32};

	// データメンバの宣言 -----------------------------------------------
private:

	// グラフィックハンドル
	int m_ghMonster;

	// モンスターのタイプ
	Type m_type;

	// 状態
	State m_state;

	// 位置
	int m_x, m_y;

	// 速度
	int m_vx, m_vy;

	// 進行方向
	int m_dir;

	// アニメーションNo
	AnimeNo m_animeNo;

	// アニメーション用タイマー
	int m_animeTimer;

	// 弱りタイマー
	int m_weakTimer;

	// メンバ関数の宣言 -------------------------------------------------
public:
	
	// コンストラクタ
	Monster();

	// デストラクタ
	~Monster();

	// 初期化関数
	void Initialize(int graphicHandle, Type type);

	// 更新関数
	void Update(Game* pGame, Stage* pStage, int stageNumber, int playerX, int playerY);

	// 描画関数
	void Render();

	// 位置を取得する関数
	int GetPositionX() const { return m_x; }
	int GetPositionY() const { return m_y; }

	// モンスターを弱くする関数
	void Weaken(int powerupTime);

	// モンスターが食べられたり、復活するときの絵のパターンチェンジする関数
	bool ChangeBody();

	// プレイヤーと接触時に呼ばれる関数
	void OnHit();

	// 状態を取得する関数
	State GetState() const { return m_state; }

private:

	// モンスターの移動速度を取得する関数
	int GetSpeed(Game* pGame, int stageNumber) const;

	// モンスターの速度設定関数
	void SetVelocity(Game* pGame, int stageNumber);

	// モンスターの復活地点にいるかチェックする関数
	bool CheckNestPosition() const;

	// モンスターの移動方向の取得
	int GetDirection() const;

	// 進行方向とは逆方向を取得
	int GetReverseDirection(int dir);

	// プレイヤーを追いかける
	int ChasePlayer(int moveFlag, int playerX, int playerY) const;

	// 移動できる方向の数を取得する関数
	int GetDirectionCount(int moveFlag);

};

