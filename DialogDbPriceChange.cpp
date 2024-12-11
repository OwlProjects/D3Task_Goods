#include "DialogDbPriceChange.h"
#include "ui_DialogDbPriceChange.h"
#include "UnixTimeDelegate.h"

#include <QSqlRecord>
#include <QSqlRelationalDelegate>

DialogDbPriceChange::DialogDbPriceChange(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbPriceChange),
  m_model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  setWindowTitle(m_tableName);
  ui->comboBoxViewMode->addItems({"ID", "имена"});
  ui->comboBoxViewMode->setCurrentIndex(1);
  ui->comboBoxDateTimeUsage->addItems({"секунды", "время"});
  ui->comboBoxDateTimeUsage->setCurrentIndex(1);
  setupTable();

  connect(ui->comboBoxViewMode, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
          this, &DialogDbPriceChange::onViewModeIndexChanged);
  connect(ui->comboBoxDateTimeUsage, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
          this, &DialogDbPriceChange::onDateUsageIndexChanged);
}

DialogDbPriceChange::~DialogDbPriceChange()
{
  delete ui;
}

void DialogDbPriceChange::onViewModeIndexChanged(const QString &text)
{
  setRelationsEnabled(text == "имена");
  m_model->select();
}

void DialogDbPriceChange::onDateUsageIndexChanged(const QString &text)
{
  if (text == "время") {
    ui->tableViewData->setItemDelegateForColumn(1,
                                                new UnixTimeDelegate(ui->tableViewData));
  } else {
    ui->tableViewData->setItemDelegateForColumn(1,
                                                new QStyledItemDelegate(ui->tableViewData));
  }
}

bool DialogDbPriceChange::setupTable()
{
  m_model->setObjectName("dataModel");
  bool isNamed = ui->comboBoxViewMode->currentText() == "имена";
  setRelationsEnabled(isNamed);
  QSqlRecord record = m_model->record();
  ui->tableViewData->setModel(m_model);
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("product_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("date_price_change"),
                                              new UnixTimeDelegate(ui->tableViewData));
  return m_model->select();
}

void DialogDbPriceChange::setRelationsEnabled(bool enabled)
{
  m_model->setTable(m_tableName);
  m_model->setHeaderData(0, Qt::Horizontal, "ID", Qt::DisplayRole);
  m_model->setHeaderData(1, Qt::Horizontal, "Дата изменения цены", Qt::DisplayRole);
  m_model->setHeaderData(2, Qt::Horizontal, "Новая цена", Qt::DisplayRole);
  if (enabled) {
    QSqlRecord record = m_model->record();
    m_model->setRelation(record.indexOf("product_id"),
                         QSqlRelation("products", "product_id", "product_name"));
    m_model->setHeaderData(0, Qt::Horizontal, "Название продукта", Qt::DisplayRole);
  } else {
    m_model->setHeaderData(0, Qt::Horizontal, "ID продукта", Qt::DisplayRole);
  }
}

void DialogDbPriceChange::on_pushButtonAddRow_clicked()
{
  QItemSelectionModel *select = ui->tableViewData->selectionModel();
  if (select->hasSelection()) {
    QModelIndexList list = select->selectedRows();
    if (!list.isEmpty()) {
      QModelIndex mi = list[0];
      int row = mi.row();
      m_model->insertRow(row, mi.parent());
      m_model->ite

    }
  }
}
