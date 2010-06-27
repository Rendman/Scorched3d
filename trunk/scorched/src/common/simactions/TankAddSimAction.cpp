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

#include <simactions/TankAddSimAction.h>
#include <simactions/TankChangeSimAction.h>
#include <simactions/TankRankSimAction.h>
#include <tank/TankContainer.h>
#include <tank/TankAvatar.h>
#include <tank/TankState.h>
#include <tank/TankModelStore.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankModelContainer.h>
#include <tankai/TankAIStore.h>
#include <common/ChannelManager.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <coms/ComsAddPlayerMessage.h>
#include <server/ServerBanned.h>
#include <server/ServerChannelManager.h>
#include <server/ScorchedServer.h>
#include <server/ServerSimulator.h>
#ifndef S3D_SERVER
#include <client/ClientChannelManager.h>
#include <tankgraph/TargetRendererImplTank.h>
#endif

unsigned int TankAddSimAction::TankAddSimActionCount = 0;

REGISTER_CLASS_SOURCE(TankAddSimAction);

TankAddSimAction::TankAddSimAction()
{
	TankAddSimActionCount++;
}

TankAddSimAction::TankAddSimAction(
	unsigned int playerId, unsigned int destinationId,
	const std::string &uniqueId, const std::string &sUID, const std::string &hostDesc,
	unsigned int ipAddress, 
	const LangString &playerName, const std::string &aiName) :
	playerId_(playerId), destinationId_(destinationId),
	uniqueId_(uniqueId), sUID_(sUID), hostDesc_(hostDesc),
	ipAddress_(ipAddress), aiName_(aiName), playerName_(playerName)
{
	TankAddSimActionCount++;
}

TankAddSimAction::~TankAddSimAction()
{
	TankAddSimActionCount--;
}

bool TankAddSimAction::invokeAction(ScorchedContext &context)
{
	// Alocate new color
	Vector color = TankColorGenerator::instance()->getNextColor(
		context.getTankContainer().getPlayingTanks());
	if (playerId_ == TargetID::SPEC_TANK_ID)
	{
		color = Vector(0.7f, 0.7f, 0.7f);
	}

	// Create tank
	Tank *tank = new Tank(
		context,
		playerId_,
		destinationId_,
		playerName_,
		color);

	// Choose team
	int team = 0;
	if (context.getOptionsGame().getTeams() > 1)
	{
		if (destinationId_ == 0 && 
			context.getOptionsGame().getTeamBallance() ==
			OptionsGame::TeamBallanceBotsVs)
		{
			team = 1;
		}
		else
		{
			team = context.getOptionsTransient().getLeastUsedTeam(
				context.getTankContainer());
		}
	}
	tank->setTeam(team);

	// Setup server stuff
	if (context.getServerMode())
	{
		tank->setUniqueId(uniqueId_.c_str());
		tank->setSUI(sUID_.c_str());
		tank->setHostDesc(hostDesc_.c_str());
		tank->setIpAddress(ipAddress_);
		if (aiName_.c_str()[0])
		{
			TankAI *ai = ((ScorchedServer &)context).getTankAIs().
				getAIByName(aiName_.c_str());
			tank->setTankAI(ai->createCopy(tank));
		}
	}

	// Add this tank 
	context.getTankContainer().addTank(tank);
	StatsLogger::instance()->tankConnected(tank);

	if (context.getServerMode())
	{
		StatsLogger::TankRank rank = StatsLogger::instance()->tankRank(tank);
		TankRankSimAction *rankSimAction = new TankRankSimAction();
		rankSimAction->addRank(rank);
		((ScorchedServer &)context).getServerSimulator().addSimulatorAction(rankSimAction);

		if (destinationId_ == 0)
		{
			// Form the tank ai model
			TankModel *tankModel = 
				context.getTankModels().getRandomModel(team, true);
			TankAvatar tankAvatar;
			tankAvatar.loadFromFile(S3D::getDataFile("data/avatars/computer.png"));

			// Tell the clients to create this tank
			ComsAddPlayerMessage addPlayerMessage(
				playerId_,
				playerName_,
				color,
				tankModel->getName(),
				0,
				team,
				""); 
			addPlayerMessage.setPlayerIconName("data/avatars/computer.png");
			addPlayerMessage.getPlayerIcon().addDataToBuffer(
				tankAvatar.getFile().getBuffer(),
				tankAvatar.getFile().getBufferUsed());

			TankChangeSimAction *changeAction = new TankChangeSimAction(addPlayerMessage);
			((ScorchedServer &)context).getServerSimulator().addSimulatorAction(changeAction);
		}
	}

#ifndef S3D_SERVER
	if (!context.getServerMode())
	{
		tank->setRenderer(new TargetRendererImplTank(tank));
		if (ClientChannelManager::instance()->getMutedPlayers().find(tank->getPlayerId())
			!= ClientChannelManager::instance()->getMutedPlayers().end())
		{
			tank->getState().setMuted(true);
		}
	}
#else
	{
		// Add to dialog
		Logger::log(S3D::formatStringBuffer("Player connected dest=\"%i\" id=\"%i\" name=\"%s\" unique=[%s] SUI=[%s]",
			tank->getDestinationId(),
			tank->getPlayerId(),
			tank->getCStrName().c_str(),
			tank->getUniqueId(),
			tank->getSUI()));
	}
	// Check if admin muted
	if (ipAddress_ != 0)
	{
		ServerBanned::BannedType type = 
			ScorchedServer::instance()->getBannedPlayers().getBanned(tank->getUniqueId(), tank->getSUI());
		if (type == ServerBanned::Muted)	
		{
			tank->getState().setMuted(true);
			ScorchedServer::instance()->getServerChannelManager().sendText( 
				ChannelText("admin", 
					"PLAYER_ADMIN_MUTED", 
					"Player admin muted [p:{0}]",
					tank->getTargetName()),
					true);
		}
		else if (type == ServerBanned::Flagged)
		{
			ScorchedServer::instance()->getServerChannelManager().sendText( 
				ChannelText("admin",
					"PLAYER_ADMIN_FLAGGED",
					"Player admin flagged [p:{0}]",
					tank->getTargetName()),
					true);
		}
	}
#endif

	ChannelText text("info", 
		"PLAYER_CONNECTED",
		"Player connected [p:{0}]",
		tank->getTargetName());
	ChannelManager::showText(context, text);

	return true;
}

bool TankAddSimAction::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(destinationId_);
	buffer.addToBuffer(playerName_);

	return true;
}

bool TankAddSimAction::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(destinationId_)) return false;
	if (!reader.getFromBuffer(playerName_)) return false;
	
	return true;
}
