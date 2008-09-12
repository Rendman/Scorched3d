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

#include <actions/SoundAction.h>
#include <weapons/WeaponSound.h>
#include <weapons/AccessoryStore.h>
#ifndef S3D_SERVER
	#include <sound/SoundUtils.h>
#endif
#include <common/Defines.h>

SoundAction::SoundAction(FixedVector &position, WeaponSound *weapon) :
	ActionReferenced("SoundAction"),
	weapon_(weapon), position_(position)
{

}

SoundAction::~SoundAction()
{
}

void SoundAction::init()
{
}

void SoundAction::simulate(fixed frameTime, bool &remove)
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		SoundBuffer *activateSound = 
			Sound::instance()->fetchOrCreateBuffer(
				S3D::getDataFile(weapon_->getSound()));

		VirtualSoundSource *source = new VirtualSoundSource(
			VirtualSoundPriority::eAction, false, true);
		if (weapon_->getRelative())	source->setRelative();
		else source->setPosition(position_.asVector());
		source->setGain(weapon_->getGain().asFloat());
		source->setReferenceDistance(weapon_->getReferenceDistance().asFloat());
		source->setRolloff(weapon_->getRolloff().asFloat());
		source->play(activateSound);
	}
#endif // #ifndef S3D_SERVER

	remove = true;
	Action::simulate(frameTime, remove);
}

std::string SoundAction::getActionDetails()
{
	return weapon_->getParent()->getName();
}