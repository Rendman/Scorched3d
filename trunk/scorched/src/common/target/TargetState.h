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

#if !defined(__INCLUDE_TargetStateh_INCLUDE__)
#define __INCLUDE_TargetStateh_INCLUDE__

class TankFalling;
class NetBuffer;
class NetBufferReader;

class TargetStateMovement
{
public:
	TargetStateMovement();
	virtual ~TargetStateMovement();
};

class TargetState
{
public:
	TargetState();
	virtual ~TargetState();

	TankFalling *getFalling() { return falling_; }
	void setFalling(TankFalling *falling) { falling_ = falling; }

	bool getDisplayDamage() { return displayDamage_; }
	void setDisplayDamage(bool displayDamage) { displayDamage_ = displayDamage; }

	bool getDisplayShadow() { return displayShadow_; }
	void setDisplayShadow(bool displayShadow) { displayShadow_ = displayShadow; }

	bool getDisplayHardwareShadow() { return displayHardwareShadow_; }
	void setDisplayHardwareShadow(bool displayHardwareShadow) { displayHardwareShadow_ = displayHardwareShadow; }

	bool getNoDamageBurn() { return noDamageBurn_; }
	void setNoDamageBurn(bool noDamageBurn) { noDamageBurn_ = noDamageBurn; }

	bool getNoCollision() { return noCollision_; }
	void setNoCollision(bool noCollision) { noCollision_ = noCollision; }

	bool getNoFalling() { return noFalling_; }
	void setNoFalling(bool noFalling) { noFalling_ = noFalling; }

	bool getNoFallingDamage() { return noFallingDamage_; }
	void setNoFallingDamage(bool noFalling) { noFallingDamage_ = noFalling; }

	TargetStateMovement *getMovement() { return movement_; }
	void setMovement(TargetStateMovement *movement) { movement_ = movement; }

	void setDriveOverToDestroy(bool d) { driveOverToDestroy_ = d; }
	bool getDriveOverToDestroy() { return driveOverToDestroy_; }

	void setFlattenDestroy(bool d) { flattenDestroy_ = d; }
	bool getFlattenDestroy() { return flattenDestroy_; }

    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	TankFalling *falling_;
	TargetStateMovement *movement_;
	bool displayHardwareShadow_;
	bool displayShadow_;
	bool displayDamage_;
	bool noDamageBurn_;
	bool noCollision_;
	bool noFalling_;
	bool noFallingDamage_;
	bool driveOverToDestroy_;
	bool flattenDestroy_;

};

#endif // __INCLUDE_TargetStateh_INCLUDE__