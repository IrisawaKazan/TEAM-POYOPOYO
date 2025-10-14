//************************************************************
//
//�@�K�w�\��������L�����N�^�[�̏���[modelcharactor.h]
//�@Author:Hatori Kensaku
//
//************************************************************
// ��d�C���N���[�h�h�~
#ifndef _CHARCTER_H_
#define _CHARCTER_H_
// �C���N���[�h
#include "object.h"
#include "motion.h"
#include "model.h"
// �I�u�W�F�N�g�̔h���N���X���`[�K�w�\��������L�����N�^�[]
class CModelCharacter : public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CModelCharacter(int Priority = 3);
	~CModelCharacter();
	// �����o�֐�
	HRESULT Init(const char* MotionFilePath,float Speed);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Q�b�^�[
	CMotion* GetMotionInfo(void) { return m_MotionInfo; };
	CModel* GetModel(const int Indx) { return m_apModel[Indx]; };
	std::vector<CModel*>& GetvModel(void) { return m_apModel; };
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3* GetpPos(void) { return &m_Pos; };
	D3DXVECTOR3 GetPosOld(void) { return m_OldPos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXVECTOR3 GetRotDest(void) { return m_RotDest; };
	D3DXVECTOR3 GetMove(void) { return m_Move; };
	int GetState(void) { return m_State; };
	int GetMotion(void) { return m_Motion; };
	float GetSpeed(void) { return m_fSpeed; };
	bool GetEnable(void) { return m_Enable; };
	// �Z�b�^�[
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetRotDest(D3DXVECTOR3 RotDest) { m_RotDest = RotDest; };
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; };
	void SetmtxRotChest(D3DXMATRIX mtxRot) { m_mtxRotChest = mtxRot; };
	void SetState(int State) { m_State = State; };
	void SetSpeed(float Speed) { m_fSpeed = Speed; };
	void OnLanding(void) { m_isLanding = true; };
	void OffLanding(void) { m_isLanding = false; };
	void SetEnable(bool Enable) { m_Enable = Enable; };
	void ToggleEnable(void) { m_Enable = !m_Enable; };
private:
	// �����o�ϐ�
	std::vector<CModel*> m_apModel;							// ���f���̏����i�[����ϐ�
	CMotion* m_MotionInfo;									// ���[�V�����̏��
	D3DXMATRIX m_mtxWorld;									// �}�g���b�N�X
	D3DXMATRIX m_mtxRotChest;								// ���̂̉�]�s��
	D3DXVECTOR3 m_Pos;										// �ʒu
	D3DXVECTOR3 m_OldPos;									// �ʒu�t���[���O�̈ʒu
	D3DXVECTOR3 m_Rot;										// ����
	D3DXVECTOR3 m_RotDest;									// �ڕW�̌���
	D3DXVECTOR3 m_Move;										// �v���C���[�̈ړ���
	int m_State;											// �v���C���[�̃X�e�[�^�X
	int m_Motion;											// ���݂̃��[�V����
	float m_fSpeed;											// �v���C���[�̈ړ����x
	bool m_isLanding;										// ���n���Ă��邩�ǂ���
	bool m_Enable;											// �L�����ǂ���
};
#endif // !_MODELCHARACTOR_H_














