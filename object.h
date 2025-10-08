//****************************************************************
//
// �|���S������[Object.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _OBJECT_H_
#define _OBJECT_H_

// �C���N���[�h
#include "main.h"

// �N���X�錾
class CObject
{
public:
	// �K��l��ݒ�
	struct Config {
		static constexpr int Priority = 8;	// �`�揇�̍ő吔
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CObject(int Priority = 3);
	virtual ~CObject();

	// �����o�֐�
	// �Z�b�^�[
	// �Q�b�^�[
	int GetPriority(void) { return m_nPriority; };
	bool GetDeathFlag(void) { return m_bDeath; };
	CObject* GetNext(void) { return m_pNext; };

	// �������z�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// �ÓI�����o�֐�
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void Delete(const int PriorityCount);
	// �Q�b�^�[
	static int GetObjectNum(void) { return m_nNumAll; };
	static CObject* GetTop(const int Priority) { return m_pTop[Priority]; };
protected:
	// �����o�֐�
	void Release(void);
private:
	// �����o�ϐ�
	static CObject* m_pTop[Config::Priority];			// ��Ԑ擪�̃|�C���^
	static CObject* m_pCur[Config::Priority];			// ��Ԍ��̃|�C���^
	CObject* m_pPrev;									// �O�̃|�C���^
	CObject* m_pNext;									// ���̃|�C���^
	static int m_nNumAll;								// �I�u�W�F�N�g�̐����J�E���g
	int m_nID;											// �����̃I�u�W�F�N�g��ID
	int m_nPriority;									// �I�u�W�F�N�g�̗D�揇��
	bool m_bDeath;										// ���S�t���O
};
#endif // !_OBJECT_H_