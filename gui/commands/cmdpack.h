#ifndef COMMANDPACK_H
#define COMMANDPACK_H

#include <QUndoCommand>
#include <QCoreApplication>
#include <QVector>

class CmdPackNotifier : public QObject
{
	Q_OBJECT
signals:
	void beginPackageUpdate();
	void endPackageUpdate();
};

class CmdPack : public QUndoCommand {
	Q_DECLARE_TR_FUNCTIONS(CmdPack)

	using CmdPtr = QUndoCommand*;
public:
	CmdPack();
    CmdPack(QVector<CmdPtr>& cmds);
	virtual ~CmdPack();

	void pushBack(QUndoCommand* c);
	void popBack();
    int size();
	void undo() override;
	void redo() override;
private:
	QVector<CmdPtr> mCmds;
public:
	CmdPackNotifier* mNotifier;
};

#endif //COMMANDPACK_H
