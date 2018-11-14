#include "stdafx.h"
#include "Component.h"

IMPLEMENT_SERIAL(CComponent, CObject, 0)

CComponent::CComponent(CComponent *pParent)
{
	this->pParent = pParent;
	this->pNext = this->pPrev = nullptr;
	this->pChildHead = this->pChildTail = nullptr;
}


CComponent::~CComponent()
{
}

void CComponent::AddChild(CComponent* pComponent)
{

}

void CComponent::RemoveChild(CComponent* pComponent)
{

}

void CComponent::Serialize(CArchive& ar) {
	
	CObject::Serialize(ar);

	if (ar.IsStoring())
		ar << id << sName;
	else
		ar >> id >> sName;
}
