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

#include <actions/Lightning.h>
#include <common/Defines.h>
#include <common/RandomGenerator.h>
#ifndef S3D_SERVER
	#include <sound/SoundUtils.h>
	#include <graph/TextureStore.h>
	#include <GLEXT/GLState.h>
	#include <GLEXT/GLCamera.h>
	#include <sprites/ExplosionTextures.h>
#endif
#include <target/TargetContainer.h>
#include <target/TargetDamageCalc.h>
#include <target/TargetLife.h>
#include <target/TargetSpace.h>
#include <engine/ActionController.h>
#include <weapons/AccessoryStore.h>
#include <math.h>

Lightning::Lightning(WeaponLightning *weapon,
		WeaponFireContext &weaponContext,
		FixedVector &position, FixedVector &velocity) :
	ActionReferenced("Lightning"),
	totalTime_(0),
	weapon_(weapon),
	weaponContext_(weaponContext),
	position_(position), velocity_(velocity),
	texture_(0)
{
}

Lightning::~Lightning()
{
}

void Lightning::init()
{
	FixedVector direction = velocity_.Normalize();
	std::map<unsigned int, fixed> hurtMap;

	generateLightning(0, 1, weapon_->getSize(), 
		position_, direction, position_, direction,
		hurtMap);

	std::map<unsigned int, fixed>::iterator hurtItor;
	for (hurtItor = hurtMap.begin();
		hurtItor != hurtMap.end();
		hurtItor++)
	{
		unsigned int playerId = (*hurtItor).first;
		fixed damage = (*hurtItor).second;

		Target *target = context_->getTargetContainer().getTargetById(playerId);
		if (target)
		{
			TargetDamageCalc::damageTarget(
				*context_, target, weapon_, weaponContext_, 
				damage, true, false, false);
		}
	}
}

std::string Lightning::getActionDetails()
{
	return S3D::formatStringBuffer("%i,%i,%i %i,%i,%i %s",
		position_[0].getInternal(), position_[1].getInternal(), position_[2].getInternal(),
		velocity_[0].getInternal(), velocity_[1].getInternal(), velocity_[2].getInternal(),
		weapon_->getParent()->getName());
}

