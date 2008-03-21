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

#include <weapons/WeaponGiveAccessory.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankAccessories.h>
#include <tank/TankScore.h>
#include <common/Defines.h>
#include <common/ChannelManager.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveAccessory);

WeaponGiveAccessory::WeaponGiveAccessory()
{

}

WeaponGiveAccessory::~WeaponGiveAccessory()
{

}

bool WeaponGiveAccessory::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	std::string giveaccessory;
	while (accessoryNode->getNamedChild("giveaccessory", giveaccessory, false))
	{
		Accessory *accessory = context.getAccessoryStore()->
			findByPrimaryAccessoryName(giveaccessory.c_str());
		if (!accessory)
		{
			return accessoryNode->returnError(
				S3D::formatStringBuffer("Failed to find accessory named %s",
					giveaccessory.c_str()));
		}
		giveAccessories_.push_back(accessory);
	}

	if (!accessoryNode->getNamedChild("number", number_)) return false;

	return true;
}

void WeaponGiveAccessory::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity)
{
	context.actionController->addAction(
		new CallbackWeapon("WeaponGiveAccessory", this, 0, 0, 
			weaponContext, position, velocity));
}

void WeaponGiveAccessory::weaponCallback(
	ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity,
	unsigned int userData)
{
	Tank *tank = context.tankContainer->getTankById(weaponContext.getPlayerId());
	if (!tank) return;

	std::vector<Accessory *>::iterator itor;
	for (itor = giveAccessories_.begin();
		itor != giveAccessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);

		if (number_ > 0)
		{
			if (tank->getAccessories().accessoryAllowed(accessory, number_))
			{
				tank->getAccessories().add(accessory, number_);
				if (!context.serverMode)
				{
					ChannelText text("combat", 
						S3D::formatStringBuffer("[p:%s] received %i * [w:%s]", 
						tank->getName(),
						number_, accessory->getName()));
					//info.setPlayerId(weaponContext.getPlayerId());
					ChannelManager::showText(text);
				}
			}
			else
			{
				int money = accessory->getSellPrice() * number_;
				tank->getScore().setMoney(tank->getScore().getMoney() + money);

				if (!context.serverMode)
				{
					ChannelText text("combat", 
						S3D::formatStringBuffer("[p:%s] received $%i", 
						tank->getName(), money));
					//info.setPlayerId(weaponContext.getPlayerId());
					ChannelManager::showText(text);
				}
			}
		}
		else
		{
			int count = tank->getAccessories().getAccessoryCount(accessory);
			if (count > 0)
			{
				int loose = MIN(count, -number_);

				tank->getAccessories().rm(accessory, loose);
				if (!context.serverMode)
				{
					ChannelText text("combat", 
						S3D::formatStringBuffer("[p:%s] lost %i * [w:%s]", 
						tank->getName(),
						loose, accessory->getName()));
					//info.setPlayerId(weaponContext.getPlayerId());
					ChannelManager::showText(text);
				}
			}
		}
	}
}