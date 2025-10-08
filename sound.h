//*************************************
//
//�@�T�E���h����[sound.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _SOUND_H_
#define _SOUND_H_

// �C���N���[�h
#include "renderer.h"

// �T�E���h�N���X���`
class CSound
{
public:
	// �T�E���h�̗񋓌^
	typedef enum
	{
		LABEL_BGM = 0,		// BGM
		LABEL_SHOT,			// ����
		LABEL_STUCK,		// �h���鉹
		LABEL_WATERFALL,	// ��
		LABEL_MOVESELECT,	// �I�������ړ��������Ƃ�
		LABEL_MOVEGROUND,	// �n�ʏ���ړ������Ƃ�
		LABEL_MOVEGRASS,	// �p���ړ������Ƃ�
		LABEL_MOVEWATER,	// ���̏���ړ������Ƃ�
		LABEL_ENTER,		// ���艹
		LABEL_MAX			// �ő�l
	}LABEL;

	typedef struct
	{
		const char* pFilename;	// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
	} SOUNDINFO;

	// �R���X�g���N�^�E�f�X�g���N�^
	CSound();
	~CSound();

	// �����o�֐�
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	// �ÓI�����o�֐�
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �Z�b�^�[
	HRESULT Play(LABEL label);
	HRESULT Play(LABEL label, const float Distance, const float Range);
	HRESULT SetVolume(LABEL label, const float Distance, const float Range);
	void Stop(LABEL label);
	void Stop(void);

	// �Q�b�^�[
	bool IsPlay(LABEL label);
private:
	// �ÓI�����o�ϐ�
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];				// �T�E���h�̏��
	// �����o�ϐ�
	IXAudio2* m_pXAudio2 = NULL;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;		// �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
};
#endif // !_SOUND_H_