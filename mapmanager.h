//****************************************************************
//
// マップマネージャーの処理[mapmanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

// 前方宣言
class CBlock;
class CSwitch;
class CDoor;
class CGoal;
class CItem;

// クラスを定義
class CMapManager
{
public:
	// デストラクタ
	~CMapManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CollisionGoaltoPlayers(void);
	void CollisionSwitchtoPlayers(void);
	void CollisionItemtoPlayers(void); // Misaki
	void CollisionSlopetoPlayers(void);
	// 生成
	void CreateObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path);

	// 読み込み
	void Load(std::string Path);

	// 静的メンバ関数
	// シングルトン
	static CMapManager* Instance(void) {
		static std::unique_ptr<CMapManager> Instance(new CMapManager);
		return Instance.get();
	};

	// ゴール判定の取得
	static bool GetGoal(void) { return m_bGoal; };

private:
	// コンストラクタ
	CMapManager() { Init(); }
	// メンバ変数
	// ゴールの当たり判定
	std::vector<CBlock*> m_vMapObject;					// マップに置くオブジェクト
	CDoor* m_Door;										// ドア
	CGoal* m_Goal;										// ゴール
	CBlock* m_Slope;									// 滑り台
	std::vector<CSwitch*> m_vMapSwitch;					// マップに置くスイッチ
	// アイテムの当たり判定 Misaki
	CItem* m_Item;
	std::vector<CItem*> m_vMapItem;					// マップに置くアイテム

	static bool m_bGoal;							// ゴールの判定(タイム用)
};

#endif // !_MAPMANAGER_H_