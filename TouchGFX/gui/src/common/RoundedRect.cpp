/******************************************************************************
* Copyright (c) 2018(-2025) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.26.0 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

#include <touchgfx/hal/HAL.hpp>
#include <gui/common/RoundedRect.hpp>
#ifndef SIMULATOR
#include <touchgfx_nema/HALGPU2D.hpp>
#include <touchgfx_nema/LCDGPU2D.hpp>
#include "nema_graphics.h"
#else
#endif
namespace touchgfx
{

Rect RoundedRect::getSolidRect() const
{
    return Rect();
}

void RoundedRect::draw(const Rect& area) const
{
    if (alpha == 0 || rect.isEmpty())
    {
        return;
    }
    Rect dirty = area;
    translateRectToAbsolute(dirty);
#ifdef SIMULATOR
    // On simulator, we just draw a normal rectangle 
    HAL::lcd().fillRect(dirty, color, alpha);
#else
    Rect meAbs = getAbsoluteRect();
    HALGPU2D* hal = static_cast<HALGPU2D*>(HAL::getInstance());
    LCDGPU2D* lcd = static_cast<LCDGPU2D*>(&HAL::lcd());
    hal->bindExecBuffer();
    lcd->bindFrameBufferTexture();
    nema_set_clip(dirty.x, dirty.y, dirty.width, dirty.height);
    nema_set_blend_fill(NEMA_BL_SIMPLE | NEMA_BLOP_MODULATE_A);
    nema_set_const_color(nema_rgba(0, 0, 0, alpha));
    const uint8_t r = Color::getRed(color);
    const uint8_t g = Color::getGreen(color);
    const uint8_t b = Color::getBlue(color);    
    nema_fill_rounded_rect(meAbs.x, meAbs.y, meAbs.width, meAbs.height, cornerRadius, nema_rgba(r, g, b, 255));
    if (shouldDrawBorder)
    {
        nema_draw_rounded_rect(meAbs.x, meAbs.y, meAbs.width, meAbs.height, cornerRadius, nema_rgba(0, 0, 0, 255));
    }
    if (nema_reg_read(0xFC) == 0)
    {
        hal->submitExecBuffer();
    }
    else
    {
        hal->submitExecBufferOnEndframe();
    }
#endif
}
} // namespace touchgfx
