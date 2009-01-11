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

#include <sky/Sun.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapemap/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <common/OptionsScorched.h>
#include <graph/OptionsDisplay.h>
#include <graph/MainCamera.h>
#include <common/Defines.h>
#include <common/Vector4.h>
#include <image/ImageFactory.h>
#include <GLEXT/GLLenseFlare.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <stdlib.h>
#include <math.h>

Sun::Sun()
{
}

Sun::~Sun()
{
}

void Sun::setPosition(float sunRotXY, float sunRotYZ)
{
	LandscapeDefn &defn = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getDefn();

	sunRotXY = sunRotXY / 180.0f * 3.14f;
	sunRotYZ = sunRotYZ / 180.0f * 3.14f;
	position_ = Vector(
		(sinf(sunRotXY) * 900.0f * cosf(sunRotYZ)) + defn.getLandscapeWidth() / 2.0f, 
		(cosf(sunRotXY) * 900.0f * cosf(sunRotYZ)) + defn.getLandscapeHeight() / 2.0f, 
		(sinf(sunRotYZ) * 900.0f));
}

void Sun::generate()
{
	LandscapeTex &tex = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();

	std::string file = S3D::getDataFile(tex.suntexture.c_str());
	if (!tex.suntexturemask.c_str()[0])
	{
		ImageHandle map = ImageFactory::loadImageHandle(file.c_str(), file.c_str(), false);
		DIALOG_ASSERT(texture_.replace(map, true));
	}
	else
	{
		std::string mask = S3D::getDataFile(tex.suntexturemask.c_str());
		ImageHandle map = ImageFactory::loadImageHandle(file.c_str(), mask.c_str(), true);
		DIALOG_ASSERT(texture_.replace(map, true));
	}
}

void Sun::setLightPosition(bool light0)
{
	LandscapeTex &tex = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();

	Vector4 sunPosition = getPosition();
	if (light0)
	{
		Vector &cameraPos = MainCamera::instance()->getTarget().getCamera().getCurrentPos();
		sunPosition = getPosition();
		sunPosition[0] -= cameraPos[0];
		sunPosition[1] -= cameraPos[1];
	}

	Vector4 sunDiffuse = tex.skydiffuse;
	Vector4 sunAmbient = tex.skyambience;

	GLenum lightNo = light0?GL_LIGHT0:GL_LIGHT1;
	glLightfv(lightNo, GL_AMBIENT, sunAmbient);
	glLightfv(lightNo, GL_DIFFUSE, sunDiffuse);
	glLightfv(lightNo, GL_POSITION, sunPosition);
}

void Sun::draw()
{
	GLState currentStateOne(GLState::TEXTURE_ON | GLState::DEPTH_OFF | GLState::BLEND_ON);
	texture_.draw();

	LandscapeTex &tex = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();

	GLCameraFrustum::instance()->drawBilboard(
		position_, 
		tex.suncolor, 
		1.0f, // alpha
		60.0f, 60.0f, // width, height
		!tex.nosunblend, // additive texture
		0); // tex coord
}
