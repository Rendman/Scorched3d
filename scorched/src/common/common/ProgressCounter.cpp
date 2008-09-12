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

#include <common/ProgressCounter.h>
#include <common/Logger.h>

ProgressCounterI::~ProgressCounterI()
{

}

ProgressCounter::ProgressCounter(ProgressCounterI *user) : 
	currentOp_("None"), currentPercentage_(0.0f), user_(user)
{

}

ProgressCounter::~ProgressCounter()
{

}

void ProgressCounter::setNewOp(const std::string &op)
{
	currentOp_ = op;
	Logger::log(op);
	if (user_) user_->progressChange(currentOp_.c_str(), currentPercentage_);
}

void ProgressCounter::setNewPercentage(float percentage)
{
	currentPercentage_ = percentage;
	if (user_) user_->progressChange(currentOp_.c_str(), currentPercentage_);
}