//****************************************************************
//
// ブロックのファクトリー処理[abstractblock.h]
// Author Kensaku Hatori
//
//****************************************************************

#ifndef _ABSTRACTBLOCK_H_
#define _ABSTRACTBLOCK_H_

class CBlock;

class CAbsBlock
{
public:
	~CAbsBlock() {};
	static CBlock* CreateBlock(D3DXVECTOR3 Pos, D3DXQUATERNION Quat, D3DXVECTOR3 Scale, std::string Path);
private:
	CAbsBlock() {};

};
#endif // !_ABSTRACTBLOCK_H_