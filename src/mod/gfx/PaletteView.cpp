#include "PaletteView.h"

void PaletteView::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
}

wxBEGIN_EVENT_TABLE(PaletteView, wxWindow)
    EVT_PAINT(PaletteView::OnPaint)
wxEND_EVENT_TABLE()