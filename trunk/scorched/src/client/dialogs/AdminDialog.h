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

#if !defined(__INCLUDE_AdminDialogh_INCLUDE__)
#define __INCLUDE_AdminDialogh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWIconTable.h>
#include <GLW/GLWTab.h>
#include <GLW/GLWDropDown.h>

class AdminDialog : 
	public GLWWindow,
	public GLWButtonI,
	public GLWIconTableI
{
public:
	static AdminDialog *instance();

	// GLWWindow
	virtual void display();

	// GLWButtonI
	virtual void buttonDown(unsigned int id);

	// GLWIconTableI
	virtual void drawColumn(unsigned int id, int row, int column, float x, float y, float w);
	virtual void rowSelected(unsigned int id, int row);
	virtual void rowChosen(unsigned int id, int row);
	virtual void columnSelected(unsigned int id, int col);

protected:
	static AdminDialog *instance_;
	GLWIconTable *adminTable_;
	GLWTextButton *ok_;
	GLWButton *kickButton_, *banButton_, *slapButton_;
	GLWButton *poorButton_, *muteButton_, *unmuteButton_;
	GLWTab *playerTab_, *botsTab_;
	GLWDropDown *aiSelector_;
	GLWButton *addButton_;

private:
	AdminDialog();
	virtual ~AdminDialog();

};

#endif