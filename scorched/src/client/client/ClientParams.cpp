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

#include <client/ClientParams.h>

ClientParams *ClientParams::instance_ = 0;

ClientParams *ClientParams::instance()
{
	if (!instance_)
	{
		instance_ = new ClientParams;
	}

	return instance_;
}

ClientParams::ClientParams() :
	connect_(options_, "connect", 
		"The name of the server to connect to, starts a NET/LAN client", 0, ""),
	client_(options_, "startclient",
		"Starts a scorched 3d client, requires the name of the client settings file e.g. data/singlecustom.xml", 0, ""),
	startcustom_(options_, "startcustom",
		"Starts a scorched 3d client, Uses the last custom game made", 0, false),
	save_(options_, "loadsave",
		"Continues a scorched 3d client game, requires the name of the saved game.", 0, ""),
	exittime_(options_, "exittime",
		"The time after which the client will exit", 0, 0),
	username_(options_, "username",
		"The username of the NET/LAN server", 0, ""),
	password_(options_, "password",
		"The password of the NET/LAN server", 0, ""),
	nonParam_(nonParamOptions_, "launch file", 
		".3dl scorched3d online gaming launching config", 0, "")
{

}

ClientParams::~ClientParams()
{
	
}

void ClientParams::reset()
{
	connect_.setValueFromString(connect_.getDefaultValueAsString());
	client_.setValueFromString(client_.getDefaultValueAsString());
	startcustom_.setValueFromString(startcustom_.getDefaultValueAsString());
	save_.setValueFromString(save_.getDefaultValueAsString());
	username_.setValueFromString(username_.getDefaultValueAsString());
	password_.setValueFromString(password_.getDefaultValueAsString());
	exittime_.setValueFromString(exittime_.getDefaultValueAsString());
}