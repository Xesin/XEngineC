#include "stdafx.h"
#include "Rect.h"
#include "Renderer.h"

void Rect::OnRender(Renderer* renderer) const
{
	renderer->RenderRect(50, 50, 100, 100, D2D1::ColorF(D2D1::ColorF::Aquamarine));
}
