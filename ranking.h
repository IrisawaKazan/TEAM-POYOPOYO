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

//	�}�N����`
#define MAX_NUMBER (2)

//  �����L���O�N���X���`
class CRanking
{
public:
	//	�R���X�g���N�^�E�f�X�g���N�^
	CRanking();
	~CRanking();

	//	�����o�֐�
	static CRanking* Instance(void) {
		static CRanking* pInstance = new CRanking;
		return pInstance;
	}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_Time;
};

#endif // !_RANKING_H_