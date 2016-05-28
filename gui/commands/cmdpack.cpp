#include "cmdpack.h"

CmdPack::CmdPack() : QUndoCommand()
{
	mNotifier = new CmdPackNotifier();
}

CmdPack::CmdPack(QVector<CmdPtr>& cmds) :
	mCmds(cmds)
{
	mNotifier = new CmdPackNotifier();
}

CmdPack::~CmdPack()
{
	for(auto p : mCmds)	 {
		delete p;
	}
	delete mNotifier;
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
	emit mNotifier->beginPackageUpdate();
	for (int i = mCmds.size() - 1; i >= 0; i--) {
		mCmds[i]->undo();
	}
	emit mNotifier->endPackageUpdate();
}

void CmdPack::redo()
{
	emit mNotifier->beginPackageUpdate();
	for (auto cmd : mCmds) {
		cmd->redo();
	}
	emit mNotifier->endPackageUpdate();
}

