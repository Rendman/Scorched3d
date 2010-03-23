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

#ifndef _ComsSimulateResultMessage_h
#define _ComsSimulateResultMessage_h

#include <coms/ComsMessage.h>

class ComsSimulateResultMessage : public ComsMessage
{
public:
	static ComsMessageType ComsSimulateResultMessageType;

	ComsSimulateResultMessage();
	ComsSimulateResultMessage(unsigned int serverTime);
	virtual ~ComsSimulateResultMessage();

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer);
    virtual bool readMessage(NetBufferReader &reader);

	unsigned int getServerTime() { return serverTime_; }

protected:
	unsigned int serverTime_;

private:
	ComsSimulateResultMessage(const ComsSimulateResultMessage &);
	const ComsSimulateResultMessage & operator=(const ComsSimulateResultMessage &);

};

#endif //_ComsSimulateResultMessage_h
