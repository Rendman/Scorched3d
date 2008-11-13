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

#include <dialogs/CameraDialog.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLCameraFrustum.h>
#include <common/DefinesString.h>
#include <graph/Main2DCamera.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <landscape/Landscape.h>
#include <tankgraph/RenderTargets.h>
#include <engine/ActionController.h>

CameraDialog *CameraDialog::instance_ = 0;

CameraDialog *CameraDialog::instance()
{
	if (!instance_)
	{
		instance_ = new CameraDialog;
	}

	return instance_;
}

CameraDialog::CameraDialog() : 
	GLWWindow("Camera", 100, 15, 200, 200, eTransparent | 
	eResizeable | eSmallTitle | eSavePosition,
	"Shows a view from behind the current tank\n"
	"or its shots in a different window")
{
	targetCam_.setCameraType(TargetCamera::CamGun);
}

CameraDialog::~CameraDialog()
{
}

void CameraDialog::savePosition(XMLNode *node)
{
	GLWWindow::savePosition(node);

	node->addChild(new XMLNode("w", int(getW())));
	node->addChild(new XMLNode("h", int(getH())));
	node->addChild(new XMLNode("type", int(targetCam_.getCameraType())));
	
}

void CameraDialog::loadPosition(XMLNode *node)
{
	GLWWindow::loadPosition(node);

	int w, h, type;
	if (!node->getNamedChild("w", w)) return;
	if (!node->getNamedChild("h", h)) return;
	if (!node->getNamedChild("type", type)) return;

	setW(float(w));
	setH(float(h));
	targetCam_.setCameraType((TargetCamera::CamType) type);
}

void CameraDialog::draw()
{
	const float inset = 5.0f;
	const float corner = 20.0f;

	GLWWindow::draw();
	
	// Add the rounded corners
	{
		GLState newState(GLState::DEPTH_ON | GLState::TEXTURE_OFF | GLState::BLEND_ON);
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 4000.0f);	
			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + inset - 2.0f, y_ + inset - 2.0f);
				drawCircle(7, 13, x_ + inset + corner, 
					y_ + inset + corner, corner);
			glEnd();
		
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + w_ - inset + 2.0f, y_ + inset - 2.0f);
				drawCircle(3, 9, x_ + w_ - inset - corner, 
					y_ + inset + corner, corner);
			glEnd();
			
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + w_ - inset + 2.0f, y_ + h_ - inset + 2.0f);
				drawCircle(-1, 5, x_ + w_ - inset - corner, 
					y_ + h_ - inset - corner, corner);
			glEnd();
			
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + inset - 2.0f, y_ + h_ - inset + 2.0f);
				drawCircle(11, 17, x_ + inset + corner, 
					y_ + h_ - inset - corner, corner);
			glEnd();
		glPopMatrix();
	}
	
	// Set the new viewport
	targetCam_.getCamera().setWindowSize(
		int(float(w_ - inset - inset) / GLViewPort::getWidthMult()), 
		int(float(h_ - inset - inset) / GLViewPort::getHeightMult())); 
	targetCam_.getCamera().setWindowOffset(
		int(float(x_ + inset) / GLViewPort::getWidthMult()), 
		int(float(y_ + inset) / GLViewPort::getHeightMult()));
	targetCam_.draw();
	
	// Draw the stuff we want to see
	//GLCameraFrustum::instance()->backupFrustum();
	drawLandscape();
	//GLCameraFrustum::instance()->restoreFrustum();
	
	// Return the viewport to the original
	Main2DCamera::instance()->draw(0);

	// Draw the camera name
	GLState newState(GLState::TEXTURE_OFF);
	Vector col(0.7f, 0.7f, 0.7f);
	GLWFont::instance()->getGameFont()->
		draw(col, 10.0f, x_ + 15.0f, y_ + 15.0f, 0.0f, 
		S3D::formatStringBuffer("%s Camera",
		targetCam_.getCameraNames()[targetCam_.getCameraType()]));
}

void CameraDialog::simulate(float frameTime)
{
	bool playing = (ScorchedClient::instance()->getGameState().getState() ==
					ClientState::StatePlaying);

	// Simulate this viewport
	targetCam_.simulate(frameTime, playing);
}

void CameraDialog::drawLandscape()
{
	GLCameraFrustum::instance()->draw(0);
	Landscape::instance()->drawLand();
	RenderTargets::instance()->render3D.draw(0);
	Landscape::instance()->drawWater();
	Landscape::instance()->drawObjects();
	ScorchedClient::instance()->getActionController().draw(0);
	ScorchedClient::instance()->getParticleEngine().draw(0);
	targetCam_.drawPrecipitation();
}

void CameraDialog::mouseDown(int button, float x, float y, bool &skipRest)
{
	GLWWindow::mouseDown(button, x, y, skipRest);
	if (x > x_ && x < x_ + 120 && y > y_ && y < y_ + 40)
	{
		int type = (int) targetCam_.getCameraType(); type++;
		if (type >= (int) TargetCamera::CamFree) type = (int) TargetCamera::CamTop;
		targetCam_.setCameraType((TargetCamera::CamType) type);
	}
}
