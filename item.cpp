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
	m_pShadow = NULL;								// 影のポインタ
	m_fAngle = 0.0f;								// 向き
	m_nCount = 0;									// 取った時のカウント
	m_test = 0;										// 
	m_bTutorial = true;								// 特別な指示を出すか

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

	// 影の位置に変更
	pos.y -= (Offset.y * 1.5f);

	// 影の生成処理
	m_pShadow = CShadow::Create(pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), m_size.x * 0.6f, m_size.z * 0.6f, "data\\TEXTURE\\Effect\\effect000.jpg");

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

	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// オブジェクトXの終了処理
	CObjectX::Uninit();

}

//***************************************
// 更新処理
//***************************************
void CItem::Update(void)
{
	// クオータニオン
	D3DXQUATERNION Quat;
	// 回転させる軸
	D3DXVECTOR3 Axis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	// 位置
	D3DXVECTOR3 pos = VEC3_NULL;

	// クオータニオンの取得
	Quat = GetQuad();
	// 位置の取得
	pos = GetPosition();

	if (m_type == ITEM_CLIMB)
	{// 登る指示の場合
		Axis = { 0.0f,1.0f,0.0f };
	}
	if (m_type != ITEM_JUMP)
	{// ジャンプの指示の場合

		if (m_bTake == false)
		{// 入手していない場合

			// 回転量を加算
			m_fAngle += 0.05f;

			// クオータニオンの計算
			D3DXQuaternionRotationAxis(&Quat, &Axis, m_fAngle);

			// 更新したクオータニオンを引数に代入
			SetQuad(Quat);
		}
	}
	else
	{// それ以外

		if (m_bTake == false)
		{// 入手していない場合

			m_test++;
			float testsinf;
			testsinf = sinf(0.05f * m_test);

			D3DXVECTOR3 set;
			set = GetPosition();
			set.y = 50.0f + (testsinf * 10.0f);
			SetPosition(set);
		}
	}

	if (m_bTake == true)
	{// 入手した場合

		// 回転量を加算
		m_fAngle += 0.2f * 0.9f;

		// 位置に移動量を加算
		pos += D3DXVECTOR3(0.0f, 3.0f * 0.8f, 0.0f);

		// クオータニオンの計算
		D3DXQuaternionRotationAxis(&Quat, &Axis, m_fAngle);

		// 更新したクオータニオンを引数に代入
		SetQuad(Quat);
		SetPosition(pos);

		// カウントを一つ進める
		m_nCount++;

		if (m_nCount == 1)
		{
			// サウンドの取得
			CSound* pSound = CManager::GetSound();

			// SE
			pSound->Play(CSound::LABEL_ITEM_SE);
		}

		if (m_nCount >= 30)
		{// カウントが設定値を超えた場合

			// カウントを初期化
			m_nCount = 0;

			// 終了処理
			Uninit();

			switch (m_type)
			{
			case ITEM_LEFT:

				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_003.png", false);

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::ARROW::Left, true);

				break;

			case ITEM_JUMP:

				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_002.png", m_bTutorial);

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::TYPE::Jump, true);

				// チュートリアルを見ない状態にする
				m_bTutorial = false;

				break;

			case ITEM_CLIMB:

				// チュートリアル表示
				CGame::GetTutorialBoard()->SetUp("data\\TEXTURE\\tutorial_004.png", m_bTutorial);

				// アイテムの有効化
				CNavi::GetInstance()->SetEnable(CNavi::TYPE::Climb, true);

				// チュートリアルを見ない状態にする
				m_bTutorial = false;

				break;

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
