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

// ネームスペース
using namespace nlohmann;
using namespace std;

//***************************************
// デストラクタ
//***************************************
CMapManager::~CMapManager()
{
	// 剛体の削除
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
	}

	// 衝突形状の削除
	m_CollisionShape.reset();
	// クリア
	m_vMapObject.clear();
	m_vDoor.clear();
	m_vMapSwitch.clear();
}

//***************************************
// 初期化処理
//***************************************
HRESULT CMapManager::Init(void)
{
	// ベクターをクリア
	m_vMapObject.clear();
	m_vDoor.clear();
	m_vMapSwitch.clear();

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

			// プレイヤーとスイッチが当たっていたら
			const bool Condition = (objA == (*Players)->GetRB() && objB == m_RigitBody.get()) || (objA == m_RigitBody.get() && objB == (*Players)->GetRB());

			// 切り上げ
			if (Condition == false) continue;

			if (CFade::GetFadeSingle()->GetFade() != CFade::FADE_OUT)
			{
				CFade::SetFade(new CResult);
			}

			// 処理を切り上げる
			break;
		}
	}

	// すべて押されていたら
	if (nFinished >= (int)m_vMapSwitch.size())
	{
		// 起動
		m_vDoor[0]->Begin();
	}
	else
	{
		// シャットダウン
		m_vDoor[0]->End();
	}
}

//***************************************
// 描画処理
//***************************************
void CMapManager::Draw(void)
{
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
	m_vDoor.clear();
	m_vMapSwitch.clear();

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
			CDoor* LocalObject = NULL;
			LocalObject = CDoor::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// 連結
			m_vDoor.push_back(LocalObject);

			// メモリ確保OBBの大きさを設定
			m_CollisionShape = std::make_unique<btBoxShape>(btVector3(LocalObject->GetSize().x, LocalObject->GetSize().y, LocalObject->GetSize().z));

			// 質量
			btScalar mass = 0.0f;

			// 慣性モーメントの計算用変数
			btVector3 inertia(0, 0, 0);

			// 計算
			m_CollisionShape->calculateLocalInertia(mass, inertia);

			// 最終的な計算結果、位置、位置からのオフセット
			btTransform transform;

			// 初期化
			transform.setIdentity();

			// 合成
			transform.setOrigin(btVector3(Pos.x, Pos.y + LocalObject->GetSize().y * 0.5f, Pos.z));

			// 位置と向きを管理するモーションを生成
			btDefaultMotionState* motionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

			// 生成
			m_RigitBody = std::make_unique<btRigidBody>(info);

			// 移動制限を設定
			m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

			// 物理ボディーに自分自身を紐付け
			m_RigitBody->setUserPointer(this);

			// 動的オブジェクトか静的オブジェクトか
			m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

			// 物理世界に物理ボディーを追加
			CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
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
}