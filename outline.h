#include "shader.h"
class COutLine : public CShader
{
public:
	// デストラクタ
	~COutLine();

	// メンバ関数
	HRESULT Init(std::string Path);
	void Begin(void);
	void BeginPass(const UINT Pass = 0);
	void End(void);
	void EndPass(void);

	// セッター
	void SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, const D3DXVECTOR4 Col = { 1.0f,1.0f,1.0f,1.0f }); // ライトやマテリアルなどの設定

	// 静的メンバ関数
	static COutLine* Instance(void) {
		static std::unique_ptr<COutLine> Instance(new COutLine);
		return Instance.get();
	};
private:
	// コンストラクタ
	COutLine() = default;
};