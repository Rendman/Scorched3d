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

#if !defined(__INCLUDE_ServerBrowserServerListh_INCLUDE__)
#define __INCLUDE_ServerBrowserServerListh_INCLUDE__

#include <SDL/SDL.h>
#include <map>
#include <string>
#include <vector>

class ServerBrowserEntry
{
public:
	ServerBrowserEntry();
	virtual ~ServerBrowserEntry();

	void addAttribute(const std::string &name, const std::string &value);
	const char *getAttribute(const std::string &attrName);

protected:
	std::map<std::string, std::string> attributes_;
};

class ServerBrowserServerList
{
public:
	ServerBrowserServerList();
	virtual ~ServerBrowserServerList();

	unsigned int &getRefreshId() { return refreshId_; }

	// Accessors onto the list
	int getNoEntries();
	const char *getEntryValue(int pos, const std::string &name);
	void addEntryValue(int pos, const std::string &name, const std::string &value);
	void sortEntries(const std::string &name);
	void addEntry(ServerBrowserEntry &entry);

	void clear();

protected:
	unsigned int refreshId_;
	std::vector<ServerBrowserEntry> servers_;
	SDL_mutex *vectorMutex_;
};

#endif
