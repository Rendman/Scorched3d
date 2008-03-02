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

#include <weapons/WeaponGiveMoney.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankScore.h>
#include <common/Defines.h>
#include <common/ChannelManager.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveMoney);

WeaponGiveMoney::WeaponGiveMoney()
{

}

WeaponGiveMoney::~WeaponGiveMoney()
{

}

bool WeaponGiveMoney::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("money", money_)) return false;

	return true;
}

void WeaponGiveMoney::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity)
{
	context.actionController->addAction(
		new CallbackWeapon("WeaponGiveMoney", this, 0, 0, 
			weaponContext, position, velocity));
}

void WeaponGiveMoney::weaponCallback(
	ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity,
	unsigned int userData)
{
	Tank *tank = context.tankContainer->getTankById(weaponContext.getPlayerId());
	if (!tank) return;

	tank->getScore().setMoney(tank->getScore().getMoney() + money_);

	if (!context.serverMode)
	{
		if (money_ > 0)
		{
			ChannelText text("combat", 
				S3D::formatStringBuffer("[p:%s] received $%i", 
				tank->getName(), money_));
			//info.setPlayerId(weaponContext.getPlayerId());
			ChannelManager::showText(text);
		}
		else
		{
			ChannelText text("combat", 
				S3D::formatStringBuffer("[p:%s] lost $%i", 
				tank->getName(), -money_));
			//info.setPlayerId(weaponContext.getPlayerId());
			ChannelManager::showText(text);
		}
	}
}

