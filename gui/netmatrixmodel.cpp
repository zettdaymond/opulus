#include <netmatrixmodel.h>
#include <QDebug>
#include <QTime>

NetMatrixModel::NetMatrixModel(QObject* parent) :
	QAbstractTableModel(parent),
	mMtx(0,0)
{}

int NetMatrixModel::rowCount(const QModelIndex&) const
{
	return mMtx.rows();
}

int NetMatrixModel::columnCount(const QModelIndex&) const
{
	return mMtx.cols();
}

QVariant NetMatrixModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::TextAlignmentRole ) {
		return Qt::AlignCenter;
	}
	if (role == Qt::DisplayRole) {
		Q_ASSERT(index.row() < mMtx.rows() && index.column() < mMtx.cols());
		return QString::number( mMtx(index.row(), index.column()) );
	}
	return QVariant();
}

bool NetMatrixModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole) {
		mMtx(index.row(),index.column()) = value.toInt();
		emit mtxValueChanged( index.row(), index.column(), value.toInt() );
	}
	return true;
}

Qt::ItemFlags NetMatrixModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

QVariant NetMatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return QString("P") + QString::number(section);
		} else if (orientation == Qt::Vertical) {
			return QString("T") + QString::number(section);
		}
	}
	return QVariant();
}

void NetMatrixModel::updateMatrix(Eigen::MatrixXi& mtx)
{
	mMtx = mtx;

	if (update == 0) {
		emit layoutChanged();
	}
}

void NetMatrixModel::startUpdateTransaction()
{
	update += 1;
}

void NetMatrixModel::stopUpdateTransaction()
{
	if (update > 0) {
		update -= 1;
	}
	if (update == 0) {
		emit layoutChanged();
	}
}

const int NetMatrixModel::val(int row, int col) const
{
	Q_ASSERT(row < mMtx.rows() && col < mMtx.cols());
	return mMtx(row,col);
}


