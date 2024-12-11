#include "UnixTimeDelegate.h"

#include <QDateTime>
#include <QDateTimeEdit>
#include <QTextOption>
#include <QPainter>

UnixTimeDelegate::UnixTimeDelegate(QObject* parent):
  QStyledItemDelegate(parent)
{

}

void UnixTimeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QVariant data = index.data();
  if (data.canConvert<quint64>()) {
    quint64 msecs = data.toULongLong() * 1000;
    QString timeStr = QDateTime::fromMSecsSinceEpoch(msecs).toString("dd.MM.yyyy hh:mm");
    QTextOption opts(Qt::AlignCenter);
    painter->drawText(option.rect, timeStr, opts);
    return;
  }
  QStyledItemDelegate::paint(painter, option, index);
}

QWidget *UnixTimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QVariant data = index.data();
  if (data.canConvert<quint64>()) {
    quint64 msecs = data.toULongLong() * 1000;
    return new QDateTimeEdit(QDateTime::fromMSecsSinceEpoch(msecs), parent);
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void UnixTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QDateTimeEdit* edit = qobject_cast<QDateTimeEdit*>(editor);
  if (edit) {
    QVariant data = index.data();
    quint64 msecs = data.toULongLong() * 1000;
    edit->setDateTime(QDateTime::fromMSecsSinceEpoch(msecs));
  }
}

void UnixTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QDateTimeEdit* edit = qobject_cast<QDateTimeEdit*>(editor);
  if (edit) {
    quint64 secs = edit->dateTime().toMSecsSinceEpoch() / 1000;
    model->setData(index, secs);
  }
}
