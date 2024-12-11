#ifndef DIALOGDBTABLE_H
#define DIALOGDBTABLE_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>

namespace Ui {
class DialogDbTable;
}

class DialogDbTable : public QDialog
{
  Q_OBJECT

public: // c-tors/d-tors
  explicit DialogDbTable(QWidget *parent = nullptr);
  DialogDbTable(QString tableName, QWidget *parent = nullptr);
  ~DialogDbTable();

private: // fields
  Ui::DialogDbTable *ui;
  QSqlRelationalTableModel* model = nullptr;
  QString tableName;
  QTableView* tableViewData = nullptr;

private: // functions
  bool setupTable();


};

#endif // DIALOGDBTABLE_H
