#pragma once

#include <vector>

/*
class CSubject
{
	// Lets keep a track of all the components we have observing
	std::vector<CComponent*> listComponent;

public:
	void Attach(CComponent *pComponent)
	{
		listComponent.push_back(pComponent);
	}

	void Detach(CComponent *pComponent)
	{
		listComponent.erase(std::remove(listComponent.begin(), listComponent.end(), pComponent), listComponent.end());
	}

	void Notify(.....)
	{
		for (std::vector<pComponent*>::const_iterator iter = listComponent.begin(); iter != listComponent.end(); ++iter)
		{
			if (*iter != 0)
			{
				(*iter)->Update(price);
			}
		}
	}
};

class CObserver
{
public:
	virtual void Update( .... ) = 0;
};
*/

class CComponent
{
public:
	CComponent();
	virtual ~CComponent();

	// void Update(double aTemp, double aHumidity);

private:
	CComponent *prev, *next;
	std::vector<CComponent*> listChildren;
};

