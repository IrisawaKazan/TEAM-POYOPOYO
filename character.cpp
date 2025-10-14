//************************************************************
//
//　階層構造があるキャラクターの処理[modelcharactor.cpp]
//　Author:Hatori Kensaku
//
//************************************************************
// インクルード
#include "character.h"
#include "manager.h"
//***************************************
// コンストラクタ
//***************************************
CModelCharacter::CModelCharacter(int Priority) : CObject(Priority)
{

}
//***************************************
// コンストラクタ
//***************************************
CModelCharacter::~CModelCharacter()
{

}
//***************************************
// 初期化処理
//***************************************
HRESULT CModelCharacter::Init(const char* MotionFilePath, float Speed)
{
	// 情報を初期化
	m_fSpeed = Speed;
	m_Enable = true;

	// モーションを生成
	m_MotionInfo = CMotion::LoadMotionFile(MotionFilePath, m_apModel);
	m_MotionInfo->SetMotion(1, false);
	return S_OK;
}
//***************************************
// 終了処理
//***************************************
void CModelCharacter::Uninit(void)
{
	if (m_MotionInfo != NULL)
	{
		m_MotionInfo->Uninit();
		m_MotionInfo = NULL;
	}
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); itr++)
	{
		if ((*itr) != NULL)
		{
			(*itr)->Uninit();
		}
	}
	m_apModel.clear();

	// 自分自身の破棄
	Release();
}
//***************************************
// 更新処理
//***************************************
void CModelCharacter::Update(void)
{
	// 移動量を0に近づける
	m_Move.x += (0.0f - m_Move.x) * 0.1f;
	//m_Move.y -= 0.5f;
	m_Move.z += (0.0f - m_Move.z) * 0.1f;
	// 昔の位置を保存
	m_OldPos = m_Pos;
	// 動かす
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;
	m_Pos.z += m_Move.z;

	// 角度の正規化
	if (m_Rot.y > D3DX_PI)
	{
		m_Rot.y = -D3DX_PI + (m_Rot.y - D3DX_PI);
	}
	else if (m_Rot.y < -D3DX_PI)
	{
		m_Rot.y = D3DX_PI + (m_Rot.y + D3DX_PI);
	}
	if (m_RotDest.y > D3DX_PI)
	{
		m_RotDest.y = -D3DX_PI + (m_RotDest.y - D3DX_PI);
	}
	else if (m_RotDest.y < -D3DX_PI)
	{
		m_RotDest.y = D3DX_PI + (m_RotDest.y + D3DX_PI);
	}
	// 角度の近道
	if (m_RotDest.y - m_Rot.y >= D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2.0f;
	}
	else if (m_RotDest.y - m_Rot.y <= -D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	// 今の角度を徐々に近づける
	m_Rot.y += (m_RotDest.y - m_Rot.y) * 0.2f;
	// モーションの更新
	m_MotionInfo->Update(m_apModel);
}
//***************************************
// 描画処理
//***************************************
void CModelCharacter::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	D3DXMATRIX mtxParent;	// 計算用マトリックス
	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);
	// 有効だったらモデルを描画する
	if (m_Enable == true)
	{
		for (auto itr = m_apModel.begin(); itr != m_apModel.end(); itr++)
		{
			if ((*itr) != NULL)
			{
				(*itr)->Draw();
			}
		}
	}
	pDevice->SetMaterial(&matDef);
}
