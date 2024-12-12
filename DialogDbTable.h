#ifndef DIALOGDBTABLE_H
#define DIALOGDBTABLE_H

#include <QDialog>
#include <QSqlTableModel>
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

private slots:
  void on_pushButtonProcessQuery_clicked();

  void on_pushButtonReset_clicked();

private: // fields
  Ui::DialogDbTable *ui;
  QSqlTableModel* m_model = nullptr;
  QString tableName;
  QTableView* tableViewData = nullptr;

private: // functions
  bool setupTable();


};

#endif // DIALOGDBTABLE_H
