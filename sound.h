//*************************************
//
//　サウンド処理[sound.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _SOUND_H_
#define _SOUND_H_

// インクルード
#include "renderer.h"

// サウンドクラスを定義
class CSound
{
public:
	// サウンドの列挙型
	typedef enum
	{
		LABEL_BGM = 0,		// BGM
		LABEL_SHOT,			// 放つ音
		LABEL_STUCK,		// 刺さる音
		LABEL_WATERFALL,	// 滝
		LABEL_MOVESELECT,	// 選択肢を移動させたとき
		LABEL_MOVEGROUND,	// 地面上を移動したとき
		LABEL_MOVEGRASS,	// 叢を移動したとき
		LABEL_MOVEWATER,	// 水の上を移動したとき
		LABEL_ENTER,		// 決定音
		LABEL_MAX			// 最大値
	}LABEL;

	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	// コンストラクタ・デストラクタ
	CSound();
	~CSound();

	// メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	// 静的メンバ関数
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// セッター
	HRESULT Play(LABEL label);
	HRESULT Play(LABEL label, const float Distance, const float Range);
	HRESULT SetVolume(LABEL label, const float Distance, const float Range);
	void Stop(LABEL label);
	void Stop(void);

	// ゲッター
	bool IsPlay(LABEL label);
private:
	// 静的メンバ変数
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];				// サウンドの情報
	// メンバ変数
	IXAudio2* m_pXAudio2 = NULL;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;		// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX] = {};	// ソースボイス
	BYTE* m_apDataAudio[LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// オーディオデータサイズ
};
#endif // !_SOUND_H_