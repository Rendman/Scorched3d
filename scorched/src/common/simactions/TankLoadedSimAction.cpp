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

#include <simactions/TankLoadedSimAction.h>
#include <tank/TankAvatar.h>
#include <tank/TankState.h>
#include <tank/TankContainer.h>

REGISTER_CLASS_SOURCE(TankLoadedSimAction);

TankLoadedSimAction::TankLoadedSimAction()
{
}

TankLoadedSimAction::TankLoadedSimAction(unsigned int playerId, bool notSpectator) :
	playerId_(playerId), notSpectator_(notSpectator)
{
}

TankLoadedSimAction::~TankLoadedSimAction()
{
}

bool TankLoadedSimAction::invokeAction(ScorchedContext &context)
{
	Tank *tank = context.getTankContainer().getTankById(playerId_);
	if (!tank) return false;

	if (notSpectator_) tank->getState().setState(TankState::sDead);
	else tank->getState().setState(TankState::sSpectator);

	return true;
}

bool TankLoadedSimAction::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(notSpectator_);
	return true;
}

bool TankLoadedSimAction::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(notSpectator_)) return false;
	return true;
}