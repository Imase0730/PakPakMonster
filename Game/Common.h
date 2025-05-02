#pragma once

class Common
{
	// クラス定数の宣言
public:

	// マップチップのサイズ（６４ドット）
	static constexpr int CHIP_SIZE = 64;

	// 移動方向
	static constexpr int UP    = (1 << 0);
	static constexpr int DOWN  = (1 << 1);
	static constexpr int LEFT  = (1 << 2);
	static constexpr int RIGHT = (1 << 3);

	static constexpr int U = UP;
	static constexpr int D = DOWN;
	static constexpr int L = LEFT;
	static constexpr int R = RIGHT;

	static constexpr int DR = (D | R);
	static constexpr int DL = (D | L);
	static constexpr int UR = (U | R);
	static constexpr int UL = (U | L);
	static constexpr int LR = (L | R);
	static constexpr int UD = (U | D);
	static constexpr int UDR = (U | D | R);
	static constexpr int UDL = (U | D | L);
	static constexpr int ULR = (U | L | R);
	static constexpr int DLR = (D | L | R);
	static constexpr int UDLR = (U | D | L | R);

	// 残機数
	static constexpr int LEVEL1_PLAYER_CNT = 6;
	static constexpr int LEVEL2_PLAYER_CNT = 3;

	// 得点
	static constexpr int SCORE_FOOD = 20;
	static constexpr int SCORE_POWER = 100;
	static constexpr int SCORE_MONSTER = 200;
	
	// 色
	enum class Color
	{
		Black,
		Green,
		Pink,
		Orange,
		Cyan,
		Purple,
		Gray,
		Yellow,
		BlueCyan,

		ColorMax,
	};

	// 色テーブル
	static constexpr int COLOR_TABLE[static_cast<int>(Color::ColorMax)][3] =
	{
		{ 0,   0,   0 },	// 黒
		{ 193, 255,  73 },	// 緑
		{ 250, 169, 240 },	// ピンク
		{ 255, 213, 157 },	// オレンジ
		{ 178, 233, 255 },	// 水色
		{ 215, 174, 255 },	// 紫
		{ 224, 227, 196 },	// グレー
		{ 255, 209,  75 },	// 黄色
		{ 172, 255, 220 },	// ブルーシアン
	};

};
