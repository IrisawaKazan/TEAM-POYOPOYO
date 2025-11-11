////==============================================================
////
//// [number.h]
//// Author: Irisawa Kazan
////
////==============================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

class CNumber
{
public:

	CNumber();
	~CNumber();
	HRESULT Init(float fX1, float fX2, float fY1,float fY2,int nCnt, int nCnt2, float fNum1, float fNum2, float fNum3, int nNum, int nAdd, const char* FileName, float fx);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumber* Create(D3DXVECTOR3 pos);

	// セッター
	void SetNumber(int nNumber, int nAdd);

	void ColAnim(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_move;						// 移動
	bool m_bChange;							// カラー判定用
	int m_nColTime;							// カラー用の計算
	int m_nIdx;								// インデックス番号

};
#endif