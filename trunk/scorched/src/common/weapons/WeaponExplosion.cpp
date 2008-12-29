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

#include <weapons/WeaponExplosion.h>
#include <weapons/Accessory.h>
#include <actions/Explosion.h>
#include <engine/ActionController.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponExplosion);

WeaponExplosion::WeaponExplosion()
{

}

WeaponExplosion::~WeaponExplosion()
{

}

bool WeaponExplosion::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

    if (!accessoryNode->getNamedChild("size", sizeExp_)) return false;
	if (!accessoryNode->getNamedChild("hurtamount", hurtAmountExp_)) return false;

	accessoryNode->getNamedChild("explosionshake", shakeExp_, false);
	accessoryNode->getNamedChild("createmushroomamount", createMushroomAmountExp_, false);
	accessoryNode->getNamedChild("minlife", minLifeExp_, false);
	accessoryNode->getNamedChild("maxlife", maxLifeExp_, false);

	if (!params_.parseXML(accessoryNode)) return false;

	return true;
}

void WeaponExplosion::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity)
{
	ExplosionParams *newParams = new ExplosionParams(params_);

	newParams->setSize(sizeExp_.getValue(context, params_.getSize()));
	newParams->setHurtAmount(hurtAmountExp_.getValue(context, params_.getHurtAmount()));
	newParams->setShake(shakeExp_.getValue(context, params_.getShake()));
	newParams->setMinLife(minLifeExp_.getValue(context, params_.getMinLife()));
	newParams->setMaxLife(maxLifeExp_.getValue(context, params_.getMaxLife()));
	newParams->setCreateMushroomAmount(createMushroomAmountExp_.getValue(context, params_.getCreateMushroomAmount()));

	Action *action = new Explosion(
		position, newParams, this, weaponContext);
	context.getActionController().addAction(action);	
}

