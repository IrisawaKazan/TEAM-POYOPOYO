//****************************************************************
//
// 階層構造モデル処理[model.h]
// Author Kensaku Hatori
//
//****************************************************************
// 二重インクルード防止
#ifndef _MODEL_H_
#define _MODEL_H_
// インクルード
#include "main.h"
// モデルクラスを定義
class CModel
{
public:
	// コンストラクタ・デストラクタ
	CModel();
	~CModel();
	// いつもの
	HRESULT Init(const char* ModelFileName);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	void Draw(D3DXCOLOR Col);
	// セッター
	void SetParent(CModel* pParent) { m_ParentModel = pParent; };
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetOffSet(D3DXVECTOR3 OffSet) { m_OffSet = OffSet; };
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetMtxRot(D3DXMATRIX mtxrot) { m_mtxRot = mtxrot; };
	void SetIndx(const int Indx) { m_nIndx = Indx; };
	// ゲッター
	CModel* GetParentModel(void) { return m_ParentModel; };
	D3DXVECTOR3 GetOffSet(void) { return m_OffSet; };
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; };
	int GetIndx(void) { return m_nIndx; };
	static CModel* Create(const char* ModelFileName);
private:
	// モデル情報の構造体
	LPD3DXMESH m_pMesh;						// メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアルへのポインタ
	DWORD m_dwNumMat;						// マテリアルの数
	int m_nTexNum;							// モデルのテクスチャの総数
	int* m_nTexIndx;						// モデルのテクスチャのインデックス
	D3DXVECTOR3 m_OffSet;					// オフセット
	D3DXVECTOR3 m_Pos;						// 位置
	D3DXVECTOR3 m_Rot;						// 向き
	D3DXMATRIX m_mtxWorld;					// マトリックス
	D3DXMATRIX m_mtxRot;					// 回転行列
	CModel* m_ParentModel;					// 親モデル
	int m_nIndx;							// 階層構造のインデックス
};
#endif // !