void Lightning::simulate(fixed frameTime, bool &remove)
{
#ifndef S3D_SERVER
	if (!context_->getServerMode())
	{   
		if (firstTime_)
		{ 
			firstTime_ = false;
			if (weapon_->getSound() &&
				0 != strcmp("none", weapon_->getSound()))
			{
				SoundBuffer *expSound =
					Sound::instance()->fetchOrCreateBuffer(
						S3D::getDataFile(weapon_->getSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					expSound, position_.asVector());
			}
		} 
	}
#endif // #ifndef S3D_SERVER

	totalTime_ += frameTime;
	remove = (totalTime_ > weapon_->getTotalTime());
	Action::simulate(frameTime, remove);
}

void Lightning::draw()
{
#ifndef S3D_SERVER
	if (!context_->getServerMode())
	{
		Vector &cameraPos = 
			GLCamera::getCurrentCamera()->getCurrentPos();

		GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);
		glColor4f(1.0f, 1.0f, 1.0f, 
			1.0f - totalTime_.asFloat() / weapon_->getTotalTime().asFloat());

		if (!texture_)
		{
			std::string file4 = S3D::getDataFile(weapon_->getTexture());
			texture_ = TextureStore::instance()->loadTexture(
				file4.c_str(), file4.c_str(), false);
		}
		texture_->draw();
		
		Vector offset(0.0f, 0.0f, 0.5f);
		bool began = false; 
		float texCoord = 0.0f;
		std::list<Segment>::iterator itor;
		for (itor = segments_.begin();
			itor != segments_.end();
			itor++)
		{
			Segment &segment = (*itor);
			Vector cameraDir = (segment.start.asVector() - cameraPos).Normalize();
			Vector offset = (segment.direction.asVector() * cameraDir).Normalize();
			offset *= 0.4f;

			if (!began)
			{
				began = true;
				glBegin(GL_QUAD_STRIP);
			}

			glTexCoord2f(1.0f, texCoord);
			glVertex3fv(segment.start.asVector() + offset * segment.size.asFloat());
			glTexCoord2f(0.0f, texCoord);
			glVertex3fv(segment.start.asVector() - offset * segment.size.asFloat());
			texCoord += 1.0f;

			if (segment.endsegment)
			{
				glTexCoord2f(0.0f, texCoord);
				glVertex3fv(segment.end.asVector() - offset * segment.size.asFloat());
				glTexCoord2f(1.0f, texCoord);
				glVertex3fv(segment.end.asVector() + offset * segment.size.asFloat());
				texCoord += 1.0f;

				began = false;
				glEnd();
			}
		}

		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#endif // #ifndef S3D_SERVER
}

void Lightning::dispaceDirection(FixedVector &direction, 
	FixedVector &originalDirection, fixed angle)
{
	RandomGenerator &generator = context_->getActionController().getRandom();

	int breakCount = 0;

	FixedVector newdir;
	while (breakCount++ < 1000)
	{
		newdir[0] = (generator.getRandFixed() - fixed(true, 5000)) * 2;
		newdir[1] = (generator.getRandFixed() - fixed(true, 5000)) * 2;
		newdir[2] = (generator.getRandFixed() - fixed(true, 5000)) * 2;
		newdir.StoreNormalize();

		fixed a = newdir[0] * direction[0] + 
			newdir[1] * direction[1] + 
			newdir[2] * direction[2];
		if (a < angle) continue;

		fixed b = newdir[0] * originalDirection[0] + 
			newdir[1] * originalDirection[1] + 
			newdir[2] * originalDirection[2];
		if (b >= weapon_->getDerivAngle())
		{
			direction = newdir;
			return;
		}
	}	
}

void Lightning::generateLightning(int id, int depth, fixed size, 
	FixedVector &originalPosition, FixedVector &originalDirection,
	FixedVector &start, FixedVector &direction,
	std::map<unsigned int, fixed> &hurtMap)
{
	if (id > 100) return;

	RandomGenerator &generator = context_->getActionController().getRandom();
	fixed length = weapon_->getSegLength() + 
		weapon_->getSegVar() * generator.getRandFixed();
	FixedVector end = start + direction * length;

	// Add the new lightning segment
	segments_.push_back(Segment());
	Segment &segment = segments_.back();

	// Set the segment
	segment.start = start;
	segment.end = end;
	segment.size = size;
	segment.direction = direction;
	segment.endsegment = false;

	// Damage any tanks
	damageTargets(segment.end, hurtMap);

	// Rand posibility that we stop
	if (depth > 1 && generator.getRandFixed() < 
		weapon_->getDeathProb())
	{
		segment.endsegment = true;
		return;
	}

	// Check if we have gone too far
	if ((originalPosition - end).Magnitude() > 
		weapon_->getConeLength()) 
	{
		segment.endsegment = true;
		return;
	}

	// Continue this lightning strand
	{
		FixedVector newdirection = end - start;
		dispaceDirection(newdirection, originalDirection, 
			weapon_->getAngleVar() * fixed(true, 2500));
		generateLightning(id + 1, depth, size, 
			originalPosition, originalDirection, 
			end, newdirection,
			hurtMap);	
	}

	// Make a new strand
	if (generator.getRandFixed() <= 
		weapon_->getSplitProb() - fixed(depth - 1) * weapon_->getSplitVar())
    {
		fixed newsize = size + weapon_->getSizeVar();
		if (newsize < weapon_->getMinSize()) 
			newsize = weapon_->getMinSize();

		FixedVector newdirection = end - start;
		dispaceDirection(newdirection, originalDirection, 
			weapon_->getAngleVar());
		generateLightning(id + 1, depth + 1, newsize, 
			originalPosition, originalDirection, 
			end, newdirection,
			hurtMap);	
	}
}

void Lightning::damageTargets(FixedVector &position, 
		std::map<unsigned int, fixed> &hurtMap)
{
	if (weapon_->getSegHurt() <= 0) return;

	std::map<unsigned int, Target *> collisionTargets;
	context_->getTargetSpace().getCollisionSet(position, 
		weapon_->getSegHurtRadius() * fixed(true, 15000), collisionTargets);
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = collisionTargets.begin();
		itor != collisionTargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		if (target->getAlive() &&
			target->getPlayerId() != weaponContext_.getPlayerId())
		{
			fixed distance = (target->getLife().getTargetPosition() -
				position).Magnitude();
			if (distance < weapon_->getSegHurtRadius() + 
				MAX(target->getLife().getSize()[0], target->getLife().getSize()[1]))
			{
				std::map<unsigned int, fixed>::iterator findItor = 
					hurtMap.find(target->getPlayerId());
				if (findItor == hurtMap.end())
				{
					hurtMap[target->getPlayerId()] = weapon_->getSegHurt();
				}
				else
				{
					hurtMap[target->getPlayerId()] += weapon_->getSegHurt();
				}
			}
		}
	}
}