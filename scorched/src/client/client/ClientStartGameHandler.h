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


#ifndef _ClientStartGameHandler_h
#define _ClientStartGameHandler_h

#include <simactions/TankStartMoveSimAction.h>

class ClientStartGameHandler 
{
public:
	static ClientStartGameHandler* instance();

	void startGame(TankStartMoveSimAction *action);

protected:
	static ClientStartGameHandler* instance_;

private:
	ClientStartGameHandler();
	virtual ~ClientStartGameHandler();

	ClientStartGameHandler(const ClientStartGameHandler &);
	const ClientStartGameHandler & operator=(const ClientStartGameHandler &);

};

#endif // _ClientStartGameHandler_h

