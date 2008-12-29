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


#if !defined(__INCLUDE_PhysicsParticleObjecth_INCLUDE__)
#define __INCLUDE_PhysicsParticleObjecth_INCLUDE__

#include <common/FixedVector.h>
#include <common/FixedVector4.h>
#include <engine/ScorchedCollisionIds.h>

enum PhysicsParticleType
{
	ParticleTypeNone = 0,
	ParticleTypeShot,
	ParticleTypeBounce,
	ParticleTypeFalling
};

struct PhysicsParticleInfo
{
	PhysicsParticleInfo(
		PhysicsParticleType type,
		unsigned int playerId,
		void *data) :
		type_(type),
		playerId_(playerId),
		data_(data)
	{
	}

	PhysicsParticleType type_;
	unsigned int playerId_;
	void *data_;
};

class PhysicsParticleObject;
class PhysicsParticleObjectHandler
{
public:
	virtual void collision(PhysicsParticleObject &position, 
		ScorchedCollisionId collisionId) = 0;
	virtual void wallCollision(PhysicsParticleObject &position,
		ScorchedCollisionId collisionId) {}
};

class Target;
class ScorchedContext;
class PhysicsParticleObject
{
public:
	PhysicsParticleObject();
	virtual ~PhysicsParticleObject();

	void setPhysics(
		PhysicsParticleInfo info,
		ScorchedContext &context, 
		FixedVector &position, FixedVector &velocity,
		fixed sphereSize = 0,
		fixed sphereDensity = 0,
		fixed windFactor = 1,
		bool underGroundCollision = false,
		bool rotateOnCollision = false,
		bool wallCollision = true);

	void applyForce(FixedVector &force);
	void simulate(fixed frameTime);

	FixedVector &getPosition() { return position_; }
	FixedVector &getVelocity() { return velocity_; }
	FixedVector4 &getRotationQuat() { return rotation_; }

	void setHandler(PhysicsParticleObjectHandler *handler) { handler_ = handler; }
	void setPosition(FixedVector &position) { position_ = position; }

protected:
	PhysicsParticleInfo info_;
	ScorchedContext *context_;
	PhysicsParticleObjectHandler *handler_;
	bool underGroundCollision_;
	bool rotateOnCollision_;
	bool wallCollision_;
	unsigned int iterations_;
	FixedVector position_;
	FixedVector velocity_;
	FixedVector windFactor_;
	FixedVector4 rotation_;
	FixedVector4 avelocity_;

	enum CollisionAction
	{
		CollisionActionNone = 0,
		CollisionActionCollision,
		CollisionActionBounce
	};
	struct CollisionInfo
	{
		ScorchedCollisionId collisionId;
		fixed deflectFactor;
		FixedVector normal;
	};

	virtual void shotWallHit(CollisionInfo &collision);
	virtual void shotShieldHit(Target *target);
	virtual void bounceShieldHit(Target *target);

	void checkCollision();
	CollisionAction checkShotCollision(CollisionInfo &collision, Target *target);
	CollisionAction checkBounceCollision(CollisionInfo &collision, Target *target);
	CollisionAction checkFallingCollision(CollisionInfo &collision, Target *target);

	bool getLandscapeCollision(CollisionInfo &collision);
	bool getRoofCollision(CollisionInfo &collision);
	bool getWallCollision(CollisionInfo &collision);
	bool getShieldCollision(CollisionInfo &collision, Target *target);
	bool getTargetCollision(CollisionInfo &collision, Target *target);
	bool getTargetBounceCollision(CollisionInfo &collision, Target *target);
};

class PhysicsParticleActionObject : public PhysicsParticleObject
{
public:
	PhysicsParticleActionObject();
	virtual ~PhysicsParticleActionObject();

protected:
	virtual void shotWallHit(CollisionInfo &collision);
	virtual void shotShieldHit(Target *target);
	virtual void bounceShieldHit(Target *target);
};

#endif
