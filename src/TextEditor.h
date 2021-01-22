#include <wx/richtext/richtextctrl.h>
#include <wx/window.h>
#include "INotify.h"

class TextEditor : public wxRichTextCtrl
{
	INotify *sub;
      void OnTextChanged(wxCommandEvent& event);
public:
    TextEditor(wxWindow* parent);
	void SetSub(INotify *sub);
    void SetFontSize(int inc);
};
