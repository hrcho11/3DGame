#pragma once

#include "cAction.h"

class cGameObject : public cObject
{
protected:
	D3DXMATRIXA16	m_matWorld;
	D3DXVECTOR3		m_vPrevPosition;
	
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
	virtual void Render(D3DXMATRIXA16* pmat);
};

