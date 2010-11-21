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

#include <tank/TankContainer.h>
#include <tank/TankState.h>

TankContainer::TankContainer(TargetContainer &targets) : 
	targets_(targets),
	playerId_(0), 
	destinationId_(0),
	roundId_(0),
	currentPlayer_(0)
{

}

TankContainer::~TankContainer()
{
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks_.begin();
		itor != tanks_.end();
		itor++)
	{
		delete itor->second;
	}
	tanks_.clear();
}

void TankContainer::addTank(Tank *tank)
{
	targets_.internalAddTarget(tank);
	tanks_[tank->getPlayerId()] = tank;
}

Tank *TankContainer::removeTank(unsigned int playerId)
{
	Target *target = targets_.internalRemoveTarget(playerId);
	if (target)
	{
		tanks_.erase(playerId);
		DIALOG_ASSERT(!target->isTarget());

		if (currentPlayer_ == target)
		{
			currentPlayer_ = 0;
		}
	}
	return (Tank *) target;
}

Tank *TankContainer::getTankById(unsigned int id)
{
	std::map<unsigned int, Tank *>::iterator findItor =
		tanks_.find(id);
	if (findItor != tanks_.end())
	{
		return (*findItor).second;
	}
	return 0;
}

Tank *TankContainer::getTankByName(const LangString &name)
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks_.begin();
		mainitor != tanks_.end();
		mainitor++)
	{
		Tank *tank = (*mainitor).second;
		if (tank->getTargetName() == name) return tank;
	}
	return 0;
}

void TankContainer::setCurrentPlayerId(unsigned int pid) 
{ 
	playerId_ = pid; 
	if (playerId_)
	{
		currentPlayer_ = getTankById(playerId_);
	}
	else 
	{
		currentPlayer_ = 0;
	}
}

int TankContainer::teamCount()
{
	int team1 = 0;
	int team2 = 0;
	int team3 = 0;
	int team4 = 0;

	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks_.begin();
		mainitor != tanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getTankPlaying() &&
			current->getState().getLives() > 0)
		{
			if (current->getTeam() == 1) team1=1;
			if (current->getTeam() == 2) team2=1;
			if (current->getTeam() == 3) team3=1;
			if (current->getTeam() == 4) team4=1;
		}
	}
	return team1 + team2 + team3 + team4;
}

int TankContainer::aliveCount()
{
	int alive = 0;
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks_.begin();
		mainitor != tanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getTankPlaying() &&
			current->getState().getLives() > 0)
		{
			alive++;
		}
	}
	return alive;
}

int TankContainer::getNoOfNonSpectatorTanks()
{
	int count = 0;
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks_.begin();
		mainitor != tanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getTankPlaying()) count++;
	}
	return count;
}

std::map<unsigned int, Tank *> &TankContainer::getPlayingTanks()
{
	return tanks_;
}

std::map<unsigned int, Tank *> &TankContainer::getAllTanks()
{
	return tanks_;
}

int TankContainer::getNoOfTanks()
{
	int count = 0;
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks_.begin();
		mainitor != tanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		count++;
	}	
	return count;
}
