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
		//TODO: need to cache this operation.
		return QString::number( mMtx(index.row(), index.column()) );
	}
	return QVariant();
}

bool NetMatrixModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole)
		{
			//save value from editor to member m_gridData
			mMtx(index.row(),index.column()) = value.toInt();
			//for presentation purposes only: build and emit a joined string
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
	if (role == Qt::DisplayRole)
	{
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

void NetMatrixModel::startUpdate()
{
	update += 1;
}

void NetMatrixModel::endUpdate()
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

void NetMatrixModel::resizeMatrix(int rows, int cols)
{
	auto oldRows = mMtx.rows();
	auto oldCols = mMtx.cols();
	mMtx.conservativeResizeLike(Eigen::MatrixXi::Zero(rows,cols));

	if (update == 0) {
		emit layoutChanged();
	}
}

void NetMatrixModel::updateMatrixValue(int row, int col, int val)
{
	if (row > mMtx.rows() || col > mMtx.cols()) {
		resizeMatrix(row > mMtx.rows() ? row : mMtx.rows(),
					 col > mMtx.cols() ? col : mMtx.cols());
	}
	mMtx(row,col) = val;
	if (update) {
		QModelIndex topLeft = createIndex(row,col);
		emit dataChanged(topLeft, topLeft);
	}
}

