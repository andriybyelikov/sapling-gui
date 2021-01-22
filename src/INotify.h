#ifndef NOTIFY
#define NOTIFY

class INotify
{
public:
	virtual void Notify(wxString message) = 0;
};

#endif
