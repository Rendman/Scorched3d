////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_Laserh_INCLUDE__)
#define __INCLUDE_Laserh_INCLUDE__

#include <engine/ActionReferenced.h>
#include <weapons/WeaponLaser.h>

class Laser : public ActionReferenced
{
public:
	Laser(WeaponLaser *weapon,
		FixedVector &position, FixedVector &direction,
		WeaponFireContext &weaponContext);
	virtual ~Laser();

	virtual void init();
	virtual void simulate(fixed frameTime, bool &remove);
	virtual void draw();
	virtual std::string getActionDetails();

protected:
	fixed totalTime_;
	fixed drawLength_;
	WeaponFireContext weaponContext_;
	WeaponLaser *weapon_;
	FixedVector position_, direction_;

	bool firstTime_;
	fixed laserTime_, hurtRadius_;
	float angXY_, angYZ_;
	fixed length_, damage_;

};

#endif // __INCLUDE_Laserh_INCLUDE__