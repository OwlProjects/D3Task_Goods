#ifndef UNIXTIMEDELEGATE_H
#define UNIXTIMEDELEGATE_H

#include <QStyledItemDelegate>

class UnixTimeDelegate : public QStyledItemDelegate
{
public:
  explicit UnixTimeDelegate(QObject* parent = nullptr);

  // QAbstractItemDelegate interface
public:
  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

};

#endif // UNIXTIMEDELEGATE_H
