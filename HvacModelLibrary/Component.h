#pragma once

#include <vector>

enum EntityType {
	EntityUnknown,      /**< Unknown */
	EntityContainer		/**< Container */
};

class AFX_EXT_CLASS CComponent : public CObject
{
public:
	DECLARE_SERIAL(CComponent);

	CComponent(CComponent *pParent = nullptr);
	virtual ~CComponent();

	virtual EntityType rtti() const {
		return EntityUnknown;
	}

	unsigned long int getId() const {
		return id;
	}
	
	/*
	virtual bool isContainer() const = 0;
	virtual bool isAtomic() const = 0;
	*/

	void AddChild(CComponent* pComponent);
	void RemoveChild(CComponent* pComponent);

	// void Update(double aTemp, double aHumidity);
	
	void Serialize(CArchive& ar);

	/*
	void *operator new(size_t stSize)
	{
		return (void *) buf;
	}

	void operator delete(void *p)
	{
	}
	*/

private:
	unsigned long int id;
	CString sName;

	CComponent *pPrev, *pNext, *pParent;
	CComponent *pChildHead, *pChildTail;

	// std::vector<CComponent*> listChildren;
};

