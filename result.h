//****************************************************************
//
// ���U���g����[title.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _RESULT_H_
#define _RESULT_H_

// �C���N���[�h
#include "scene.h"

//	���U���g�N���X���`
class CResult	:public CScene
{
public:
	//	�R���X�g���N�^�E�f�X�g���N�^���N�^
	CResult();
	~CResult();

	//	�����o�֐�
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;

private:


};

#endif // !_RESULT_H_


