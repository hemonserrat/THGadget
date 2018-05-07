/**
 * display.hpp
 *
 * This file is part of THGadget.
 *
 * Copyright (C) 2017,  Hernan Monserrat hemonserrat<at>gmail<dot>com
 * Based on the TFT Touch Shield 2.0 examples - tftbmp by Loove 2013-1-21
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

uint32_t read32(File f);
uint16_t read16(File f);

boolean bmpReadHeader(File f);
void bmpdraw(File f, int x, int y);
void loadBackground(void);

#endif
