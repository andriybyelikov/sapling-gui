#include "App.h"
#include <iostream>
#include <wx/stdpaths.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    return true;
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc (g_cmdLineDesc);
	    // must refuse '/' as parameter starter or cannot use "/path" style paths
	    parser.SetSwitchChars (wxT("-"));
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
    wxString file;
    if (parser.GetParamCount() > 0)
	file = parser.GetParam(0);
    else file = "";
    wxStandardPaths::Get().SetFileLayout(wxStandardPathsBase::FileLayout_XDG);
    (frame = new Frame(file))->Show();
	return true;
}
