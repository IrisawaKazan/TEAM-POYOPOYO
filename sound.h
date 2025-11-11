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
		LABEL_TITLE_BGM = 0,		// タイトルBGM
		LABEL_GAME_BGM,				// ゲームBGM
		LABEL_RANKING_BGM,			// ランキングBGM

		LABEL_SELECT_SE,			// 選択移動SE
		LABEL_DECISION_SE,			// 選択決定SE
		LABEL_STAMP_SE,				// 指示を配置SE
		LABEL_CHANGE_SE,			// 指示を変更SE
		LABEL_PAUSE_SE,				// ポーズSE
		LABEL_FALLING_SE,			// 落下SE

		LABEL_MAX					// 最大値
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