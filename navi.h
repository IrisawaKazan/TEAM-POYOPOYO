//------------------------------------
//
// ナビゲーションマーカー [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

class CNaviMarker;
class CNaviObject;
//--------------------------------
// ナビゲーションマーカーのクラス
//--------------------------------
class CNavi
{
public:
	// ナビゲーションオブジェクトリスト
	enum class LIST : unsigned char
	{
		RightArrow,
		FrontArrow,
		LeftArrow,
		BackArrow,
		//Climb,
		//Attack,
		Max
	};

	// ナビゲーションオブジェクトのタイプ
	enum class TYPE : unsigned char
	{
		None,
		Arrow,
		//Climb,
		//Attack,
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

	void SetMarker(void);
	void RemoveMarker(void) { m_pMarker = nullptr; }

	const std::vector<CNaviObject*>& GetObjects(void) const { return m_apObject; }
	LIST GetList(void) const { return m_list; }

	void HitCheckObject();

	static CNavi* GetInstance()
	{
		static CNavi instance;
		return &instance;
	}

private:
	CNavi() : m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f }, m_pos{ 0.0f,0.0f,0.0f }, m_clickPos{ 0.0f,0.0f,0.0f }, m_aRayCastTarget{}, m_pMarker{}, m_apObject{}, m_list{}, m_pNewObject{} {};
	~CNavi() {};

	D3DXVECTOR2 ConvertMouseToScreen(D3DXVECTOR2 mousePos);
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);
	D3DXVECTOR3 MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, const float enableAngle, D3DXVECTOR3* pNor = nullptr);
	bool CheckLatent(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, float lengthSq);
	D3DXMATRIX CreateMatrixFromNormal(D3DXVECTOR3 nor);

	static constexpr float MARKER_HEIGHT = 0.1f;                                       // 地面からマーカーをオフセットする高さ
	static constexpr float OBJECT_HEIGHT = 0.05f;                                      // 地面からオブジェクトをオフセットする高さ
	static constexpr const char* MARKER_TEXTURE_PATH = "data/TEXTURE/MagicCircle.png"; // マーカーのテクスチャパス
	static const float ENABLE_ANGLE;                                                   // おけるオブジェクト角度の閾値(どこまでを床としますか?)
	static const D3DXVECTOR3 MARKER_OFFSET;                                            // マーカーのオフセット位置
	static const D3DXVECTOR2 MARKER_SIZE;                                              // マーカーのサイズ

	D3DXVECTOR3 m_RayPos; // レイの始点
	D3DXVECTOR3 m_RayDir; // レイの方向

	D3DXVECTOR3 m_pos;      // 現在の位置
	D3DXVECTOR3 m_clickPos; // クリックした位置

	std::vector<RayCastTarget> m_aRayCastTarget; // レイキャストの対象オブジェクト配列

	CNaviMarker* m_pMarker; // マーカーのポインタ

	std::vector<CNaviObject*> m_apObject; // オブジェクトの配列
	CNaviObject* m_pNewObject;            // 新しいオブジェクト

	LIST m_list;                 // 今選ばれているオブジェクト
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