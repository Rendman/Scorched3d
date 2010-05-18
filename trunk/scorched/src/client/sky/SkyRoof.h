////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#if !defined(__INCLUDE_SkyRoofh_INCLUDE__)
#define __INCLUDE_SkyRoofh_INCLUDE__

#include <common/Vector.h>

class HeightMap;
class SkyRoof
{
public:
	SkyRoof();
	virtual ~SkyRoof();

	void draw(bool asShadow);
	void generate();

protected:
	unsigned int list_;
	unsigned int tris_;

	void makeList();
	void drawSegment(
		Vector &a, Vector &b, 
		Vector &na, Vector &nb);
	void makeNormal(Vector &position, Vector &normal);
};

#endif // __INCLUDE_SkyRoofh_INCLUDE__
