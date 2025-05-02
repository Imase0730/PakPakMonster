//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// クラスの前方宣言
class Game;

class Stage
{
	// 列挙型の宣言 -------------------------------------------------
public:

	// えさの状態
	enum class FoodState
	{
		None,
		Food_3,
		Food_2,
		Food_1,
		Power_3,
		Power_2,
		Power_1,

		FoodStateMax,
	};

	// ボーナスの状態
	enum class BonusState
	{
		Wait,		// 表示待ち
		Display,	// 表示中
		Score,		// 点数表示中
	};

	// 構造体の宣言 -------------------------------------------------
public:

	// えさの構造体
	struct FoodInformation
	{
		FoodState state;	// えさの状態 
		int x, y;			// えさの位置
	};

	// ボーナスえさの構造体
	struct BonusInformation
	{
		BonusState state;	// 状態
		int x, y;			// 位置
		int score;			// 得点
		int timer;			// タイマー
	};

	// クラス定数の宣言 -------------------------------------------------
public:

	// マップサイズ
	static constexpr int STAGE_SIZE = 7;

	// 移動可能マップ
	static constexpr int MOVE_INFOMATION[STAGE_SIZE][STAGE_SIZE] =
	{
		{ Common::DR,  Common::LR,  Common::DL,           0,  Common::DR,  Common::LR, Common::DL },
		{ Common::UD,           0, Common::UDR,  Common::LR, Common::UDL,           0, Common::UD },
		{ Common::UR, Common::DLR,  Common::UL,  Common::UD,  Common::UR, Common::DLR, Common::UL },
		{          0,  Common::UD,           0,  Common::UD,           0,  Common::UD,          0 },
		{ Common::DR, Common::ULR,  Common::DL,  Common::UD,  Common::DR, Common::ULR, Common::DL },
		{ Common::UD,           0, Common::UDR,  Common::LR, Common::UDL,           0, Common::UD },
		{ Common::UR,  Common::LR,  Common::UL,           0,  Common::UR,  Common::LR, Common::UL },
	};

	// モンスター用移動＆戻り用マップ
	static constexpr int RETURN_INFOMATION[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 0,         0,         0,         0,         0,         0, 0 },
		{ 0,         0, Common::R, Common::D, Common::L,         0, 0 },
		{ 0, Common::R,         0,         0,         0, Common::L, 0 },
		{ 0,         0,         0,         0,         0,         0, 0 },
		{ 0, Common::U,         0,         0,         0, Common::U, 0 },
		{ 0,         0, Common::U,         0, Common::U,         0, 0 },
		{ 0,         0,         0,         0,         0,         0, 0 },
	};

private:

	// えさの配置１
	static constexpr int FOOD_POSITION_1[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 2, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 1 },
	};

	// えさの配置２
	static constexpr int FOOD_POSITION_2[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 1, 0, 0, 0, 0, 0, 2 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 1 }
	};

	// えさの数
	static constexpr int FOOD_MAX = 8;

	// 始めのボーナス出現時間
	static constexpr int BONUS_APPEAR_TIME_FIRST = 9;

	// ２回目以降のボーナス出現時間
	static constexpr int BONUS_APPEAR_TIME_NEXT = 15;

	// ボーナスえさ表示時間
	static constexpr int BONUS_DISPLAY_TIME = 5 * 60;

	// ボーナスの得点表示時間
	static constexpr int BONUS_SCORE_DISPLAY_TIME = 4 * 60;

	// ボーナスえさの得点
	static constexpr int BONUS_SCORE[] = { 100, 200, 300, 400, 500, 600, 700, 800 };

	// えさの色
	static constexpr Common::Color FOOD_COLOR[static_cast<int>(FoodState::FoodStateMax)] =
	{
		Common::Color::Black,
		Common::Color::Green,
		Common::Color::Pink,
		Common::Color::Orange,
		Common::Color::Green,
		Common::Color::Pink,
		Common::Color::Orange,
	};

	// ステージの色（簡単 0～2 難しい 1～3 を使用）
	static constexpr Common::Color STAGE_COLOR[] =
	{
		Common::Color::BlueCyan, Common::Color::Green, Common::Color::Cyan, Common::Color::Black
	};

	// データメンバの宣言 -----------------------------------------------
private:

	// グラフィックハンドル
	int m_ghBg01;
	int m_ghBg02;
	int m_ghBg03;
	int m_ghFood;

	// ボーナスの表示状態
	BonusInformation m_bonusInformation;

	// ステージ上に配置されているえさの情報
	FoodInformation m_foodInformation[FOOD_MAX];

	// ゲーム中に使われるマップデータへのポインタ
	const int (*m_foodPosition)[STAGE_SIZE];

	// メンバ関数の宣言 -------------------------------------------------
public:

	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Render(Game* pGame, int stage) const;

	// えさの初期化
	void FoodInitiaize();
	
	// 指定位置にえさがあるか調べる関数
	FoodInformation* GetFoodInformation(int x, int y);
	
	// ボーナスの情報を取得する関数
	BonusInformation* GetBonusInformation() { return &m_bonusInformation; }

	// ボーナスと接触したか調べる関数
	bool IsHitBonus(int mapX, int mapY) const;

	// ボーナスを取得した時呼ぶ関数
	void GetBonus(Game* pGame);
	
	// ステージ上のえさの数を取得する関数
	int GetFoodCnt() const;

	// ステージを設定する関数（パワーアップえさの場所が違う）
	void SetStage();

private:

	// ボーナス設定関数
	void SetBonus();

public:

	// デバッグ情報を表示する関数
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};

