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
	typedef struct
	{
		D3DXVECTOR3 pos;//頂点座標
		float rhw;		//座用変換用係数(1.0fで固定)
		D3DCOLOR col;	//頂点カラー
		D3DXVECTOR2 tex;//テクスチャ座標

	}VERTEX_2D;

	CNumber();
	~CNumber();
	HRESULT Init(float fX1, float fX2, float fY1,float fY2,int nCnt, float fNum1, float fNum2, int nNum, int nAdd, const char* FileName, float fx);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumber* Create(D3DXVECTOR3 pos);
	void SetNumber(int nNumber, int nAdd);


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;						// 弾の位置
	int m_nIdx;								// インデックス番号

};
#endif