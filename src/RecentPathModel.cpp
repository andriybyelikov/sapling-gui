#include "RecentPathModel.h"
#include <iostream>
#include <fstream>
#include "config.h"
#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

RecentPathModel::RecentPathModel()
{
    wxStandardPaths &sp = wxStandardPaths::Get();
    first = last = 0;
    wxString cf(wxStandardPaths::Get().GetUserConfigDir() + "/" + CONFIG_TITLE + "/" + CONFIG_RECENT_FILES_HISTORY);
    if (wxFileExists(cf)) {
        std::ifstream his(cf);
        if (his.is_open()) {
            std::string s;
            while (last < LIM && std::getline(his, s))
                list[last++] = wxString(s);
            his.close();
        }
    } else {
        if (!wxDirExists(wxStandardPaths::Get().GetUserConfigDir() + "/" + CONFIG_TITLE))
            wxMkdir(wxStandardPaths::Get().GetUserConfigDir() + "/" + CONFIG_TITLE);
        std::ofstream his(cf);
        his.close();
    }
}

void RecentPathModel::flush()
{
    wxStandardPaths &sp = wxStandardPaths::Get();
wxFileName fn(sp.GetExecutablePath());
    wxString cf(wxStandardPaths::Get().GetUserConfigDir() + "/" + CONFIG_TITLE + "/" + CONFIG_RECENT_FILES_HISTORY);
    std::ofstream his(cf);
    int i;
    for (i = 0; i < getCount() - 1; i++) {
        his << list[(first + i) % LIM] << "\n";
    }
    if (getCount() > 0)
        his << list[(first + i) % LIM]; 
    his.close();
}

void RecentPathModel::add(wxString path)
{
    if (--first < 0)
        first += LIM;
    if (first == last) {
        --last;
        if (last < 0)
            last += LIM;
    }
    list[first] = path;
    flush();
}

void RecentPathModel::remove(wxString path)
{
    for (int i = first; i != last; i = (i + 1) % LIM) {
        if (list[i].Cmp(path) == 0) {
            if (i == first) {
                first++;
                if (first >= LIM)
                    first -= LIM;
            } else if (i == last) {
                last--;
                if (last < 0)
                    last += LIM;
            } else {

                for (int j = i; j != (first - 1 < 0 ? first - 1 + LIM : first - 1); j = (j + 1) % LIM) {
                    list[j] = list[(j + 1) % LIM];
                }
                last--;
                if (last < 0)
                    last += LIM;
            }
            flush();
            return;
        }
    }
}

int RecentPathModel::getCount() {
    return last + (last < first ? LIM : 0) - first;
}

wxString RecentPathModel::get(int i) {
    return list[(first + i) % LIM];
}
