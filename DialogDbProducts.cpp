#include "DialogDbProducts.h"
#include "ui_DialogDbProducts.h"
#include "PropertiesDelegate.h"

#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>

DialogDbProducts::DialogDbProducts(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbProducts),
  m_model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  setWindowTitle(m_tableName);
  setWindowFlag(Qt::WindowContextHelpButtonHint, false);
  setWindowFlag(Qt::WindowMaximizeButtonHint, true);
  ui->comboBoxViewMode->addItem("ID");
  ui->comboBoxViewMode->addItem("имена");
  ui->comboBoxViewMode->setCurrentIndex(1);
  setupTable();

  connect(ui->comboBoxViewMode, &QComboBox::currentTextChanged,
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
  int product_properties_Col = record.indexOf("product_properties");
  ui->tableViewData->setModel(m_model);
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("manufacturer_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("category_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(product_properties_Col,
                                              new PropertiesDelegate(ui->tableViewData));

  auto selModel = new QItemSelectionModel(m_model, this);
  ui->tableViewData->setSelectionModel(selModel);
  QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
  mapper->setModel(m_model);
  mapper->setItemDelegate(new PropertiesDelegate(ui->tableViewData));
  mapper->addMapping(ui->lineEditMapper, product_properties_Col);
  mapper->addMapping(ui->labelMapper, product_properties_Col, "text");

  connect(m_model, &QAbstractItemModel::modelReset, this,
        [this, mapper](){
            if (m_model->rowCount() > 0) {
                mapper->toFirst();
            }
        });
  connect(selModel, &QItemSelectionModel::currentRowChanged, this,
          [mapper](const QModelIndex &current, const QModelIndex &previous){
                Q_UNUSED(previous)
                mapper->setCurrentIndex(current.row());
          });


  return m_model->select();
}

void DialogDbProducts::setRelationsEnabled(bool enabled)
{
  m_model->setTable(m_tableName);
  QSqlRecord record = m_model->record();
  int manufacturer_id_Col = record.indexOf("manufacturer_id");
  int category_id_Col = record.indexOf("category_id");
  m_model->setHeaderData(record.indexOf("product_id"),
                         Qt::Horizontal, "ID продукта", Qt::DisplayRole);
  m_model->setHeaderData(record.indexOf("product_name"),
                         Qt::Horizontal, "Название\nпродукта", Qt::DisplayRole);
  m_model->setHeaderData(record.indexOf("product_properties"),
                         Qt::Horizontal, "Свойства\nпродукта", Qt::DisplayRole);
  if (enabled) {
    QSqlRecord record = m_model->record();
    m_model->setRelation(manufacturer_id_Col,
                         QSqlRelation("manufacturers", "manufacturer_id", "manufacturer_name"));
    m_model->setRelation(record.indexOf("category_id"),
                         QSqlRelation("categories", "category_id", "category_name"));
    m_model->setHeaderData(manufacturer_id_Col, Qt::Horizontal, "Название\nпроизводителя", Qt::DisplayRole);
    m_model->setHeaderData(category_id_Col, Qt::Horizontal, "Название\nкатегории", Qt::DisplayRole);
  } else {
    m_model->setHeaderData(manufacturer_id_Col, Qt::Horizontal, "ID производителя", Qt::DisplayRole);
    m_model->setHeaderData(category_id_Col, Qt::Horizontal, "ID категории", Qt::DisplayRole);
  }
}

void DialogDbProducts::onViewModeIndexChanged(const QString &text)
{
  setRelationsEnabled(text == "имена");
  m_model->select();
}
