#include "DialogDbPurchases.h"
#include "ui_DialogDbPurchases.h"
#include "UnixTimeDelegate.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QDateTime>
#include <QDataWidgetMapper>

DialogDbPurchases::DialogDbPurchases(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbPurchases),
  m_model(new QSqlRelationalTableModel(this))
{
  ui->setupUi(this);
  setWindowTitle(m_tableName);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
  ui->comboBoxViewMode->addItems({"ID", "имена"});
  ui->comboBoxViewMode->setCurrentIndex(1);
  ui->comboBoxDateTimeUsage->addItems({"секунды", "время"});
  ui->comboBoxDateTimeUsage->setCurrentIndex(1);
  setupTable();
  connect(ui->comboBoxViewMode, &QComboBox::currentTextChanged,
          this, &DialogDbPurchases::onViewModeIndexChanged);
  connect(ui->comboBoxDateTimeUsage, &QComboBox::currentTextChanged,
          this, &DialogDbPurchases::onDateUsageIndexChanged);
}

DialogDbPurchases::~DialogDbPurchases()
{
  delete ui;
}

void DialogDbPurchases::onViewModeIndexChanged(const QString &text)
{
  setRelationsEnabled(text == "имена");
  m_model->select();
}

void DialogDbPurchases::onDateUsageIndexChanged(const QString &text)
{
  int dateTimeCol = m_model->record().indexOf("purchase_date");
  if (text == "время") {
    ui->tableViewData->setItemDelegateForColumn(dateTimeCol,
                                                new UnixTimeDelegate(ui->tableViewData));
  } else {
    ui->tableViewData->setItemDelegateForColumn(dateTimeCol,
                                                new QStyledItemDelegate(ui->tableViewData));
  }
  m_model->select();
}

bool DialogDbPurchases::setupTable()
{
  m_model->setEditStrategy(QSqlTableModel::OnRowChange);
  m_model->setObjectName("dataModel");
  bool isNamed = ui->comboBoxViewMode->currentText() == "имена";
  setRelationsEnabled(isNamed);
  QSqlRecord record = m_model->record();
  ui->tableViewData->setModel(m_model);
  int purchase_date_Col = record.indexOf("purchase_date");
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("customer_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(record.indexOf("store_id"),
                                              new QSqlRelationalDelegate(ui->tableViewData));
  ui->tableViewData->setItemDelegateForColumn(purchase_date_Col,
                                              new UnixTimeDelegate(ui->tableViewData));

  auto selModel = new QItemSelectionModel(m_model, this);
  ui->tableViewData->setSelectionModel(selModel);
  QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
  UnixTimeDelegate* delegate = new UnixTimeDelegate(mapper);
  mapper->setModel(m_model);
  mapper->setItemDelegate(delegate);
  mapper->addMapping(ui->dateTimeEditMapper, purchase_date_Col);
  mapper->setRootIndex(m_model->index(0, purchase_date_Col));

  connect(m_model, &QAbstractItemModel::modelReset, this,
          [this, mapper](){
              if (m_model->rowCount() > 0) {
                  mapper->toFirst();
              }
          });
  connect(selModel, &QItemSelectionModel::currentRowChanged, this,
          [mapper, delegate](const QModelIndex &current, const QModelIndex &previous){
              Q_UNUSED(previous)
              mapper->setCurrentIndex(current.row());
              mapper->setItemDelegate(delegate);
          });

  return m_model->select();
}

void DialogDbPurchases::setRelationsEnabled(bool enabled)
{
  m_model->setTable(m_tableName);
  QSqlRecord record = m_model->record();
  int customer_id_Col = record.indexOf("customer_id");
  int store_id = record.indexOf("store_id");
  m_model->setHeaderData(record.indexOf("purchase_id"), Qt::Horizontal, "ID заказа", Qt::DisplayRole);
  m_model->setHeaderData(record.indexOf("purchase_date"), Qt::Horizontal, "Дата заказа", Qt::DisplayRole);
  if (enabled) {
    // Будут использованы установленные ниже отношения для делегатов
    m_model->setRelation(customer_id_Col,
                         QSqlRelation("customers", "customer_id", "customer_name"));
    m_model->setRelation(store_id,
                         QSqlRelation("stores", "store_id", "store_name"));
    // Изменять заголовок под данные
    m_model->setHeaderData(customer_id_Col, Qt::Horizontal, "Имя покупателя", Qt::DisplayRole);
    m_model->setHeaderData(store_id, Qt::Horizontal, "Название магазина", Qt::DisplayRole);
  } else {
    // Отношения  для делегатов не устанавливаются
    // Изменять заголовок под данные
    m_model->setHeaderData(customer_id_Col, Qt::Horizontal, "ID покупателя", Qt::DisplayRole);
    m_model->setHeaderData(store_id, Qt::Horizontal, "ID магазина", Qt::DisplayRole);
  }
  // Подогнать ширину столбцов под данные
  ui->tableViewData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//  ui->tableViewData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

void DialogDbPurchases::on_pushButtonAppendRow_clicked()
{
  QSqlRecord rec = m_model->record();
  rec.setValue("customer_id", 1);
  rec.setValue("store_id", 1);
  rec.setValue("purchase_date", 1609459200);

  int row = m_model->rowCount();
  if (!m_model->insertRecord(row, rec)) {
    ui->plainTextEditLog->appendPlainText(QString("[%1] ошибка добавления строки %2: %3")
                                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                                                .arg(row + 1)
                                                .arg(m_model->lastError().text()));
    ui->plainTextEditLog->appendPlainText(QString("    record: %1 %2 %3 %4")
                                          .arg(rec.value("purchase_id").toInt())
                                          .arg(rec.value("customer_id").toInt())
                                          .arg(rec.value("store_id").toInt())
                                          .arg(rec.value("purchase_date").toInt()));
  } else {
    ui->plainTextEditLog->appendPlainText(QString("[%2] Добавлена строка %1").arg(row + 1)
                                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
  }
  m_model->select();
}

void DialogDbPurchases::on_pushButtonDeleteRow_clicked()
{
  QItemSelectionModel *select = ui->tableViewData->selectionModel();
  if (select->hasSelection()) {
    QModelIndexList rowList = select->selectedRows();
    if (!rowList.isEmpty()) {

      int row = rowList[0].row();


      if (!m_model->removeRow(row)) {
        ui->plainTextEditLog->appendPlainText(QString("[%3] ошибка удаления строки %1: %2").arg(row + 1)
                                              .arg(m_model->lastError().text())
                                              .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
      } else {
        ui->plainTextEditLog->appendPlainText(QString("[%2] Удалена строка %1").arg(row + 1)
                                              .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
      }

    }
  }
  m_model->select();
}
