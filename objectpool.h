#include "object.h"

class CObjectPool : public CObject
{
public:
	CObjectPool();
	~CObjectPool();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};