//****************************************************************
//
// ゲームの処理[game.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二十インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

// インクルード
#include "scene.h"

// 前方宣言
class CPlayer;
class CScore;
class CTimer;
class CMapEditer;
class CPauseManager;
class CPlayerManager;
class CMapManager;
class CTutorialBoard;

// ゲームシーンクラスを定義
class CGame : public CScene
{
public:
	// 規定値を設定
	struct Config {
		// プレイヤー
		struct Player {
			static const D3DXVECTOR3 Pos;				// 位置
			static const D3DXVECTOR3 Rot;				// 向き
		};
		// カメラ
		struct Camera {
			static const D3DXVECTOR3 Rot;				// 向き
			static const D3DXVECTOR3 PosR;				// 注視点
			static const D3DXVECTOR3 PosV;				// 始点
		};
		// 空
		struct Sky {
			static const D3DXVECTOR3 Pos;				// 位置
			static constexpr float Radius = 7000.0f;	// 半径
			static constexpr int Horizon = 8;			// 分割数
			static constexpr int Vertical = 5;			// 分割数
		};
	};
	// コンストラクタ・デストラクタ
	CGame();
	~CGame();

	// メンバ関数
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;

	// 静的メンバ関数
	// セッター
	static void ResetPlayer(void);

	// ゲッター
	static CTutorialBoard* GetTutorialBoard(void) { return m_pTutorialBoard; }
	static CPlayerManager* GetPlayerManager(void) { return m_pPlayerManager; }; // sato Add
	static bool GetGoal(void) { return m_bGoal; };
private:
	// ナビゲーションオブジェクトUIのテクスチャリスト
	static constexpr std::array<const char*, 6u> NAVI_UI_TEXTURES =
	{
		"data/TEXTURE/UI/ArrowMark002.png",
		"data/TEXTURE/UI/ArrowMark001.png",
		"data/TEXTURE/UI/ArrowMark003.png",
		"data/TEXTURE/UI/ArrowMark000.png",
		"data/TEXTURE/UI/ClimbMark000.png",
		"data/TEXTURE/UI/JumpMark000.png"
	};

	// 静的メンバ変数
	static CPauseManager* m_pPauseManager;				// ポーズマネージャーへのポインタ
	static CPlayerManager* m_pPlayerManager;			// プレイヤーマネージャー
	static CMapManager* m_pMapManager;			// プレイヤーマネージャー

	static CTutorialBoard* m_pTutorialBoard;

	static bool m_bGoal;
};
#endif // !_GAME_H_