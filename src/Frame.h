#ifndef FRAME
#define FRAME

#include <wx/frame.h>
#include "TextEditor.h"
#include "CustomTree.h"
#include "RecentPathModel.h"
#include <wx/menu.h>
#include "INotify.h"

class Frame : public wxFrame, public INotify
{
    wxString* currentFile;
    TextEditor* textEditor;
    CustomTree* customTree;
    RecentPathModel* rpm;
    wxMenu *openRecentFileMenu;

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    void OnSaveFileAs(wxCommandEvent& event);
    void OnOpenRecentFile(wxCommandEvent &event);

    void OpenFile(wxString file);
    void SaveFile(wxString file);
    void UpdateRecentFiles();
    void Notify(wxString message);

    void ZoomIn(wxCommandEvent &event);
    void ZoomOut(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
public:
    Frame(wxString file);
};

#endif
