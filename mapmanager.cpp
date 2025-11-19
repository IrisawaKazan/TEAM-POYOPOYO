//****************************************************************
//
// マップマネージャーの処理[mapmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "mapmanager.h"
#include "block.h"
#include "modelmanager.h"
#include "switch.h"
#include "scene.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "playermanager.h"
#include "goal.h"
#include "item.h"	// Misaki
#include "navi.h"

// ネームスペース
using namespace nlohmann;
using namespace std;

// 静的メンバ変数
bool CMapManager::m_bGoal = false;

//***************************************
// デストラクタ
//***************************************
CMapManager::~CMapManager()
{
	// クリア
	m_vMapObject.clear();
	m_vMapSwitch.clear();
	m_vMapItem.clear();

}

//***************************************
// 初期化処理
//***************************************
HRESULT CMapManager::Init(void)
{
	// ベクターをクリア
	m_vMapObject.clear();
	m_vMapSwitch.clear();
	m_vMapItem.clear();

	m_bGoal = false;

	// 正常終了
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CMapManager::Uninit(void)
{
}

//***************************************
// 更新処理
//***************************************
void CMapManager::Update(void)
{
	// 何個のスイッチが押されているか
	int nFinished = 0;

	// スイッチにアクセス
	for (auto Switchs = m_vMapSwitch.begin(); Switchs != m_vMapSwitch.end(); Switchs++)
	{
		// 押されていたら
		if ((*Switchs)->IsPress() == true)
		{
			// インクリメント
			nFinished++;
		}
	}

	// すべて押されていたら
	if (nFinished >= (int)m_vMapSwitch.size())
	{
		// 起動
		m_Door->Begin();
	}
	else
	{
		// シャットダウン
		m_Door->End();
	}

	// スイッチとプレイヤーの当たり判定
	CollisionSwitchtoPlayers();

	// ゴールとプレイヤーの当たり判定
	CollisionGoaltoPlayers();

	// アイテムとプレイヤーの当たり判定
	CollisionItemtoPlayers();

	// ナビにレイキャストオブジェクトを登録 sato
	{
		CModelManager* pModelManager = CModelManager::Instance();
		CModelManager::ModelInfo modelinfo{};
		for (const auto& pObject : m_vMapObject)
		{// ブロック
			modelinfo = pModelManager->GetAddress(pObject->GetIndx());
			if (pObject->GetFilepath().find("Switch") == std::string::npos)
			{// switchの土台はキャストしない
				CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, pObject->GetWorldMtx());
			}
			// 隠れはする
			CNavi::GetInstance()->RegisterLatentObject(modelinfo.pMesh, pObject->GetWorldMtx());
		}
		for (const auto& pObject : m_vMapSwitch)
		{// switchは隠すだけ
			modelinfo = pModelManager->GetAddress(pObject->GetIndx());
			CNavi::GetInstance()->RegisterLatentObject(modelinfo.pMesh, pObject->GetWorldMtx());
		}
		for (const auto& pSlope : m_vMapSlopes)
		{// スロープも隠すだけ
			modelinfo = pModelManager->GetAddress(pSlope->GetIndx());
			CNavi::GetInstance()->RegisterLatentObject(modelinfo.pMesh, pSlope->GetWorldMtx());
		}
		// ドアは隠すだけ
		modelinfo = pModelManager->GetAddress(m_Door->GetIndx());
		CNavi::GetInstance()->RegisterLatentObject(modelinfo.pMesh, m_Door->GetWorldMtx());
	}
	// ナビにレイキャストオブジェクトを登録 end
}

//***************************************
// 描画処理
//***************************************
void CMapManager::Draw(void)
{
}

