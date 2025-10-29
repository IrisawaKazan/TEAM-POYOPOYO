//=================================================
//
//	switch.cpp
//
// Author:chikada shouya
//
//=================================================
// 
//***************************************
// インクルードファイル宣言
//***************************************
#include "switch.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"
#include "playermanager.h"

//***************************************
// コンストラクタ
//***************************************
CSwitch::CSwitch(int nPriority) :CBlock(nPriority)
{
}

//***************************************
// デストラクタ
//***************************************
CSwitch::~CSwitch()
{
}

//***************************************
// スイッチ生成
//***************************************
CSwitch* CSwitch::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CSwitch* pSwitch = nullptr;
	pSwitch = new CSwitch;
	if (pSwitch != nullptr)
	{
		// オブジェクト設定
		pSwitch->SetFilepath(sName);
		pSwitch->SetPosition(pos);
		pSwitch->SetRotasion(rot);
		pSwitch->SetScale(Scale);
		pSwitch->Init();
		return pSwitch;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//　スイッチ初期化処理
//***************************************
HRESULT CSwitch::Init(void)
{
	// 初期化
	CBlock::Init();

	return S_OK;
}

//***************************************
//　スイッチ終了処理
//***************************************
void CSwitch::Uninit(void)
{
	// 破棄
	CBlock::Uninit();
}

//***************************************
//　スイッチ更新処理
//***************************************
void CSwitch::Update(void)
{
	// 衝突判定
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();
	D3DXVECTOR3 myPos = GetPosition();

	for (int i = 0; i < numManifolds; i++)
	{
		for (auto Players = CGame::GetPlayerManager()->GetVPlayer().begin(); Players != CGame::GetPlayerManager()->GetVPlayer().end(); Players++)
		{
			btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);

			const btCollisionObject* objA = manifold->getBody0();
			const btCollisionObject* objB = manifold->getBody1();

			if (manifold->getNumContacts() > 0)
			{
				if ((objA == (*Players)->GetRB() && objB == GetRB()) || (objA == GetRB() && objB == (*Players)->GetRB()))
				{
					myPos.y -= 0.1f;
				}
				else if (myPos.y <= 0)
				{
					myPos.y += 0.1f;
				}
			}
		}
	}
	SetPosition(myPos);
	// 更新
	CBlock::Update();
}

//***************************************
//　スイッチ描画処理
//***************************************
void CSwitch::Draw(void)
{
	// 描画
	CBlock::Draw();
}