#ifndef RECENT_PATH_MODEL
#define RECENT_PATH_MODEL
#include <wx/string.h>
#define LIM 10

class RecentPathModel
{
    wxString list[LIM];
    int first, last;
    void flush();
public:
    RecentPathModel();
    void add(wxString path);
    void remove(wxString path);
    int getCount();
    wxString get(int i);
};

#endif
