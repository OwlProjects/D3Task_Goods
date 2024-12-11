#ifndef DBTABLE_H
#define DBTABLE_H

#include <QSqlRelationalTableModel>
#include <QTableView>

class DbTable
{
public:
  DbTable();
  DbTable(QString tableName, QTableView* tableViewData = nullptr);

protected:
  virtual bool setupTable();

private:
  QSqlRelationalTableModel* m_model = nullptr;
  QString m_tableName;
  QTableView* m_tableViewData = nullptr;

private:
  void setTableName(QString tableName);
  void setTableView(QTableView* tableView);
};

#endif // DBTABLE_H
