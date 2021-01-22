#include "config.h"
#include "Frame.h"
#include <wx/menuitem.h>
#include <wx/msgdlg.h>
#include <wx/splitter.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <iostream>
#include <fstream>
#include <wx/icon.h>
#include <wx/stdpaths.h>
#include <wx/accel.h>

enum
{
    ID_OPENRECENT_FILE = wxID_HIGHEST,
    ID_NEWPROJECT = ID_OPENRECENT_FILE + 10,
    ID_ZOOMIN, ID_ZOOMOUT
};

Frame::Frame(wxString file) : wxFrame(NULL, wxID_ANY, CONFIG_TITLE)
{
    currentFile = nullptr;
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;
    openRecentFileMenu = new wxMenu;
    rpm = new RecentPathModel();
    fileMenu->Append(wxID_NEW);
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_OPEN);
    fileMenu->AppendSubMenu(openRecentFileMenu, "Open Recent");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_SAVE);
    fileMenu->Append(wxID_SAVEAS);
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);
    helpMenu->Append(wxID_ABOUT);
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

	wxStandardPaths &sp = wxStandardPaths::Get();
	wxFileName fn(sp.GetExecutablePath());
    wxString rf(wxStandardPaths::Get().GetResourcesDir() + "/sapling-gui.png");
    wxIcon icon;
    icon.LoadFile(rf);
    SetIcon(icon);
    //wxSplitterWindow* splitter = new wxSplitterWindow(this);
    this->textEditor = new TextEditor(this);
    //this->customTree = new CustomTree(splitter);
    //splitter->SplitVertically(customTree, textEditor);
    if (file.Cmp("") != 0)
        OpenFile(file);
    else
        UpdateRecentFiles();
    textEditor->SetSub(this);
	// accel
	wxAcceleratorEntry entries[2];
	entries[0].Set(wxACCEL_CTRL, (int) '+', ID_ZOOMIN);
	entries[1].Set(wxACCEL_CTRL, (int) '-', ID_ZOOMOUT);
	wxAcceleratorTable accel(2, entries);
	SetAcceleratorTable(accel);
}

void Frame::UpdateRecentFiles()
{
    // clear
    int x = 0;
    while (openRecentFileMenu->GetMenuItemCount() > 0) {
        openRecentFileMenu->Destroy(openRecentFileMenu->FindItemByPosition(0));
    }
    // reload
    if (rpm->getCount() == 0)
	openRecentFileMenu->Append(wxID_NONE, "No recent files");
    else
        for (int i = 0; i < rpm->getCount(); i++) {
            openRecentFileMenu->Append(ID_OPENRECENT_FILE + i, wxString(rpm->get(i)));
	}
}

void Frame::OnOpenRecentFile(wxCommandEvent &event)
{
    wxString file = (openRecentFileMenu->FindItem(event.GetId(), NULL))->GetItemLabel();
    OpenFile(file);
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(CONFIG_ABOUT_STRING, "About", wxOK | wxICON_AUTH_NEEDED,
    this);
}

void Frame::OnNewFile(wxCommandEvent& WXUNUSED(event))
{
	currentFile = nullptr;
	SetTitle("Untitled - " + CONFIG_TITLE);
	textEditor->Clear();
}

void Frame::OpenFile(wxString file)
{
    SetTitle(file + " - " + CONFIG_TITLE);
    wxFileName fn(file);
fn.MakeAbsolute();
    // memory leak
    currentFile = new wxString(fn.GetFullPath());
    char buf[16384];
    if (file.Cmp("")) {
    	std::ifstream f(*currentFile);
    	if (f.is_open()) {
            std::string s;
	    do {
	        f.getline(buf, 16384);
	        s += std::string(buf);
	        s+="\n"; 
	    } while (f);
            f.close();
            rpm->remove(*currentFile);
            rpm->add(*currentFile);
            UpdateRecentFiles();
	      textEditor->ChangeValue(s);
        }
    }
}

void Frame::SaveFile(wxString file)
{
    SetTitle(file + " - " + CONFIG_TITLE);
     wxFileName fn(file);
fn.MakeAbsolute();
    // memory leak
    currentFile = new wxString(fn.GetFullPath());
    std::ofstream f(file);
    if (f.is_open()) {
        f << textEditor->GetValue();
        f.close();
        rpm->remove(*currentFile);
        rpm->add(*currentFile);
        UpdateRecentFiles();
    }
}

void Frame::OnOpenFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fd(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (fd.ShowModal() == wxID_OK)
	OpenFile(fd.GetPath());
}

void Frame::OnSaveFile(wxCommandEvent& event)
{
    if (currentFile == nullptr)
        OnSaveFileAs(event);
    else
        SaveFile(*currentFile);
}

void Frame::OnSaveFileAs(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fd(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (fd.ShowModal() == wxID_OK)
	SaveFile(fd.GetPath());
}

void Frame::Notify(wxString message)
{
	SetTitle(message + (currentFile != nullptr ? *currentFile : "Untitled") + " - " + CONFIG_TITLE);
}

void Frame::ZoomIn(wxCommandEvent &) { textEditor->SetFontSize(1); }

void Frame::ZoomOut(wxCommandEvent &) { textEditor->SetFontSize(-1); }

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(wxID_NEW, Frame::OnNewFile)
    EVT_MENU(wxID_OPEN, Frame::OnOpenFile)
    EVT_MENU(wxID_SAVE, Frame::OnSaveFile)
    EVT_MENU(wxID_SAVEAS, Frame::OnSaveFileAs)
    EVT_MENU(wxID_EXIT, Frame::OnQuit)
    EVT_MENU(wxID_ABOUT, Frame::OnAbout)
    EVT_MENU_RANGE(ID_OPENRECENT_FILE, ID_OPENRECENT_FILE + 10, Frame::OnOpenRecentFile)
    EVT_MENU(ID_ZOOMIN, Frame::ZoomIn)
    EVT_MENU(ID_ZOOMOUT, Frame::ZoomOut)
wxEND_EVENT_TABLE()