//***************************************
// プレイヤーたちとゴールの当たり判定
//***************************************
void CMapManager::CollisionGoaltoPlayers(void)
{
	// 何組が衝突しているか
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// 衝突回数分繰り返し
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// プレイヤーの配列にアクセス
		for (auto Players = CGame::GetPlayerManager()->GetVPlayer().begin(); Players != CGame::GetPlayerManager()->GetVPlayer().end(); Players++)
		{
			// 衝突しているペアを取得
			btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

			// 衝突していたら
			if (manifold->getNumContacts() <= 0) continue;

			// 衝突オブジェクト１、２を取得
			const btCollisionObject* objA = manifold->getBody0();
			const btCollisionObject* objB = manifold->getBody1();

			// プレイヤーとゴールが当たっていたら
			const bool Condition = (objA == (*Players)->GetRB() && objB == m_Goal->GetCollisionObject()) || (objA == m_Goal->GetCollisionObject() && objB == (*Players)->GetRB());

			// 切り上げ
			if (Condition == false) continue;

			if (CFade::GetFadeSingle()->GetFade() != CFade::FADE_OUT)
			{
				m_bGoal = true;
				CFade::SetFade(new CResult);
			}

			// 処理を切り上げる
			break;
		}
	}
}

//***************************************
// プレイヤーたちとスイッチの当たり判定
//***************************************
void CMapManager::CollisionSwitchtoPlayers(void)
{
	// 何組が衝突しているか
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// 押されたかどうか
	bool IsPressed = false;

	// スイッチの配列にアクセス
	for (auto Switch = m_vMapSwitch.begin(); Switch != m_vMapSwitch.end(); Switch++)
	{
		// 押されていない状態にする
		IsPressed = false;

		// 衝突回数分繰り返し
		for (int nCnt = 0; nCnt < numManifolds; nCnt++)
		{
			// プレイヤーの配列にアクセス
			for (auto Players = CGame::GetPlayerManager()->GetVPlayer().begin(); Players != CGame::GetPlayerManager()->GetVPlayer().end(); Players++)
			{
				// 衝突しているペアを取得
				btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

				// 衝突していたら
				if (manifold->getNumContacts() <= 0) continue;

				// 衝突オブジェクト１、２を取得
				const btCollisionObject* objA = manifold->getBody0();
				const btCollisionObject* objB = manifold->getBody1();

				// プレイヤーとスイッチが当たっていたら
				const bool Condition = (objA == (*Players)->GetRB() && objB == (*Switch)->GetRB()) || (objA == (*Switch)->GetRB() && objB == (*Players)->GetRB());

				// 切り上げ
				if (Condition == false) continue;

				IsPressed = true;

				// 処理を切り上げる
				break;
			}
			// 押されていたら切り上げる
			if (IsPressed == true) break;
		}
		// 状態を設定
		(*Switch)->SetPressed(IsPressed);
	}
}

//***************************************
// プレイヤーたちとアイテムの当たり判定 Misaki
//***************************************
void CMapManager::CollisionItemtoPlayers(void)
{
	// 何組が衝突しているか
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// アイテムの配列にアクセス
	for (auto Item = m_vMapItem.begin(); Item != m_vMapItem.end(); Item++)
	{
		// 衝突回数分繰り返し
		for (int nCnt = 0; nCnt < numManifolds; nCnt++)
		{
			// プレイヤーの配列にアクセス
			for (auto Players = CGame::GetPlayerManager()->GetVPlayer().begin(); Players != CGame::GetPlayerManager()->GetVPlayer().end(); Players++)
			{
				// 衝突しているペアを取得
				btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

				// 衝突していたら
				if (manifold->getNumContacts() <= 0) continue;

				// 衝突オブジェクト１、２を取得
				const btCollisionObject* objA = manifold->getBody0();
				const btCollisionObject* objB = manifold->getBody1();

				// プレイヤーとアイテムが当たっていたら
				const bool Condition = (objA == (*Players)->GetRB() && objB == (*Item)->GetCollisionObject()) || (objA == (*Item)->GetCollisionObject() && objB == (*Players)->GetRB());
				
				if (Condition == false)
				{// 当たっていなかった場合
					continue;
				}
				else if(Condition == true)
				{// 当たっている場合
					// アイテムを持っている状態に変更する
					(*Item)->SetItemTake(true);
				}

				// 処理を切り上げる
				break;
			}
		}
	}
}

//***************************************
// オブジェクトを生成
//***************************************
void CMapManager::CreateObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	// 生成、要素に追加
	CBlock* LocalObject = NULL;
	LocalObject = CBlock::Create(Path, Pos, Rot);
	m_vMapObject.push_back(LocalObject);
}

