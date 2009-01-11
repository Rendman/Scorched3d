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

#if !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
#define AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_

#include <map>
#include <set>
#include <vector>
#include <weapons/Weapon.h>
#include <GLW/GLWFlag.h>
#include <GLW/GLWWindow.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWPanel.h>
#include <GLW/GLWTab.h>
#include <GLW/GLWCheckBoxText.h>
#include <GLW/GLWDropDownText.h>

class Tank;
class BuyAccessoryDialog : public GLWWindow,
						public GLWButtonI,
						public GLWCheckBoxI,
						public GLWDropDownI,
						public GLWTabI
{
public:
	static BuyAccessoryDialog *instance();

	// Inherited from GLWButtonI
	virtual void draw();
	virtual void buttonDown(unsigned int id);
	virtual void display();

	// Inherited from GLWCheckBoxI
	virtual void stateChange(bool state, unsigned int id);

	// Inherited from GLWDropDownI
	virtual void select(unsigned int id, const int pos, GLWSelectorEntry value);

	// Inherited from GLWTabI
	virtual void tabDown(unsigned int id);

	void playerRefreshKeepPos();

protected:
	static BuyAccessoryDialog *instance_;
	unsigned int okId_, giftId_;
	bool firstDrawTime_;
	std::map<std::string, GLWTab *> buyTabs_;
	GLWTab *sellTab_;
	GLWTab *favouritesTab_;
	GLWPanel *topPanel_;
	GLWDropDownText *sortDropDown_;
	GLWCheckBoxText *defaultTab_;
	GLWFlag *flag_;
	std::map<unsigned int, Accessory *> sellMap_;
	std::map<unsigned int, Accessory *> buyMap_;
	std::map<unsigned int, Accessory *> favMap_;
	std::set<std::string> favorites_;

	void addTabs();
	void loadFavorites();
	void saveFavorites();
	void playerRefresh();
	void addPlayerName();
	void addPlayerWeapons();
	void addPlayerWeaponsSell();
	void addPlayerFavorites();
	void addPlayerWeaponsBuy(GLWTab *tab, const char *group);
	bool addAccessory(Tank *tank, GLWTab *tab, float height, Accessory *current);

private:
	BuyAccessoryDialog();
	virtual ~BuyAccessoryDialog();
};

#endif // !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
