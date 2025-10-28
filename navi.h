//------------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

class CNaviMarker;
class CNaviObject;
//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̃N���X
//--------------------------------
class CNavi
{
public:
	// �i�r�Q�[�V�����I�u�W�F�N�g���X�g
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

	// �i�r�Q�[�V�����I�u�W�F�N�g�̃^�C�v
	enum class TYPE : unsigned char
	{
		None,
		Arrow,
		//Climb,
		//Attack,
		Max
	};

	// ���C�L���X�g�̑ΏۃI�u�W�F�N�g�����i�[����\����
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

	static CNavi* GetInstance()
	{
		static CNavi instance;
		return &instance;
	}

private:
	CNavi() : m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f }, m_pos{ 0.0f,0.0f,0.0f }, m_clickPos{ 0.0f,0.0f,0.0f }, m_aRayCastTarget{}, m_pMarker{}, m_apObject{}, m_list{} {};
	~CNavi() {};

	D3DXVECTOR2 ConvertMouseToScreen(D3DXVECTOR2 mousePos);
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);
	D3DXVECTOR3 MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld);

	static constexpr float HEIGHT = 0.05f;                                             // �n�ʂ̍���
	static constexpr const char* MARKER_TEXTURE_PATH = "data/TEXTURE/MagicCircle.png"; // �}�[�J�[�̃e�N�X�`���p�X
	static const D3DXVECTOR3 MARKER_OFFSET;                                            // �}�[�J�[�̃I�t�Z�b�g�ʒu
	static const D3DXVECTOR2 MARKER_SIZE;                                              // �}�[�J�[�̃T�C�Y

	D3DXVECTOR3 m_RayPos; // ���C�̎n�_
	D3DXVECTOR3 m_RayDir; // ���C�̕���

	D3DXVECTOR3 m_pos;      // ���݂̈ʒu
	D3DXVECTOR3 m_clickPos; // �N���b�N�����ʒu

	std::vector<RayCastTarget> m_aRayCastTarget; // ���C�L���X�g�̑ΏۃI�u�W�F�N�g�z��

	CNaviMarker* m_pMarker; // �}�[�J�[�̃|�C���^

	std::vector<CNaviObject*> m_apObject; // �I�u�W�F�N�g�̔z��

	LIST m_list;                 // ���I�΂�Ă���I�u�W�F�N�g
};

//-----------------------
// Swap���g����vector�̗v�f�폜
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
// Swap���g����vector�̗v�f�폜
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