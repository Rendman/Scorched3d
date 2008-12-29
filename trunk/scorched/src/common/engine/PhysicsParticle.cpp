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

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedContext.h>

PhysicsParticle::PhysicsParticle()  : 
	Action("PhysicsParticle"),
	collision_(false), totalActionTime_(0)
{

}

PhysicsParticle::~PhysicsParticle()
{

}

void PhysicsParticle::setPhysics(
	PhysicsParticleInfo info,
	FixedVector &position, FixedVector &velocity,
	fixed sphereSize, fixed sphereDensity, fixed windFactor,
	bool underGroundCollision, bool rotateOnCollision,
	bool wallCollision)
{
	physicsObject_.setPhysics(
		info,
		*context_, 
		position, velocity,
		sphereSize, sphereDensity, windFactor,
		underGroundCollision,
		rotateOnCollision,
		wallCollision);
	physicsObject_.setHandler(this);
}

void PhysicsParticle::applyForce(FixedVector &force)
{
	physicsObject_.applyForce(force);
}

void PhysicsParticle::collision(PhysicsParticleObject &position, 
	ScorchedCollisionId collisionId)
{
	collision_ = true;
}

void PhysicsParticle::setCurrentPosition(FixedVector &position)
{
	physicsObject_.setPosition(position);
}

FixedVector &PhysicsParticle::getCurrentPosition()
{
	return physicsObject_.getPosition();
}

FixedVector &PhysicsParticle::getCurrentVelocity()
{
	return physicsObject_.getVelocity();
}

FixedVector4 &PhysicsParticle::getRotationQuat()
{
	return physicsObject_.getRotationQuat();
}

void PhysicsParticle::simulate(fixed frameTime, bool &remove)
{
	physicsObject_.simulate(frameTime);
	Action::simulate(frameTime, remove);
	if (collision_) remove = true;
	totalActionTime_ += frameTime;
	if (totalActionTime_ > 30) remove = true;
}

PhysicsParticleReferenced::PhysicsParticleReferenced()
{
}

PhysicsParticleReferenced::~PhysicsParticleReferenced()
{
}


