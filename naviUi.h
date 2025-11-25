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
	CNaviUI() : CObject(6), m_pFrames{}, m_pObjects{}, m_pKeys{}, m_objectTexturePaths{}, m_arrowTexturePaths{}, m_keyTexturePaths{}, m_pos{}, m_size{} {};
	~CNaviUI() {};
	static CNaviUI* Create(const char* const frameTexturePath, std::span<const char* const> objectTexturePaths, std::span<const char* const> keyTexturePaths, std::span<const char* const> arrowTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetFrameTexturePath(const char* const frameTexturePath) { m_frameTexturePath = frameTexturePath; }
	void SetObjectTexturePaths(std::span<const char* const> objectTexturePaths) { m_objectTexturePaths = objectTexturePaths; }
	void SetArrowTexturePaths(std::span<const char* const> arrowTexturePaths) { m_arrowTexturePaths = arrowTexturePaths; }
	void SetKeyTexturePaths(std::span<const char* const> keyTexturePaths) { m_keyTexturePaths = keyTexturePaths; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	static constexpr float FARAME_TO_OBJECT_SIZE_MAGNIFICATION = 0.8f;            // フレームに対する中身の大きさの倍率
	static constexpr float SIDE_OBJECT_POS_MAGNIFICATION_X = 1.38f;               // 横のオブジェクトの位置の倍率(X)
	static constexpr float SIDE_OBJECT_POS_MAGNIFICATION_Y = 0.65f;               // 横のオブジェクトの位置の倍率(Y)
	static constexpr float MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION = 0.35f; // 真ん中に対する横の大きさの倍率
	static constexpr float KEY_HEIGHT_MAGNIFICATION = 0.55f;                      // キーの高さの倍率
	static constexpr float KEY_SIZE_MAGNIFICATION = 0.5f;                         // キーの大きさの倍率

	void SetObjectUI();
	void SetKeyUI();

	std::array<CObject2D*, 3u> m_pFrames;                // UIフレーム
	std::array<CObject2D*, 3u> m_pObjects;               // UIオブジェクト
	std::array<CObject2D*, 2u> m_pKeys;                  // UIキー
	const char* m_frameTexturePath;                      // フレームのテクスチャ
	std::span<const char* const> m_objectTexturePaths;   // ナビゲーションオブジェクトのテクスチャ配列
	std::span<const char* const> m_arrowTexturePaths;    // ナビゲーションオブジェクト(やじるし)のテクスチャ配列
	std::span<const char* const> m_keyTexturePaths;      // キーのテクスチャ配列
	D3DXVECTOR3 m_pos;                                   // 基準位置
	D3DXVECTOR2 m_size;                                  // 基準サイズ
};