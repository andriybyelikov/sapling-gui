#ifndef APP
#define APP

#include <wx/app.h>
#include <wx/cmdline.h>
#include "Frame.h"

class App : public wxApp
{
    Frame* frame;
public:
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
 { wxCMD_LINE_PARAM, nullptr, nullptr, "input file", wxCMD_LINE_VAL_STRING , wxCMD_LINE_PARAM_OPTIONAL},
  { wxCMD_LINE_NONE}
};

#endif
