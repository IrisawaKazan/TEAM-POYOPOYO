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

// ゲームシーンクラスを定義
class CGame : public CScene
{
public:
	// 規定値を設定
	struct Config {
		// パス
		struct FilePath {
			static constexpr const char* MeshField =		"data\\TEXT\\DefoultStage\\MeshField.txt";		// メッシュの情報
			static constexpr const char* MapObjectInfo =	"data\\TEXT\\DefoultStage\\MapInfo.json";		// マップの情報
			static constexpr const char* GimmickInfo =		"data\\TEXT\\DefoultStage\\GimmickInfo.json";	// ギミックの情報
			static constexpr const char* MapWallInfo =		"data\\TEXT\\DefoultStage\\MapWallInfo.json";	// 見えない壁の情報
		};
		// メッシュシリンダー
		struct MeshCylinder {
			static const D3DXVECTOR3 Pos;				// 位置
			static const D3DXVECTOR3 Rot;				// 向き
			static constexpr float Radius = 5000.0f;	// 半径
			static constexpr float Height = 2500.0f;	// 高さ
			static constexpr int DivX = 8;				// 分割数
			static constexpr int DivY = 3;				// 分割数
		};
		// 川
		struct River {
			static const D3DXVECTOR3 Pos;				// 位置
			static const D3DXVECTOR2 Size;				// 大きさ
		};
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
		// 制限時間用の火
		struct WildFire {
			static constexpr float FireDistance = 200.0f;	// 間隔
			static constexpr int FireInterval = 30;			// 時間の間隔
			static constexpr int FireNum = 6;				// 数
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
	static bool GetPause(void) { return m_isPause; };
private:
	// 静的メンバ変数
	static CPauseManager* m_pPauseManager;				// ポーズマネージャーへのポインタ
	static CPlayerManager* m_pPlayerManager;			// プレイヤーマネージャー
	static D3DXVECTOR3 m_WildFirePos;					// 山火事の位置の配列
	static bool m_isPause;								// ポーズ中かどうか
	std::unique_ptr<btCollisionShape> m_GroundShape;	// 当たり判定
	std::unique_ptr<btRigidBody> m_RigitBody;			// リジットボディー
};
#endif // !_GAME_H_