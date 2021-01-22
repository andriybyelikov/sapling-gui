#include "TextEditor.h"
#include "config.h"
#include <iostream>

TextEditor::TextEditor(wxWindow* parent)
: wxRichTextCtrl(parent, wxID_ANY)
{
sub=nullptr;
	wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	font.SetFaceName(CONFIG_FONTNAME);
    SetFont(font);
    SetDefaultStyle(*wxBLACK);
    Bind(wxEVT_TEXT, &TextEditor::OnTextChanged, this);
}

void TextEditor::SetSub(INotify *sub){this->sub=sub;}

void TextEditor::OnTextChanged(wxCommandEvent& event)
{
	if (sub != nullptr) sub->Notify(IsModified()?"*":"");
}

void TextEditor::SetFontSize(int inc)
{
	if (GetFontScale() > 0.5)
		SetFontScale(GetFontScale() + (double) inc * 0.1, true);
}
