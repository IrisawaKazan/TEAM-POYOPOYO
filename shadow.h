//======================================================
// 
// 影の処理
// Author:Takahashi Misaki
// 
//======================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// 影クラス
//*****************************************************************************
class CShadow :public CObject3D
{
public:

	// コンストラクタ
	CShadow(int nPriority = 4);
	// デストラクタ
	~CShadow();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CShadow* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* FilePath);

private:
	// メンバ変数

};

#endif