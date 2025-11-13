//****************************************************************
//
// アイテムの処理
// Author:Takahashi Misaki
//
//****************************************************************

// インクルード
#include "item.h"
#include "modelmanager.h"
#include "game.h"
#include "navi.h"
#include "tutorialBoard.h"

//***************************************
// コンストラクタ
//***************************************
CItem::CItem(int nPriority) :CObjectX(nPriority)
{
	m_type = ITEM_NOEN;								// アイテムの種類
	m_size = D3DXVECTOR3(50.0f, 50.0f, 50.0f);		// サイズ
	m_bTake = false;								// 入手したかどうか
	m_RBOffset = VEC3_NULL;							// リジットボディのオフセット
}

//***************************************
// デストラクタ
//***************************************
CItem::~CItem()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CItem::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	// 位置情報を取得
	D3DXVECTOR3 pos = GetPosition();

	// メモリ確保OBBの大きさを設定
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x, m_size.y, m_size.z));

	// GhostObjectを作成
	m_GhostObject = std::make_unique<btGhostObject>();

	// 衝突形状を設定
	m_GhostObject->setCollisionShape(m_CollisionShape.get());

	// ワールド変換（位置と回転）を設定
	D3DXVECTOR3 Offset = VEC3_NULL;
	Offset.y += m_size.y * 0.5f;
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.x, pos.y + Offset.y, pos.z)); // 位置を設定
	m_GhostObject->setWorldTransform(transform);

	// 衝突グループとマスクを設定してワールドに追加
	CManager::GetDynamicsWorld()->addCollisionObject(
		m_GhostObject.get(),
		btBroadphaseProxy::SensorTrigger,
		btBroadphaseProxy::AllFilter
	);

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CItem::Uninit(void)
{
	// 剛体の削除
	if (m_GhostObject)
	{
		CManager::GetDynamicsWorld()->removeCollisionObject(m_GhostObject.get());
		m_GhostObject.reset();
	}

	// 衝突形状の削除
	m_CollisionShape.reset();

	// オブジェクトXの終了処理
	CObjectX::Uninit();

}

//***************************************
// 更新処理
//***************************************
void CItem::Update(void)
{
	// クオータニオン
	D3DXQUATERNION pQuat;
	// 回転させる軸
	D3DXVECTOR3 Axis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	// 向き
	static float fAngle = 0.0f;
	// 位置
	D3DXVECTOR3 pos = VEC3_NULL;
	// カウント
	static int nCount = 0;
	static int test = 0;

	// クオータニオンの取得
	pQuat = GetQuad();
	// 位置の取得
	pos = GetPosition();

	if (m_type == ITEM_CLIMB)
	{
		Axis = { 0.0f,1.0f,0.0f };
	}
	if (m_type != ITEM_JUMP)
	{
		if (m_bTake == false)
		{// 入手していない場合

			// 回転量を加算
			fAngle += 0.05f;

			// クオータニオンの計算
			D3DXQuaternionRotationAxis(&pQuat, &Axis, fAngle);

			// 更新したクオータニオンを引数に代入
			SetQuad(pQuat);
		}
	}
	else
	{
		if (m_bTake == false)
		{// 入手していない場合

			test++;
			float testsinf;
			testsinf = sinf(0.05f * test);

			D3DXVECTOR3 set;
			set = GetPosition();
			set.y = 50.0f + (testsinf * 10.0f);
			SetPosition(set);
		}
	}
	if (m_bTake == true)
	{// 入手した場合

		// 回転量を加算
		fAngle += 0.2f * 0.9f;

		// 位置に移動量を加算
		pos += D3DXVECTOR3(0.0f, 3.0f * 0.8f, 0.0f);

		// クオータニオンの計算
		D3DXQuaternionRotationAxis(&pQuat, &Axis, fAngle);

		// 更新したクオータニオンを引数に代入
		SetQuad(pQuat);
		SetPosition(pos);

		// カウントを一つ進める
		nCount++;

		if (nCount >= 30)
		{// カウントが設定値を超えた場合

			// カウントを初期化
			nCount = 0;

			// 終了処理
			Uninit();

			// アイテムの種類で分ける
			if (m_type == ITEM_LEFT)
			{// 左向きのアイテムの場合
				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_003.png");

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::LIST::LeftArrow, true);

			}
			else if (m_type == ITEM_JUMP)
			{
				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_002.png");

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::LIST::Jump, true);
			}
			else if (m_type == ITEM_CLIMB)
			{
				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_004.png");

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::LIST::Climb, true);
			}

		}
	}

}


//***************************************
// 描画処理
//***************************************
void CItem::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();

}

//***************************************
// 生成処理
//***************************************
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale,  const std::string FileName)
{
	// アイテムのポインタ
	CItem* pItem = nullptr;

	// 生成
	pItem = new CItem;

	if (pItem == nullptr)
	{// ポインタがnullptrだったら
		return nullptr;
	}

	// オブジェクトXのセッターに引数の値を入れる
	pItem->SetPosition(pos);	// 位置
	pItem->SetRotasion(rot);	// 向き
	pItem->SetScale(scale);		// 拡大率
	pItem->SetIdx(FileName);	// モデルのファイル名

	// メンバ変数に代入する
	pItem->m_type = type;

	// 初期化処理
	pItem->Init();

	// ポインタを返す
	return pItem;
}
