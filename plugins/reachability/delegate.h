#include <QItemDelegate>

class Delegate : public QItemDelegate
{
public:

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
};
