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

#if !defined(__INCLUDE_TankStateh_INCLUDE__)
#define __INCLUDE_TankStateh_INCLUDE__

#include <net/NetBuffer.h>

class Tank;
class ScorchedContext;
class TankState
{
public:
	enum State
	{
		sInitializing,
		sLoading,
		sPending,
		sNormal,
		sDead
	};

	enum ReadyState
	{
		sReady,
		SNotReady
	};

	TankState(ScorchedContext &context, unsigned int playerId);
	virtual ~TankState();
	
	void setTank(Tank *tank) { tank_ = tank; }

	// State Modifiers
	void newGame();
	void newMatch();
	void clientNewGame();
	
	// Ready State
	void setReady() { readyState_ = sReady; }
	void setNotReady() { readyState_ = SNotReady; }
	ReadyState getReadyState() { return readyState_; }

	// State
	void setState(State s);
	State getState() { return state_; }
	void setSpectator(bool s) { spectator_ = s; }
	bool getSpectator() { return spectator_; }	

	void setAdmin(unsigned int sid) { sid_ = sid; }
	unsigned int getAdmin() { return sid_; }
	void setAdminTries(int adminTries) { adminTries_ = adminTries; }
	int getAdminTries() { return adminTries_; }
	void setMuted(bool muted) { muted_ = muted; }
	bool getMuted() { return muted_; }
	void setSkipShots(bool skip) { skipshots_ = skip; }
	bool getSkipShots() { return skipshots_; }
	int getLives() { return lives_; }
	void setLives(int lives) { lives_ = lives; }
	int getMaxLives() { return maxLives_; }
	void setDestroy(bool destroy) { destroy_ = destroy; }
	bool getDestroy() { return destroy_; }
	
	const char *getStateString();
	const char *getSmallStateString();

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	Tank *tank_;
	ScorchedContext &context_;
	State state_;
	ReadyState readyState_;
	unsigned int sid_; int adminTries_;
	int lives_, maxLives_;
	bool spectator_;
	bool muted_;
	bool skipshots_;
	bool destroy_;

};

#endif
