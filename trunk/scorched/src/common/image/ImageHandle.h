////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ImageHANDLE_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_)
#define AFX_ImageHANDLE_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_

#include <image/Image.h>

class ImageHandle : public Image
{
public:
	ImageHandle();
	ImageHandle(const Image &other);
	ImageHandle(const ImageHandle &other);
	virtual ~ImageHandle();

	ImageHandle &operator=(const Image &other);
	ImageHandle &operator=(const ImageHandle &other);

	virtual unsigned char *getBits() { return bits_; }
	virtual void setBits(unsigned char *bits) { bits_ = bits; }
	virtual int getWidth() { return width_; }
	virtual int getHeight() { return height_; }

	virtual void removeOwnership() { owner_ = false; }

	virtual int getAlignment() { return alignment_; }
	virtual int getComponents() { return components_; }

protected:
	int width_, height_;
	int alignment_, components_;
	unsigned char *bits_;
	bool owner_;
	
};

#endif // !defined(AFX_ImageHANDLE_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_)
