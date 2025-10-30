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

// ネームスペース
using namespace nlohmann;
using namespace std;

//***************************************
// 初期化処理
//***************************************
HRESULT CMapManager::Init(void)
{
	// ベクターをクリア
	m_vMapObject.clear();

	// 正常終了
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CMapManager::Uninit(void)
{
	// 今置いてあるオブジェクトの破棄
	for (auto MapObjects = m_vMapObject.begin(); MapObjects != m_vMapObject.end(); MapObjects++)
	{
		(*MapObjects)->Uninit();
		MapObjects = m_vMapObject.erase(MapObjects);
		if (MapObjects == m_vMapObject.end()) break;
	}

	// クリア
	m_vMapObject.clear();
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

	// 今置いてあるオブジェクトの破棄
	for (auto MapObjects = m_vMapObject.begin(); MapObjects != m_vMapObject.end(); MapObjects++)
	{
		(*MapObjects)->Uninit();
		MapObjects = m_vMapObject.erase(MapObjects);
		if (MapObjects == m_vMapObject.end()) break;
	}

	// クリア
	m_vMapObject.clear();

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