//*************************************
//
//　キャラクター処理[character.h]
//　Author:chikada shouya
//
//*************************************
// 二重インクルード防止
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "object.h"

class CCharacter : public CObject
{
public:
	CCharacter();
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;						//移動
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_rotDest;					//目的の向き

};

#endif