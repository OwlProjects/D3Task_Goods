#include "DialogDbTable.h"
#include "ui_DialogDbTable.h"

DialogDbTable::DialogDbTable(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbTable),
  model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  tableViewData = ui->tableViewData;
}

DialogDbTable::DialogDbTable(QString tableName, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbTable),
  model(new QSqlRelationalTableModel(this)),
  tableName(tableName)
{
  ui->setupUi(this);
  tableViewData = ui->tableViewData;
  setWindowTitle(tableName);
  setupTable();
}

DialogDbTable::~DialogDbTable()
{
  delete ui;
}

bool DialogDbTable::setupTable()
{
  if (!tableViewData) {
    return false;
  }
  model->setObjectName("dataModel");
  tableViewData->setModel(model);
  model->setTable(tableName);
  return model->select();
}
