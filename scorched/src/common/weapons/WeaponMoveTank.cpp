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

#include <weapons/WeaponMoveTank.h>
#include <weapons/AccessoryStore.h>
#include <actions/TankMovement.h>
#include <engine/ActionController.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeDefn.h>

REGISTER_ACCESSORY_SOURCE(WeaponMoveTank);

WeaponMoveTank::WeaponMoveTank() :
	stepTime_(fixed(true, 500)), useFuel_(-1), maximumRange_(100)
{

}

WeaponMoveTank::~WeaponMoveTank()
{

}

bool WeaponMoveTank::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;
	accessoryNode->getNamedChild("steptime", stepTime_, false);
	accessoryNode->getNamedChild("maximumrange", maximumRange_, false);

	std::string usefuel;
	if (accessoryNode->getNamedChild("usefuel", usefuel, false))
	{
		if (0 == strcmp(usefuel.c_str(), "true")) useFuel_ = -1;
		else if (0 == strcmp(usefuel.c_str(), "false")) useFuel_ = 0;
		else useFuel_ = atoi(usefuel.c_str());
	}

	return true;
}

void WeaponMoveTank::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity)
{
	int posX = position[0].asInt();
	int posY = position[1].asInt();
	if (posX > 5 && posX < context.landscapeMaps->getDefinitions().getDefn()->landscapewidth - 5 &&
		posY > 5 && posY < context.landscapeMaps->getDefinitions().getDefn()->landscapeheight - 5)
	{
		context.actionController->addAction(
			new TankMovement(weaponContext, this, posX, posY));		
	}
}