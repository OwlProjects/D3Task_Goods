#include "PropertiesDelegate.h"
#include "PropertiesWidget.h"

#include <QTextOption>
#include <QPainter>
#include <QStringBuilder>
#include <QDateTime>

PropertiesDelegate::PropertiesDelegate(QObject *parent):
  QStyledItemDelegate(parent)
{

}

void PropertiesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QVariant data = index.data();
  if (data.canConvert<int>()) {
    int props = data.toInt();
    QString propsStr;
    propsStr += props & (1 << 0) ? " Л" : "";
    propsStr += props & (1 << 1) ? " П" : "";
    propsStr += props & (1 << 2) ? " А" : "";
    propsStr += props & (1 << 3) ? " В" : "";
    propsStr += props & (1 << 4) ? " К" : "";
    QTextOption opts(Qt::AlignCenter);
    // Примечание
    //  Код по установке цветов ниже нормально работает с белой темой на Win10
    QBrush background;
    QPen pen;
    if ((option.state & QStyle::State_Selected) ||
        (option.state & QStyle::State_MouseOver)) {
      background = option.palette.highlight();
      pen.setColor(option.palette.highlightedText().color());
    } else {
      background = option.palette.base();
      pen.setColor(option.palette.windowText().color());
    }
    painter->fillRect(option.rect, background);
    painter->setPen(pen);
    // конец кода по установке цветов
    painter->drawText(option.rect, propsStr, opts);
    return;
  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QWidget *PropertiesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(option)
  int data = index.data().toInt();
  PropertiesWidget* bitsWidget = new PropertiesWidget(data, parent);
  return bitsWidget;
}

void PropertiesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  PropertiesWidget* bitsw = qobject_cast<PropertiesWidget*>(editor);
  if (bitsw) {
    int data = bitsw->bitsMask();
    model->setData(index, data);
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void PropertiesDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  PropertiesWidget* bitsw = qobject_cast<PropertiesWidget*>(editor);
  if (bitsw) {
    const QRect& rect = option.rect;
    QPoint tL = bitsw->parentWidget()->mapToGlobal(rect.topLeft());
    bitsw->move(tL.x(), tL.y() + rect.height());
  } else {
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
  }
}
