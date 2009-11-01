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

#include <coms/ComsSyncCheckMessage.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>
#include <engine/ActionController.h>
#ifndef S3D_SERVER
	#include <client/ScorchedClient.h>
#endif
#include <server/ScorchedServer.h>
#include <target/TargetContainer.h>
#include <target/TargetState.h>
#include <target/TargetLife.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <tank/Tank.h>
#include <landscapemap/LandscapeMaps.h>
#include <set>

ComsMessageType ComsSyncCheckMessage::ComsSyncCheckMessageType("ComsSyncCheckMessageType");

ComsSyncCheckMessage::ComsSyncCheckMessage() :
	ComsMessage(ComsSyncCheckMessageType)
{
}

ComsSyncCheckMessage::ComsSyncCheckMessage(unsigned int syncId,
	ScorchedContext &context) :
	ComsMessage(ComsSyncCheckMessageType),
	syncId_(syncId)
{
	// Send the height map data
	HeightMap &map = context.getLandscapeMaps().getGroundMaps().getHeightMap();
	for (int y=0; y<map.getMapHeight(); y++)
	{
		for (int x=0; x<map.getMapWidth(); x++)
		{
			fixed height = map.getHeight(x, y);
			landscapeBuffer_.addToBuffer(height);
			FixedVector &normal = map.getNormal(x, y);
			landscapeBuffer_.addToBuffer(normal);
		}
	}

	// Send target data
	std::map<unsigned int, Target *> &possibletargets =
		context.getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	targetsBuffer_.addToBuffer((int) possibletargets.size());
	for (itor = possibletargets.begin();
		itor != possibletargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		targetsBuffer_.addToBuffer(target->getPlayerId());
		targetsBuffer_.addToBuffer(target->isTarget());

		NetBuffer tmpBuffer;
		if (target->isTarget())
		{
			target->writeMessage(tmpBuffer);
		}
		else
		{
			((Tank*)target)->writeMessage(tmpBuffer, true);
		}
		targetsBuffer_.addToBuffer(tmpBuffer);
	}

	// Add sync checks
	syncChecks_ = context.getActionController().getSyncCheck();
}

ComsSyncCheckMessage::~ComsSyncCheckMessage()
{
}

bool ComsSyncCheckMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(syncId_);
	buffer.addToBuffer(landscapeBuffer_);
	buffer.addToBuffer(targetsBuffer_);
	int checks = (int) syncChecks_.size();
	buffer.addToBuffer(checks);
	std::vector<std::string>::iterator itor;
	for (itor = syncChecks_.begin();
		itor != syncChecks_.end();
		itor++)
	{
		buffer.addToBuffer(*itor);
	}
	return true;
}

bool ComsSyncCheckMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(syncId_)) return false;
	if (!reader.getFromBuffer(landscapeBuffer_)) return false;
	if (!reader.getFromBuffer(targetsBuffer_)) return false;
	int checks = 0;
	if (!reader.getFromBuffer(checks)) return false;
	for (int c=0; c<checks; c++)
	{
		std::string check;
		if (!reader.getFromBuffer(check)) return false;
		syncChecks_.push_back(check);
	}
	return true;
}

