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

#if !defined(__INCLUDE_ServerChannelManagerh_INCLUDE__)
#define __INCLUDE_ServerChannelManagerh_INCLUDE__

#include <coms/ComsMessageHandler.h>
#include <server/ServerChannelFilter.h>
#include <server/ServerChannelAuth.h>
#include <time.h>
#include <set>
#include <map>

class ServerChannelManager : 
	public ComsMessageHandlerI
{
public:
	static ServerChannelManager *instance();

	struct MessageEntry
	{
		std::string message;
		unsigned int messageid;
	};

	void sendText(const ChannelText &text, bool serverLog, bool filter = true);
	void sendText(const ChannelText &text, unsigned int destination, bool serverLog, bool filter = true);
	std::list<MessageEntry> &getLastMessages() { return lastMessages_; }
	std::list<std::string> getAllChannels();

	void simulate(float frameTime);

	// Notification of when players disconnect
	void destinationDisconnected(unsigned int destinationId);
	void refreshDestination(unsigned int destinationId);

	// Inherited from ComsMessageHandlerI
	virtual bool processMessage(
		NetMessage &message,
		const char *messageType,
		NetBufferReader &reader);

protected:
	static ServerChannelManager *instance_;

	class DestinationLocalEntry
	{
	public:
		DestinationLocalEntry(unsigned int localId = 0);

		unsigned int getLocalId() { return localId_; }
		std::set<std::string> &getChannels() { return channels_; }
		std::set<std::string> &getAvailableChannels() { return availableChannels_; }

	protected:
		unsigned int localId_;
		std::set<std::string> channels_; // Subscribed channels
		std::set<std::string> availableChannels_; // Available channeld
	};
	class DestinationEntry 
	{
	public:
		DestinationEntry(unsigned int destinationId);

		unsigned int getDestinationId() { return destinationId_; }

		bool hasChannel(const char *channel);
		void addChannel(const char *channel, unsigned int localId, bool current);
		void removeChannel(const char *channel, unsigned int localId);

		bool hasLocalId(unsigned int localId);
		void addLocalId(unsigned int localId);
		void removeLocalId(unsigned int localId);

		void setMessageCount(unsigned int count) { messageCount_ = count; }
		unsigned int getMessageCount() { return messageCount_; }

		time_t getMuteTime() { return muteTime_; }
		void setMuteTime(time_t t) { muteTime_ = t; }

		void getLocalIds(const char *channel, std::list<unsigned int> &ids);
		std::map<unsigned int, DestinationLocalEntry> &getLocalEntries() { return localEntries_; }

	protected:
		time_t muteTime_;
		unsigned int messageCount_;
		unsigned int destinationId_;
		std::set<std::string> channels_; // Subscribed channeld for all localids
		std::map<unsigned int, DestinationLocalEntry> localEntries_;

		void updateChannels();
	};
	class ChannelEntry
	{
	public:
		ChannelEntry(ChannelDefinition def,
			ServerChannelFilter *filter = 0,
			ServerChannelAuth *auth = 0);
		virtual ~ChannelEntry();

		const char *getName() { return channelDef_.getChannel(); }
		ChannelDefinition &getDefinition() { return channelDef_; }
		ServerChannelFilter *getFilter() { return filter_; }
		ServerChannelAuth *getAuth() { return auth_; }

	protected:
		ChannelDefinition channelDef_;
		ServerChannelFilter *filter_;
		ServerChannelAuth *auth_;
	};

	unsigned int lastMessageId_;
	float totalTime_;
	std::map<unsigned int, DestinationEntry *> destinationEntries_;
	std::list<ChannelEntry *> channelEntries_;
	std::list<MessageEntry> lastMessages_;

	ChannelEntry *getChannelEntryByName(const char *name);
	DestinationEntry *getDestinationEntryById(unsigned int destinationId);

	void registerClient(unsigned int destinationId, unsigned int localId,
		std::list<ChannelDefinition> &startChannels);
	void deregisterClient(unsigned int destinationId, unsigned int localId);
	void joinClient(unsigned int destinationId, unsigned int localId,
		std::list<ChannelDefinition> &startChannels);
	void actualSend(const ChannelText &constText,
		std::map<unsigned int, DestinationEntry *> &destinations, 
		bool serverLog, bool filter);

private:
	ServerChannelManager();
	virtual ~ServerChannelManager();

};

#endif