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

#if !defined(__INCLUDE_GLWChannelTexth_INCLUDE__)
#define __INCLUDE_GLWChannelTexth_INCLUDE__

#include <GLW/GLWChannelView.h>
#include <GLW/GLWIconButton.h>
#include <GLW/GLWSelector.h>
#include <string>

class GLWChannelText : 
	public GLWidget, 
	public GLWButtonI, 
	public GLWSelectorI, 
	public GLWChannelViewI
{
public:
	GLWChannelText();
	virtual ~GLWChannelText();

	// GLWChannelViewI
	virtual void channelsChanged(unsigned int id);

	// GLWSelectorI
	virtual void itemSelected(GLWSelectorEntry *entry, int position);

	// GLWButtonI
	virtual void buttonDown(unsigned int id);

	// GLWidget
	virtual void draw();
	virtual void simulate(float frameTime);
	virtual bool initFromXML(XMLNode *node);
	virtual void mouseDown(int button, float x, float y, bool &skipRest);
	virtual void mouseUp(int button, float x, float y, bool &skipRest);
	virtual void mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);
	virtual void setParent(GLWPanel *parent);
	virtual void setX(float x);
	virtual void setY(float y);
	virtual void setW(float w);
	virtual void setH(float h);

	REGISTER_CLASS_HEADER(GLWChannelText);
protected:
	static std::list<ChannelText> lastMessages_;
	GLTexture buttonTexture_, colorTexture_;
	GLWChannelViewTextRenderer prompt_;
	GLWChannelView::CurrentChannelEntry channelEntry_;
	GLWIconButton button_;
	GLWChannelView view_;
	std::map<KeyboardKey *, std::string> keys_;
	std::string text_;
	float fontSize_, outlineFontSize_;
	float ctime_;
	bool cursor_, visible_;
	int maxTextLen_;
	unsigned int whisperDest_;
	std::string whisperDestStr_;

	void processNotVisibleKey(char c, unsigned int dik, bool &skipRest);
	void processVisibleKey(char c, unsigned int dik);
	void processSpecialText();
	void processNormalText();
	bool checkCurrentChannel();
	bool channelValid(const char *channelName);
	void setVisible(bool visible);
	void setChannelEntry(GLWChannelView::CurrentChannelEntry &entry);
};

#endif // __INCLUDE_GLWChannelTexth_INCLUDE__