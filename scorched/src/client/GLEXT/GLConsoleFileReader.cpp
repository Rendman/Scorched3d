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

#include <GLEXT/GLConsole.h>
#include <GLEXT/GLConsoleFileReader.h>
#include <GLEXT/GLConsole.h>
#include <common/FileLines.h>
#include <common/DefinesString.h>
#include <XML/XMLFile.h>
#include <XML/XMLParser.h>
#include <stdio.h>

bool GLConsoleFileReader::loadFileIntoConsole(const std::string &fileName,
											  std::string &errorMessage)
{
	XMLFile file;
	if (!file.readFile(fileName))
	{
		errorMessage = file.getParserError();
		return false;
	}
	if (!file.getRootNode()) return true;

	// Itterate all of the commands in the file
	std::list<XMLNode *>::iterator childrenItor;
	for (childrenItor = file.getRootNode()->getChildren().begin();
		childrenItor != file.getRootNode()->getChildren().end();
		childrenItor++)
	{
		XMLNode *currentNode = (*childrenItor);		
		if (strcmp(currentNode->getName(), "command")==0)
		{
			GLConsole::instance()->addLine(true, currentNode->getContent());
		}
	}
	return true;
}

void GLConsoleFileReader::saveConsoleIntoFile(const std::string &filename)
{
	FileLines filelines;
	filelines.addLine("<commands source=\"Scorched3D\">");

	std::deque<GLConsoleLine *> &lines = GLConsole::instance()->getLines();
	std::deque<GLConsoleLine *>::iterator itor;
	for (itor = lines.begin();
		itor != lines.end();
		itor++)
	{
		std::string cleanLine;
		std::string dirtyLine((*itor)->getLine());
		XMLNode::removeSpecialChars(dirtyLine, cleanLine);
		if ((*itor)->getLineType() != GLConsoleLine::eNone)
		{
			filelines.addLine(S3D::formatStringBuffer("  <command>%s</command>",
				cleanLine.c_str()));
		}
		else
		{
			filelines.addLine(S3D::formatStringBuffer("  <!-- %s -->",
				cleanLine.c_str()));
		}
	}

	filelines.addLine("</commands>");
	filelines.writeFile(filename);
}