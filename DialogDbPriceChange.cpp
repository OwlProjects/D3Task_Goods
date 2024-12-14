#include "DialogDbPriceChange.h"
#include "ui_DialogDbPriceChange.h"
#include "UnixTimeDelegate.h"

#include <QStringBuilder>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QSqlError>

DialogDbPriceChange::DialogDbPriceChange(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbPriceChange),
  m_model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  setWindowTitle(m_tableName);
  setWindowFlag(Qt::WindowContextHelpButtonHint, false);
  ui->comboBoxViewMode->addItems({"ID", "имена"});
  ui->comboBoxViewMode->setCurrentIndex(1);
  ui->comboBoxDateTimeUsage->addItems({"секунды", "время"});
  ui->comboBoxDateTimeUsage->setCurrentIndex(1);
  setupTable();

  connect(ui->comboBoxViewMode, &QComboBox::currentTextChanged,
          this, &DialogDbPriceChange::onViewModeIndexChanged);
  connect(ui->comboBoxDateTimeUsage, &QComboBox::currentTextChanged,
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
  int date_price_change_Col = m_model->record().indexOf("date_price_change");
  if (text == "время") {
    ui->tableViewData->setItemDelegateForColumn(date_price_change_Col,
                                                m_timeDelegates["date_price_change"]);
  } else {
    ui->tableViewData->setItemDelegateForColumn(date_price_change_Col,
                                                m_timeDelegates["date_price_change_unix_Seconds"]);
  }
  m_model->select();
}

bool DialogDbPriceChange::setupTable()
{
  m_model->setEditStrategy(QSqlTableModel::OnRowChange);
  m_model->setObjectName("dataModel");
  ui->tableViewData->setModel(m_model);
  bool isNamed = ui->comboBoxViewMode->currentText() == "имена";
  setRelationsEnabled(isNamed);
  QSqlRecord record = m_model->record();
  m_timeDelegates["date_price_change"] = new UnixTimeDelegate(ui->tableViewData);
  m_timeDelegates["date_price_change_unix_Seconds"] = new QStyledItemDelegate(ui->tableViewData);
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("product_id"),
                                               new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("date_price_change"),
                                              m_timeDelegates["date_price_change"]);
  return m_model->select();
}

void DialogDbPriceChange::setRelationsEnabled(bool enabled)
{
  m_model->setTable(m_tableName);
  QSqlRecord record = m_model->record();
  int product_id_Col = record.indexOf("product_id");
  m_model->setHeaderData(record.indexOf("product_id"),
                         Qt::Horizontal, "ID", Qt::DisplayRole);
  m_model->setHeaderData(record.indexOf("date_price_change"),
                         Qt::Horizontal, "Дата изменения цены", Qt::DisplayRole);
  m_model->setHeaderData(record.indexOf("new_price"),
                         Qt::Horizontal, "Новая цена", Qt::DisplayRole);
  if (enabled) {
    m_model->setRelation(product_id_Col,
                         QSqlRelation("products", "product_id", "product_name"));
    m_model->setHeaderData(product_id_Col, Qt::Horizontal, "Название продукта", Qt::DisplayRole);
  } else {
    m_model->setHeaderData(product_id_Col, Qt::Horizontal, "ID продукта", Qt::DisplayRole);
  }
}

