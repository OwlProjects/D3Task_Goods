#include "DialogDbProducts.h"
#include "ui_DialogDbProducts.h"

#include <QSqlRecord>
#include <QSqlRelationalDelegate>

DialogDbProducts::DialogDbProducts(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbProducts),
  m_model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  setWindowTitle(m_tableName);
  ui->comboBoxViewMode->addItem("ID");
  ui->comboBoxViewMode->addItem("имена");
  ui->comboBoxViewMode->setCurrentIndex(1);
  setupTable();

  connect(ui->comboBoxViewMode, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
          this, &DialogDbProducts::onViewModeIndexChanged);
}

DialogDbProducts::~DialogDbProducts()
{
  delete ui;
}

QString DialogDbProducts::tableName() const
{
  return m_tableName;
}

bool DialogDbProducts::setupTable()
{
  m_model->setObjectName("dataModel");
  bool isNamed = ui->comboBoxViewMode->currentText() == "имена";
  setRelationsEnabled(isNamed);
  QSqlRecord record = m_model->record();
  ui->tableViewData->setModel(m_model);
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("manufacturer_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("category_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  return m_model->select();
}

void DialogDbProducts::setRelationsEnabled(bool enabled)
{
  m_model->setTable(m_tableName);
  m_model->setHeaderData(0, Qt::Horizontal, "ID продукта", Qt::DisplayRole);
  m_model->setHeaderData(1, Qt::Horizontal, "Название\nпродукта", Qt::DisplayRole);
  if (enabled) {
    QSqlRecord record = m_model->record();
    m_model->setRelation(record.indexOf("manufacturer_id"),
                         QSqlRelation("manufacturers", "manufacturer_id", "manufacturer_name"));
    m_model->setRelation(record.indexOf("category_id"),
                         QSqlRelation("categories", "category_id", "category_name"));
    m_model->setHeaderData(2, Qt::Horizontal, "Название\nпроизводителя", Qt::DisplayRole);
    m_model->setHeaderData(3, Qt::Horizontal, "Название\nкатегории", Qt::DisplayRole);
  } else {
    m_model->setHeaderData(2, Qt::Horizontal, "ID производителя", Qt::DisplayRole);
    m_model->setHeaderData(3, Qt::Horizontal, "ID категории", Qt::DisplayRole);
  }
}

void DialogDbProducts::onViewModeIndexChanged(const QString &text)
{
  setRelationsEnabled(text == "имена");
  m_model->select();
}
