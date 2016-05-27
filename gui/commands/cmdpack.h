#ifndef COMMANDPACK_H
#define COMMANDPACK_H

#include <QUndoCommand>
#include <QCoreApplication>
#include <QVector>

class CmdPack : public QUndoCommand {
	Q_DECLARE_TR_FUNCTIONS(CmdPack)
	using CmdPtr = QUndoCommand*;
public:
	CmdPack() = default;
    CmdPack(QVector<CmdPtr>& cmds);
	virtual ~CmdPack();

	void pushBack(QUndoCommand* c);
	void popBack();
    int size();
	void undo() override;
	void redo() override;
private:
	QVector<CmdPtr> mCmds;
};

#endif //COMMANDPACK_H
