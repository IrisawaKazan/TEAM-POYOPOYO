//*************************************
//
//　2Dポリゴン処理[Object2D.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

// インクルード防止
#include "Object.h"

// オブジェクトクラスを継承した2Dポリゴン用のクラスを宣言
class CObject2D :public CObject
{
public:
	// コンストラクタ・デストラクタ
	CObject2D(int Priority = 3);
	~CObject2D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject2D* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot);
	static CObject2D* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot,const D3DXVECTOR2 Size);

	// セッター
	void SetPosition(const D3DXVECTOR3 Pos);
	void SetRotasion(const D3DXVECTOR3 Rot);
	void SetSize(const D3DXVECTOR2 Size);
	void SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV);
	void SetCol(const D3DXCOLOR Col);
	void SetTexIndx(const int Indx) { m_nTexIndx = Indx; };

	// ゲッター
	D3DXVECTOR3 GetPosition(void) {return m_Pos; };
	D3DXVECTOR3 GetRotasion(void) { return m_Rot; };
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth, m_fHeigth); };
	int GetTexIndx(void) { return m_nTexIndx; };
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	D3DXVECTOR3 m_Pos;					// 位置
	D3DXVECTOR3 m_Rot;					// 向き
	float m_fAngle;						// 中心点から四隅までの角度
	float m_fLength;					// 長さ
	float m_fWidth;						// 横幅
	float m_fHeigth;					// 高さ
	int m_nTexIndx;						// テクスチャへのインデックス
};
#endif // !_OBJECT2D_H_