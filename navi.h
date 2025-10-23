//------------------------------------
//
// ナビゲーションマーカー [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

class CNaviMarker;
class CArrow;
//--------------------------------
// ナビゲーションマーカーのクラス
//--------------------------------
class CNavi
{
public:
	// 矢印の向き
	enum class ARROW_DIRECTION : unsigned char
	{
		Left,
		Front,
		Back,
		Max
	};

	// レイキャストの対象オブジェクト情報を格納する構造体
	struct RayCastTarget
	{
		LPD3DXMESH pMesh;
		D3DXMATRIX mtxWorld;
	};

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	D3DXVECTOR3 GetClickPos(void) const { return m_clickPos; }

	void RegisterRayCastObject(LPD3DXMESH pMesh, const D3DXMATRIX& mtxWorld);
	void CalculateIntersection(void);

	void ResetMarker(void);

	const std::vector<CArrow*>& GetArrow(void) const { return m_apArrow; }
	ARROW_DIRECTION GetDirection(void) const { return m_direction; }

	static CNavi* GetInstance()
	{
		static CNavi instance;
		return &instance;
	}

private:
	CNavi() : m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f }, m_pos{ 0.0f,0.0f,0.0f }, m_clickPos{ 0.0f,0.0f,0.0f }, m_aRayCastTarget{}, m_pMarker{}, m_apArrow{}, m_direction{} {};
	~CNavi() {};

	D3DXVECTOR2 ConvertMouseToScreen(D3DXVECTOR2 mousePos);
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);
	D3DXVECTOR3 MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld);

	static constexpr float HEIGHT = 0.05f;                                             // 地面の高さ
	static constexpr const char* MARKER_TEXTURE_PATH = "data/TEXTURE/MagicCircle.png"; // ナビマーカーのテクスチャパス
	static const D3DXVECTOR3 MARKER_OFFSET;                                            // ナビマーカーのオフセット位置
	static const D3DXVECTOR2 MARKER_SIZE;                                              // ナビマーカーのサイズ

	D3DXVECTOR3 m_RayPos; // レイの始点
	D3DXVECTOR3 m_RayDir; // レイの方向

	D3DXVECTOR3 m_pos;      // 現在の位置
	D3DXVECTOR3 m_clickPos; // クリックした位置

	std::vector<RayCastTarget> m_aRayCastTarget; // レイキャストの対象オブジェクト配列

	CNaviMarker* m_pMarker;          // ナビマーカーのポインタ

	std::vector<CArrow*> m_apArrow; // 矢印の配列
	ARROW_DIRECTION m_direction;    // 矢印の向き
};

//-----------------------
// Swapを使ったvectorの要素削除
//-----------------------
template<typename T>
void SwapRemove(std::vector<T>& vec, size_t index)
{
	if (index < vec.size())
	{
		std::swap(vec[index], vec.back());
		vec.pop_back();
	}
}

//-----------------------
// Swapを使ったvectorの要素削除
//-----------------------
template<typename T>
void SwapRemove(std::vector<T>& vec, T element)
{
	auto it = std::find(vec.begin(), vec.end(), element);
	if (it != vec.end())
	{
		std::swap(*it, vec.back());
		vec.pop_back();
	}
}