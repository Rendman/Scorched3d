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

#if !defined(AFX_FACE_H__711CB826_F1C1_4563_8760_65F19C8FDAAC__INCLUDED_)
#define AFX_FACE_H__711CB826_F1C1_4563_8760_65F19C8FDAAC__INCLUDED_

#include <common/Vector.h>

class Face  
{
public:
	Face();
	virtual ~Face();

	int v[3];
	Vector normal[3];
	Vector tcoord[3];
};

#endif // !defined(AFX_FACE_H__711CB826_F1C1_4563_8760_65F19C8FDAAC__INCLUDED_)
