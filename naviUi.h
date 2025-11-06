//------------------------------------
//
// ナビゲーションUI [naviUi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once
#include <array>
#include "object.h"

class CObject2D;
//--------------------------------
// ナビゲーションUIのクラス
//--------------------------------
class CNaviUI : public CObject
{
public:
	CNaviUI() : CObject(7), m_pFrames{}, m_pObjects{}, m_objectTexturePaths{}, m_pos{}, m_size{} {};
	~CNaviUI() {};
	static CNaviUI* Create(const char* frameTexturePath, std::vector<const char*> objectTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetFrameTexturePath(const char* frameTexturePath) { m_frameTexturePath = frameTexturePath; }
	void SetObjectTexturePaths(std::vector<const char*> objectTexturePaths) { m_objectTexturePaths = objectTexturePaths; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	void SetObjectUI(void);

	std::array<CObject2D*, 3u> m_pFrames;          // UIフレーム
	std::array<CObject2D*, 3u> m_pObjects;         // 2Dオブジェクト配列
	const char* m_frameTexturePath;                // フレームのテクスチャ
	std::vector<const char*> m_objectTexturePaths; // ナビゲーションオブジェクトのテクスチャ配列
	D3DXVECTOR3 m_pos;                             // 基準位置
	D3DXVECTOR2 m_size;                            // 基準サイズ

};