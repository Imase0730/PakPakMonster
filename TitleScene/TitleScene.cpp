//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"
#include "Game/Common.h"

// コンストラクタ
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
	// 残機数の初期化
	if (m_pGame->GetLevel() == Game::Level::Easy)
	{
		m_pGame->SetRest(Common::LEVEL1_PLAYER_CNT);
	}
	else
	{
		m_pGame->SetRest(Common::LEVEL2_PLAYER_CNT);
	}
}

// 更新処理
void TitleScene::Update(int keyCondition, int keyTrigger)
{
	// Zキーでスタート
	if (keyCondition & PAD_INPUT_1)
	{
		// ゲームプレイシーンへ
		m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
	}
	// Xキーで難易度変更 
	if (keyTrigger & PAD_INPUT_2)
	{
		if (m_pGame->GetLevel() == Game::Level::Easy)
		{
			m_pGame->SetLevel(Game::Level::Difficult);		// 難易度（難しい）
			m_pGame->SetRest(Common::LEVEL2_PLAYER_CNT);	// 残機数
		}
		else
		{
			m_pGame->SetLevel(Game::Level::Easy);			// 難易度（簡単）
			m_pGame->SetRest(Common::LEVEL1_PLAYER_CNT);	// 残機数
		}
	}
}

// 描画処理
void TitleScene::Render()
{
	Game::Level level = m_pGame->GetLevel();

	// 結果画面の背景の描画
	m_pGame->DrawResult();

	// ゲームモードの表示
	m_pGame->DrawScore(18 * 16 + 8, 5 * 16 + 8, static_cast<int>(level) + 1, 1, Common::Color::Cyan, 8);

	// 残機数
	m_pGame->DrawScore(16 * 16 + 8, 20 * 16 + 8, m_pGame->GetRest(), 1, Common::Color::Cyan, 8);

	// ハイスコア
	m_pGame->DrawScore(8 * 16, 15 * 16 + 4, m_pGame->GetHighScore(level), 5, Common::Color::BlueCyan, 8);
}

// 終了処理
void TitleScene::Finalize()
{
}

