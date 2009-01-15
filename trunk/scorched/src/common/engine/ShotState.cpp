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

#include <engine/ShotState.h>
#include <engine/ActionController.h>
#include <actions/CheckResurrection.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>
#include <common/RandomGenerator.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <tank/TankAccessories.h>
#include <tank/TankPosition.h>
#include <target/TargetLife.h>
#include <target/TargetState.h>

ShotState::ShotState(ScorchedContext &context,
	PlayShots &playShots) :
	context_(context), playShots_(playShots),
	firstTime_(true)
{
}

ShotState::~ShotState()
{
}

void ShotState::setup()
{
	// Set all player kills this turn to 0 (used for multikill)
	{
		std::map<unsigned int, Tank *> &tanks = 
			context_.getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			tank->getScore().setTurnKills(0);
		}
	}

	// Reset the counts in the action controller
	context_.getActionController().setStopImmediately(true);
	context_.getActionController().resetTime();
	context_.getActionController().clear();

	if (context_.getOptionsGame().getActionSyncCheck())
	{
		std::map<unsigned int, Target *> &targets =
			context_.getTargetContainer().getTargets();
		std::map<unsigned int, Target *>::iterator itor;
		for (itor = targets.begin();
			itor != targets.end();
			itor++)
		{
			Target *target = itor->second;
			if (target->getPlayerId() >= TargetID::MIN_TARGET_TRANSIENT_ID ||
				target->getTargetState().getMovement() ||
				!target->isTarget())
			{
				context_.getActionController().addSyncCheck(
					S3D::formatStringBuffer("TargetDef : %u %s %i %i,%i,%i %i,%i,%i %s", 
						target->getPlayerId(),
						target->getCStrName().c_str(),
						target->getLife().getLife().getInternal(),
						target->getLife().getTargetPosition()[0].getInternal(),
						target->getLife().getTargetPosition()[1].getInternal(),
						target->getLife().getTargetPosition()[2].getInternal(),
						target->getLife().getVelocity()[0].getInternal(),
						target->getLife().getVelocity()[1].getInternal(),
						target->getLife().getVelocity()[2].getInternal(),
						target->getAlive()?"Alive":"Dead"));
			}
		}
	}

	// Add all shots that should be run at the start of the round
	// to the action controller
	playShots_.playShots(context_);
	context_.getActionController().addLastAction(new CheckResurrection());

	// Reset the amount of time taken
	firstTime_ = true;

	// Add all of the new events
	context_.getActionController().getEvents().initialize(context_);
}

bool ShotState::run(float frameTime)
{
	if (!context_.getActionController().noReferencedActions() ||
		firstTime_)
	{
		// The action controller will now have shots to simulate
		// We continue simulation until there are no actions left
		// in the action controller
		firstTime_ = false;
	}
	else
	{
		// We have finished all shots
		Logger::log(S3D::formatStringBuffer(
			"Finished playing Shots %.2f seconds", 
				context_.getActionController().getActionTime().asFloat()));
		context_.getActionController().getEvents().clear();
		context_.getActionController().logProfiledActions();
		context_.getActionController().setStopImmediately(false);

		return true;
	}
	return false;
}