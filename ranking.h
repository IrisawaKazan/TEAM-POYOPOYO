//****************************************************************
//
// �����L���O�̏���[ranking.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _RANKING_H_
#define _RANKING_H_

//  �C���N���[�h
#include "object.h"
#include "object2D.h"
#include "number.h"

//  �����L���O�N���X���`
class CRanking
{
public:
	//	�R���X�g���N�^�E�f�X�g���N�^
	CRanking();
	~CRanking();

	//	�����o�֐�
	static CRanking* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif // !_RANKING_H_