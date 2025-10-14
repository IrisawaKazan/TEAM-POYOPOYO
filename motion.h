//*************************************
//
//�@���[�V��������[motion.h]
//�@Author:Hatori Kensaku
//
//*************************************
// ��d�C���N���[�h�h�~
#ifndef _MOTION_H_
#define _MOTION_H_
// �C���N���[�h
#include "model.h"
#include "main.h"
//#include "collision.h"

// ���[�V�����N���X���`
class CMotion
{
public:
	// �p�[�c�̃L�[���
	struct KEY
	{
		// �ڕW�̈ʒu
		float fPosX;
		float fPosY;
		float fPosZ;
		// �ڕW�̌���
		float fRotX;
		float fRotY;
		float fRotZ;
	};
	// ���[�V�����̃L�[���
	struct KEYINFO
	{
		int nFrame;				// �t���[��
		std::vector<KEY> pKey;	// �L�[���
	};
	// ���[�V�������
	struct MOTIONINFO
	{
		bool bLoop;						// ���[�v���邩�ǂ���
		int nNumKey;					// �L�[�̑���
		std::vector<KEYINFO> pKeyInfo;	// �L�[�̏��
	};
	// �R���X�g���N�^�E�f�X�g���N�^
	CMotion();
	~CMotion();
	// �����o�֐�
	void Uninit(void);
	void Update(std::vector<CModel*> &pModel);
	void UpdateKey(void);
	void UpdateCurrentMotion(CModel* pModel);
	void UpdateBlendMotion(CModel* pModel);
	// �������̊֐���
	bool isUpdateCurrentMotion(void) { return m_bFinishMotion == false && m_bFirstBlend == false; };
	bool isEvent(const int StartFrame, const int EndFrame);
	// ���[�_�[
	void LoadCharactorInfo(std::ifstream& ifs, std::vector <CModel*> pModel);
	void LoadMotionInfo(std::ifstream& ifs, int* nMotionCount);
	// �Z�b�^�[
	void SetNumModel(int nNumModel) { m_nNumModel = nNumModel; };
	void SetNumMotion(int nNumMotion) { m_nNumMotion = nNumMotion; };
	void SetMotion(int nMotion,const bool isKeep);
	void SetisKeep(const bool bKeep) { m_bKeep = bKeep; };
	// �Q�b�^�[
	int GetNumModel(void) { return m_nNumModel; };
	int GetNumMotion(void) { return m_nNumMotion; };
	int GetMotion(void) { return m_nMotionType; };
	int GetBlendMotion(void) { return m_nBlendMotionType; };
	int GetFirstBlend(void) { return m_bFirstBlend; };
	int GetKey(void) { return m_nKey; };
	int GetNextKey(void) { return m_nNextKey; };
	int GetBlendKey(void) { return m_nKeyBlend; };
	int GetBlendNextKey(void) { return m_nNextKeyBlend; };
	int GetMotionCount(void) { return m_nCounterMotion; };
	int GetMotionBlendCount(void) { return m_nCounterBlend; };
	int GetMotionFrame(void) { return m_nCounterMotion; };
	bool GetFinishMotion(void) { return m_bFinishMotion; };
	// �ÓI�����o�֐�
	static CMotion* LoadMotionFile(const char* cFileName, std::vector <CModel*> &pModel);
private:
	// �����o�ϐ�
	std::vector<MOTIONINFO> m_MotionInfo;										// ���[�V�����̏��
	int m_nAllFrame;															// �S�̃t���[��
	int m_nMotionType, m_nBlendMotionType;										// ���̃��[�V�����̎�ށA�u�����h��̃��[�V�����̎��
	int m_nNumKey, m_nKey, m_nCounterMotion, m_nNextKey;						// �L�[�̏��ƃ��[�V�����J�E���^�[
	int m_nNumModel;															// ���f���̐�
	int m_nNumMotion;															// ���[�V�����̐�
	int m_nNumKeyBlend, m_nKeyBlend, m_nCounterMotionBlend, m_nNextKeyBlend;	// �u�����h��̃L�[�̏��ƃJ�E���^�[
	int m_nCounterBlend, m_nFrameBlend;											// �u�����h��̃��[�V�����̃J�E���^�[�ƃu�����h�̃J�E���^�[
	bool m_bBlendMotion;														// �u�����h���邩�ǂ���
	bool m_bFinishMotion;														// ���[�V�������I��������ǂ���
	bool m_bFirstBlend;															// �ŏ��̃u�����h
	bool m_bKeep;																// �Ō�̃L�[�ňێ����邩�ǂ���(�㔼�g�p)

};
#endif // !_MOTION_H_