//***************************************
// 読み込み
//***************************************
void CMapManager::Load(std::string Path)
{
	// jsonファイルを開く
	ifstream ifs(Path);

	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}

	// クリア
	m_vMapObject.clear();
	m_vMapSwitch.clear();
	m_vMapItem.clear();
	m_vMapSlopes.clear();

	// jsonデータを宣言
	ordered_json jsonData;
	// jsonを読み込む
	ifs >> jsonData;

	// SetStage配列の中にあるデータを代入存在しなかったら終わる
	for (const auto& obj : jsonData)
	{
		bool Static;
		float Mass;
		D3DXVECTOR3 Pos, Scale;
		D3DXQUATERNION Quad;
		std::string LocalPath;

		// IsStaticタグのデータを取り出す
		Static = obj["RigitBody"]["IsStatic"];
		// Massタグのデータを取り出す
		Mass = obj["RigitBody"]["Mass"];

		// Pathタグのデータを取り出す
		LocalPath = obj["Assets"]["Path"];

		// Posタグのデータを取り出す
		Pos.x = obj["Transform"]["Pos"]["x"];
		Pos.y = obj["Transform"]["Pos"]["y"];
		Pos.z = obj["Transform"]["Pos"]["z"];

		// Rotタグのデータを取り出す
		Quad.w = obj["Transform"]["Rot"]["w"];
		Quad.x = obj["Transform"]["Rot"]["x"];
		Quad.y = obj["Transform"]["Rot"]["y"];
		Quad.z = obj["Transform"]["Rot"]["z"];

		// Scaleタグのデータを取り出す
		Scale.x = obj["Transform"]["Scale"]["x"];
		Scale.y = obj["Transform"]["Scale"]["y"];
		Scale.z = obj["Transform"]["Scale"]["z"];

		if (LocalPath.find("Switch-Button") != string::npos)
		{
			// 生成、要素に追加
			CSwitch* LocalObject = NULL;
			LocalObject = CSwitch::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// 連結
			m_vMapSwitch.push_back(LocalObject);
		}
		else if (LocalPath.find("Door") != string::npos)
		{
			// 生成、要素に追加
			m_Door = CDoor::Create(LocalPath, Pos, VEC3_NULL);
			m_Door->SetScale(Scale);
			m_Door->SetQuat(CMath::ConvertQuat(Quad));
			m_Door->SetIdx(LocalPath);

			m_Goal = CGoal::Create(Pos, m_Door->GetSize());
		}
		else if (LocalPath.find("slope") != string::npos)
		{
			CBlock* Local;
			Local = CBlock::Create(LocalPath, Pos, VEC3_NULL);
			Local->SetScale(Scale);
			Local->SetQuat(CMath::ConvertQuat(Quad));
			Local->SetIdx(LocalPath);
			// 連結
			m_vMapSlopes.push_back(Local);
		}
		else
		{
			// 生成、要素に追加
			CBlock* LocalObject = NULL;
			LocalObject = CBlock::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// 連結
			m_vMapObject.push_back(LocalObject);
		}
	}

	// アイテムの生成処理 Misaki
	CItem* Instanfce;
	Instanfce = CItem::Create(CItem::ITEM_JUMP,	// 種類
		D3DXVECTOR3(300.0f, 50.0f, 600.0f), 	// 位置
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 			// 向き
		D3DXVECTOR3(1.2f, 1.2f, 1.2f),			// 拡大率
		"data\\MODEL\\item\\item002.x");		// テキストファイル

		// 連結
	m_vMapItem.push_back(Instanfce);

	// アイテムの生成処理 Misaki
	Instanfce = CItem::Create(CItem::ITEM_CLIMB,	// 種類
		D3DXVECTOR3(700.0f, 200.0f, -500.0f), 	// 位置
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 			// 向き
		D3DXVECTOR3(1.2f, 1.2f, 1.2f),			// 拡大率
		"data\\MODEL\\item\\item003.x");		// テキストファイル

		// 連結
	m_vMapItem.push_back(Instanfce);

	// アイテムの生成処理 Misaki
	Instanfce = CItem::Create(CItem::ITEM_LEFT,	// 種類
		D3DXVECTOR3(300.0f, 50.0f, -75.0f), 	// 位置
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 			// 向き
		D3DXVECTOR3(1.2f, 1.2f, 1.2f),			// 拡大率
		"data\\MODEL\\item\\item001.x");		// テキストファイル

	// 連結
	m_vMapItem.push_back(Instanfce);
}