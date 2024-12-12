#ifndef PROPERTIESDELEGATE_H
#define PROPERTIESDELEGATE_H

#include <QStyledItemDelegate>

class PropertiesDelegate : public QStyledItemDelegate
{
public: // c-tors/d-tors
  explicit PropertiesDelegate(QObject* parent = nullptr);

public: // functions
  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // PROPERTIESDELEGATE_H
