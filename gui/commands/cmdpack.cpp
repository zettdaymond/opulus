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

