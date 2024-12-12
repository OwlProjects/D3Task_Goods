#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>

namespace Ui {
class PropertiesWidget;
}

class PropertiesWidget : public QWidget
{
  Q_OBJECT

public: // c-tors/d-tors
  explicit PropertiesWidget(QWidget *parent = nullptr);
  PropertiesWidget(int bitsMask, QWidget *parent = nullptr);
  ~PropertiesWidget();

public: // functions
  void setBitsMask(int mask);
  int bitsMask() const;

private: // fields
  Ui::PropertiesWidget *ui;


private: // functions
  void setupWidget();
};

#endif // PROPERTIESWIDGET_H
