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

#include <tank/TankSort.h>
#include <tank/Tank.h>
#include <tank/TankContainer.h>
#include <tank/TankTeamScore.h>
#include <tank/TankScore.h>
#include <tank/TankState.h>
#include <common/OptionsScorched.h>

int TankSort::compare(ScorchedContext &context,
		const char *nameX, TankScore &scoreX,
		const char *nameY, TankScore &scoreY)
{
	if (scoreX.getScore() > scoreY.getScore()) return 1;
	if (scoreX.getScore() == scoreY.getScore())
	{
		return strcmp(nameX, nameY);
	}
	return -1;
}

bool TankSort::SortOnScore::operator()(const Tank *x, const Tank *y, ScorchedContext &context) const
{
	Tank &tankX = *((Tank *) x);
	Tank &tankY = *((Tank *) y);
	TankScore &scoreX = tankX.getScore();
	TankScore &scoreY = tankY.getScore();

	if (tankX.getState().getSpectator() &&
		tankY.getState().getSpectator())
	{
		if (strcmp(((Tank *)x)->getName(), ((Tank *)y)->getName()) < 0) return true;
		return false;
	}
	else if (tankX.getState().getSpectator())
	{
		return false;
	}
	else if (tankY.getState().getSpectator())
	{
		return true;
	}

	int compareResult = compare(context, 
		tankX.getName(), scoreX, 
		tankY.getName(), scoreY);
	return (compareResult > 0);
}

int TankSort::getWinningTeam(ScorchedContext &context)
{
	for (int i=1; i<=context.optionsGame->getTeams(); i++)
	{
		int scorei = context.tankTeamScore->getScore(i);

		bool top = true;
		for (int j=1; j<=context.optionsGame->getTeams(); j++)
		{
			if (i == j) continue;

			int scorej = context.tankTeamScore->getScore(j);
			if (scorej >= scorei)
			{
				top = false;
				break;
			}
		}
		if (top) return i;
	}
	return 0;
}

void TankSort::getSortedTanks(std::list<Tank *> &list, ScorchedContext &context)
{
	std::list<Tank *> newList;
	while (!list.empty())
	{
		std::list<Tank *>::iterator removeItor = list.begin();
		std::list<Tank *>::iterator itor = list.begin(); itor++;
		for (;itor != list.end(); itor++)
		{
			static TankSort::SortOnScore compare;
			if (!compare(*itor, *removeItor, context)) removeItor = itor;
		}

		newList.push_front(*removeItor);
		list.erase(removeItor);
	}

	list = newList;
}

void TankSort::getSortedTanksIds(ScorchedContext &context, 
	std::list<unsigned int> &list, bool allTanks)
{
	std::list<Tank *> sortedTanks;
	std::map<unsigned int, Tank *> tanks;
	if (allTanks) tanks = context.tankContainer->getAllTanks();
	else tanks = context.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		sortedTanks.push_back(tank);
	}

	getSortedTanks(sortedTanks, context);
	std::list<Tank *>::iterator resultitor;
	for (resultitor = sortedTanks.begin();
		resultitor != sortedTanks.end();
		resultitor++)
	{
		list.push_back((*resultitor)->getPlayerId());
	}
}