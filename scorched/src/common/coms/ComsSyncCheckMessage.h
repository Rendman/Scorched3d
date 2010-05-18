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

#ifndef _ComsSyncCheckMessage_h
#define _ComsSyncCheckMessage_h

#include <coms/ComsMessage.h>
#include <engine/ScorchedContext.h>
#include <vector>

class ComsSyncCheckMessage : public ComsMessage
{
public:
	static ComsMessageType ComsSyncCheckMessageType;

	ComsSyncCheckMessage();
	ComsSyncCheckMessage(unsigned int syncId, ScorchedContext &context);
	virtual ~ComsSyncCheckMessage();

	unsigned int getSyncId() { return syncId_; }
	NetBuffer &getLandscapeBuffer() { return landscapeBuffer_; }
	NetBuffer &getRoofBuffer() { return roofBuffer_; }
	NetBuffer &getTargetsBuffer() { return targetsBuffer_; }
	std::vector<std::string> &getSyncCheck() { return syncChecks_; }

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer);
    virtual bool readMessage(NetBufferReader &reader);

protected:
	std::vector<std::string> syncChecks_;
	unsigned int syncId_;
	NetBuffer landscapeBuffer_, roofBuffer_;
	NetBuffer targetsBuffer_;

private:
	ComsSyncCheckMessage(const ComsSyncCheckMessage &);
	const ComsSyncCheckMessage & operator=(const ComsSyncCheckMessage &);

};

#endif // _ComsSyncCheckMessage_h

