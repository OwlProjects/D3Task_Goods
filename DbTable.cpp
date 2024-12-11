#include "DbTable.h"

DbTable::DbTable()
{

}

DbTable::DbTable(QString tableName, QTableView *tableViewData):
  m_tableName(tableName)
{
  m_tableViewData = tableViewData;
  setupTable();
}

bool DbTable::setupTable()
{
  if (!m_tableViewData) {
    return false;
  }
  m_model = new QSqlRelationalTableModel();
  m_model->setObjectName("dataModel");
  m_tableViewData->setModel(m_model);
  m_model->setTable(m_tableName);
  return m_model->select();
}

void DbTable::setTableName(QString tableName)
{
  m_tableName = tableName;
}

void DbTable::setTableView(QTableView *tableViewData)
{
  m_tableViewData = tableViewData;
}
