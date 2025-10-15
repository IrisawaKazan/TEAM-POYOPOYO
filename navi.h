//------------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "objectbillboard.h"

//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̃N���X
//--------------------------------
class CNavi : public CObjectBillBoard
{
public:
	CNavi() {};
	~CNavi() {};
	static CNavi* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void CreateRay(D3DXVECTOR2 mousePos);
};