#include "PropertiesWidget.h"
#include "ui_PropertiesWidget.h"

PropertiesWidget::PropertiesWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PropertiesWidget)
{
  ui->setupUi(this);
  setupWidget();
}

PropertiesWidget::PropertiesWidget(int bitsMask, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PropertiesWidget)
{
  ui->setupUi(this);
  setupWidget();
  setBitsMask(bitsMask);
}

PropertiesWidget::~PropertiesWidget()
{
  delete ui;
}

void PropertiesWidget::setBitsMask(int mask)
{
  ui->checkBoxBit0->setChecked(mask & (1 << 0));
  ui->checkBoxBit1->setChecked(mask & (1 << 1));
  ui->checkBoxBit2->setChecked(mask & (1 << 2));
  ui->checkBoxBit3->setChecked(mask & (1 << 3));
  ui->checkBoxBit4->setChecked(mask & (1 << 4));
  ui->checkBoxBit5->setChecked(mask & (1 << 5));
  ui->checkBoxBit6->setChecked(mask & (1 << 6));
  ui->checkBoxBit7->setChecked(mask & (1 << 7));
}

int PropertiesWidget::bitsMask() const
{
  int mask = 0;
  mask |= ui->checkBoxBit0->isChecked() << 0;
  mask |= ui->checkBoxBit1->isChecked() << 1;
  mask |= ui->checkBoxBit2->isChecked() << 2;
  mask |= ui->checkBoxBit3->isChecked() << 3;
  mask |= ui->checkBoxBit4->isChecked() << 4;
  mask |= ui->checkBoxBit5->isChecked() << 5;
  mask |= ui->checkBoxBit6->isChecked() << 6;
  mask |= ui->checkBoxBit7->isChecked() << 7;
  return mask;
}

void PropertiesWidget::setupWidget()
{
  setWindowFlags(Qt::Popup);
}
