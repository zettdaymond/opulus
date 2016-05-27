#include "cmdpack.h"

CmdPack::CmdPack(QVector<CmdPtr>& cmds) :
	mCmds(cmds)
{}

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
	for (int i = mCmds.size() - 1; i >= 0; i--) {
		mCmds[i]->undo();
	}
}

void CmdPack::redo()
{
	for (auto cmd : mCmds) {
		cmd->redo();
	}
}

