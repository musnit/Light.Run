#include "GameObject.h"

class ContactListener 
{
public:
	ContactListener();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
protected:
private:
};