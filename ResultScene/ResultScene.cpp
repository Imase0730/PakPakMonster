//--------------------------------------------------------------------------------------
// File: ResultScene.cpp
//
//--------------------------------------------------------------------------------------
#include "ResultScene.h"
#include "Game/Game.h"
#include "Game/Common.h"

// コンストラクタ
ResultScene::ResultScene(Game* pGame)
	: m_pGame{ pGame }
	, m_timer{}
{
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 初期化処理
void ResultScene::Initialize()
{
	// 次の画面に切り替わるまでの時間（３秒）
	m_timer = RESULT_DISPLAY_TIME;
}

// 更新処理
void ResultScene::Update(int keyCondition, int keyTrigger)
{
	if (m_timer > 0) m_timer--;

	// 次のステージへ
	if (m_timer == 0)
	{
		// ゲームプレイシーンへ
		m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
	}
}

// 描画処理
void ResultScene::Render()
{
	// 結果画面の背景の描画
	m_pGame->DrawResult();

	// ゲームモードの表示
	m_pGame->DrawScore(18 * 16 + 8, 5 * 16 + 8, static_cast<int>(m_pGame->GetLevel()) + 1, 1, Common::Color::Cyan, 8);

	// 残機数
	m_pGame->DrawScore(16 * 16 + 8, 20 * 16 + 8, m_pGame->GetRest(), 1, Common::Color::Cyan, 8);

	// 得点
	m_pGame->DrawScore(8 * 16, 15 * 16 + 4, m_pGame->GetScore(), 5, Common::Color::BlueCyan, 8);
}

// 終了処理
void ResultScene::Finalize()
{
}

