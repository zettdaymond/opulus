/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "cmdpack.h"
#include "controller.h"

CmdPack::CmdPack(Controller* c) :
	mController(c)
{
	Q_ASSERT(c);
}

CmdPack::CmdPack(Controller* c, QVector<CmdPtr>& cmds) :
	mController(c),
	mCmds(cmds)
{
	Q_ASSERT(c);
}

CmdPack::~CmdPack()
{
	for(auto p : mCmds)	 {
		delete p;
	}
}

void CmdPack::pushBack(QUndoCommand* c)
{
	mCmds.push_back(CmdPtr(c));
}

void CmdPack::popBack()
{
    mCmds.pop_back();
}

int CmdPack::size()
{
    return mCmds.size();
}

void CmdPack::undo()
{
	mController->disableGuiNotifications();
	for (int i = mCmds.size() - 1; i >= 0; i--) {
		mCmds[i]->undo();
	}
	mController->enableGuiNotifications();
}

void CmdPack::redo()
{
	mController->disableGuiNotifications();
	for (auto cmd : mCmds) {
		cmd->redo();
	}
	mController->enableGuiNotifications();
}

