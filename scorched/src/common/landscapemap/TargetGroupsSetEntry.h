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

#if !defined(__INCLUDE_TargetGroupsSetEntryh_INCLUDE__)
#define __INCLUDE_TargetGroupsSetEntryh_INCLUDE__

#include <target/TargetGroup.h>
#include <map>
#include <string>

class TargetGroupsSetEntry
{
public:
	TargetGroupsSetEntry(const char *name);
	virtual ~TargetGroupsSetEntry();

	const char *getName() { return name_.c_str(); }

	TargetGroup *getObjectByPos(int position);
	TargetGroup *getObjectById(unsigned int playerId);
	bool hasObject(TargetGroup *object);
	bool hasObjectById(unsigned int playerId);
	std::map<unsigned int, TargetGroup *> &getObjects() { return objects_; }

	virtual void addObject(TargetGroup *object, bool thin);
	virtual bool removeObject(TargetGroup *object);

	virtual int getObjectCount();

protected:
	std::string name_;
	std::map<unsigned int, TargetGroup *> objects_;
};

#endif // __INCLUDE_TargetGroupsSetEntryh_INCLUDE__
