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

#ifndef TOUCHGFX_ROUNDEDRECT_HPP
#define TOUCHGFX_ROUNDEDRECT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
    class RoundedRect : public Widget
    {
        public:
        /** Construct a new RoundedRect with a default alpha value of 255 (solid) */
        RoundedRect()
            : Widget(), alpha(255), color(0), cornerRadius(0)
        {
        }       

        virtual Rect getSolidRect() const;

        /**
         * Sets the color of the Rectangle.
         *
         * @param  newColor The color of the rectangle.
         *
         * @see getColor, Color::getColorFromRGB
         */
        void setColor(colortype newColor)
        {
            color = newColor;
        }

        /**
         * Sets the border color of the Rectangle.
         *
         * @param  newBorderColor The color of the border.
         *
         * @see getColor, Color::getColorFromRGB
         */
        void setBorderColor(colortype newBorderColor)
        {
            borderColor = newBorderColor;
        }

        /**
         * Sets whether the border should be drawn.
         *
         * @param  draw true if the border should be drawn, false otherwise.
         */
        void drawBorder(bool draw)
        {
            shouldDrawBorder = draw;
        }

        /**
         * Gets the current color of the Rectangle.
         *
         * @return The current color of the rectangle.
         *
         * @see setColor, Color::getRed, Color::getGreen, Color::getRed
         */
        colortype getColor() const
        {
            return color;
        }

        /**
         * Gets the border color of the Rectangle.
         *
         * @return The border color of the rectangle.
         *
         * @see setColor, Color::getRed, Color::getGreen, Color::getRed
         */
        colortype getBorderColor() const
        {
            return borderColor;
        }

        /**
         * @copydoc Image::setAlpha
         */
        void setAlpha(uint8_t newAlpha)
        {
            alpha = newAlpha;
        }

        /**
         * @copydoc Image::getAlpha
         */
        uint8_t getAlpha() const
        {
            return alpha;
        }

        /**
         * Sets the corner radius of the RoundedRect.
         *
         * @param  newRadius The radius of the corners.
         *
         * @see getCornerRadius
         */
        virtual void setCornerRadius(int16_t newRadius)
        {
            cornerRadius = newRadius;
        }
        /**
         * Gets the current corner radius of the RoundedRect.
         *
         * @return The current radius of the corners.
         *
         * @see setCornerRadius
         */
        int16_t getCornerRadius() const
        {
            return cornerRadius;
        }

        virtual void draw(const Rect& area) const;

        protected:
        uint8_t alpha;   ///< The alpha value, 0-255, where 0 is fully transparent and 255 is fully opaque.
        colortype color; ///< The color of the rect.
        colortype borderColor; ///< The color of the border.
        int16_t cornerRadius; ///< The radius of the corners.
        bool shouldDrawBorder; ///< If true, the border is drawn
    };
}

#endif // TOUCHGFX_ROUNDEDRECT_HPP