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

#if !defined(__INCLUDE_GroundMapsh_INCLUDE__)
#define __INCLUDE_GroundMapsh_INCLUDE__

#include <landscapemap/HeightMap.h>
#include <landscapemap/NapalmMap.h>
#include <landscapemap/TargetGroups.h>

class RandomGenerator;
class ScorchedContext;
class LandscapeInclude;
class LandscapeDefinitionCache;
class GroundMaps
{
public:
	GroundMaps(LandscapeDefinitionCache &defnCache);
	virtual ~GroundMaps();

	// Generates the next level
	void generateMaps(
		ScorchedContext &context,
		ProgressCounter *counter = 0);

	// Height map functions
	fixed getHeight(int w, int h);
	fixed getInterpHeight(fixed w, fixed h);
	FixedVector &getNormal(int w, int h);
	void getInterpNormal(fixed w, fixed h, FixedVector &normal);
	bool getIntersect(Line &direction, Vector &intersect);

	// Napalm map functions
	fixed &getNapalmHeight(int w, int h)
		{ return nmap_.getNapalmHeight(w, h); }

	// Deformable landscape area fns
	int getLandscapeWidth();
	int getLandscapeHeight();

	// Playable landscape area fns
	int getArenaWidth();
	int getArenaHeight();
	int getArenaX();
	int getArenaY();

	// Objects funtions
	TargetGroups &getGroups() { return groups_; }

	// Actual heightmap
	HeightMap &getHeightMap() { return map_; }

protected:
	int arenaX_, arenaY_;
	int arenaWidth_, arenaHeight_;
	HeightMap map_; // The current level's heightmap
	NapalmMap nmap_; // How high napalm is at certain points
	TargetGroups groups_; // The groups in the scene
	LandscapeDefinitionCache &defnCache_;

	// Generate levels
	void generateHMap(
		ScorchedContext &context,
		ProgressCounter *counter = 0);
	void generateObjects(
		ScorchedContext &context,
		ProgressCounter *counter = 0);
	void generateObject(RandomGenerator &generator, 
		LandscapeInclude &place,
		ScorchedContext &context,
		unsigned int &playerId,
		ProgressCounter *counter = 0);

};

#endif // __INCLUDE_GroundMapsh_INCLUDE__
