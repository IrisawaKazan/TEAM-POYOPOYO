//****************************************************************
//
// �|�[�Y�̏���[pause.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _PAUSE_H_
#define _PAUSE_H_

// �C���N���[�h
#include "object2D.h"

// �|�[�Y�N���X���`
class CPause : public CObject2D
{
public:
	// ���j���[�̗񋓌^
	typedef enum
	{
		CONTINUE = 0,	// �R���e�B�j���[(������)
		RETRY,			// ���g���C(��蒼��)
		QUIT,			// �N�C�b�g(��߂�)
		MAX
	}Menu;

	// �|�[�Y�̋K��l��ݒ�
	struct Config {
		static const D3DXCOLOR SelectColor;		// �I��ł���Ƃ��̐F
		static const D3DXCOLOR DefoultColor;	// �ʏ�̐F
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CPause();
	~CPause();

	// ���z�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// �ÓI�����o�֐�
	// ����
	static CPause* Create(D3DXVECTOR3 Pos, Menu Menu);
protected:
	int m_nAnimCounter;		// �J�E���^�[
private:
};

// �R���e�j���[�N���X���`
class CContinue : public CPause
{
public:
	// �R���e�j���[���j���[�̋K��l��ݒ�
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\continue.png" ;		// ���j���[�̃p�X
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CContinue();
	~CContinue();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};

// ���g���C���j���[�N���X���`���`
class CRetry : public CPause
{
public:
	// �R���e�j���[���j���[�̋K��l��ݒ�
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\retry.png";		// ���j���[�̃p�X
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CRetry();
	~CRetry();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};

// �N�C�b�g���j���[�N���X���`���`
class CQuit : public CPause
{
public:
	// �R���e�j���[���j���[�̋K��l��ݒ�
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\quit.png";		// ���j���[�̃p�X
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CQuit();
	~CQuit();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};
#endif // !_PAUSE_H_