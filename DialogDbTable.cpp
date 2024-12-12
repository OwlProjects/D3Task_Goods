#include "DialogDbTable.h"
#include "ui_DialogDbTable.h"

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

DialogDbTable::DialogDbTable(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbTable),
  m_model(new QSqlTableModel(this))
{
  ui->setupUi(this);
  tableViewData = ui->tableViewData;
}

DialogDbTable::DialogDbTable(QString tableName, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogDbTable),
  m_model(new QSqlTableModel(this)),
  tableName(tableName)
{
  ui->setupUi(this);
  tableViewData = ui->tableViewData;
  setWindowTitle(tableName);
  setupTable();
  ui->splitterHorizontal->setSizes({100, 1});
}

DialogDbTable::~DialogDbTable()
{
  delete ui;
}

bool DialogDbTable::setupTable()
{
  m_model->setObjectName("dataModel");
  tableViewData->setModel(m_model);
  m_model->setTable(tableName);
  return m_model->select();
}

void DialogDbTable::on_pushButtonProcessQuery_clicked()
{
  QSqlQuery query(ui->plainTextEditQuery->toPlainText());
  QString message;
  if (query.exec()) {
    // Примечание
    //  Использование setQuery(std::move(query)) для модели таблицы (QSqlTableModel),
    //  похоже, НЕЖЕЛАТЕЛЬНАЯ ОПЕРАЦИЯ, так как при непосредственном обращении к модели
    //  таблицы метод setQuery(std::move(query)) имеет спецификатор доступа protected
    //  В итоге приходится использовать указатель типа модели запроса (QSqlQueryModel)
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(m_model);
    if (!model) {
      message = QString("[%1] Ошибка: запрос не может быть применен")
                        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
      return;
    }
    model->setQuery(std::move(query));
    tableViewData->update();
    message = QString("[%1] Запрос выполнен")
                      .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
  } else {
    message = QString("[%1] Ошибка выполнения запроса: %2")
                      .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                      .arg(query.lastError().text());
  }
  ui->plainTextEditLog->appendPlainText(message);
}

void DialogDbTable::on_pushButtonReset_clicked()
{
  m_model->setTable(tableName);
  bool selRes = m_model->select();
  QString message = selRes
    ? QString("[%1] Модель сброшена и показывает все содержимое")
      .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
    : QString("[%1] Ошибка сброса модели: %2")
      .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
      .arg(m_model->lastError().text());;
  ui->plainTextEditLog->appendPlainText(message);
}
