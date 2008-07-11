////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <coms/ComsAdminMessage.h>

ComsAdminMessage::ComsAdminMessage(
	ComsAdminMessageType type,
	const char *param1,
	const char *param2) :
	ComsMessage("ComsAdminMessage"),
	type_(type),
	param1_(param1),
	param2_(param2)
{

}

ComsAdminMessage::~ComsAdminMessage()
{

}

bool ComsAdminMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) type_);
	buffer.addToBuffer(param1_);
	buffer.addToBuffer(param2_);
	return true;
}

bool ComsAdminMessage::readMessage(NetBufferReader &reader)
{
	int t = 0;
	if (!reader.getFromBuffer(t)) return false; 
	type_ = (ComsAdminMessageType) t;
	if (!reader.getFromBuffer(param1_)) return false; 
	if (!reader.getFromBuffer(param2_)) return false; 

	return true;
}
