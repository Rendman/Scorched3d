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

#if !defined(__INCLUDE_ScorchedContexth_INCLUDE__)
#define __INCLUDE_ScorchedContexth_INCLUDE__

class AccessoryStore;
class ActionController;
class GameState;
class TargetSpace;
class TargetContainer;
class TargetMovement;
class LandscapeMaps;
class ComsMessageHandler;
class NetInterface;
class OptionsScorched;
class OptionsTransient;
class ViewPoints;
class ModFiles;
class LandscapeDefinitions;
class TankContainer;
class TankModelStore;
class TankTeamScore;
class LUAWrapper;

class ScorchedContext
{
public:
	ScorchedContext(const char *name);
	virtual ~ScorchedContext();

	ActionController *actionController;
	GameState *gameState;
	LandscapeMaps *landscapeMaps;
	ComsMessageHandler *comsMessageHandler;
	NetInterface *netInterface;
	OptionsScorched *optionsGame;
	OptionsTransient *optionsTransient;
	ViewPoints *viewPoints;
	ModFiles *modFiles;
	AccessoryStore *accessoryStore;
	LandscapeDefinitions *landscapes;
	TargetContainer *targetContainer;
	TankContainer *tankContainer;
	TankModelStore *tankModelStore;
	TankTeamScore *tankTeamScore;
	TargetSpace *targetSpace;
	TargetMovement *targetMovement;
	LUAWrapper *luaWrapper;
	bool serverMode;
};

#endif