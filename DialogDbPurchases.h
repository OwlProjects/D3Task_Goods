#ifndef DIALOGDBPURCHASES_H
#define DIALOGDBPURCHASES_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>

namespace Ui {
class DialogDbPurchases;
}

class DialogDbPurchases : public QDialog
{
  Q_OBJECT

public: // c-tors/d-tors
  explicit DialogDbPurchases(QWidget *parent = nullptr);
  ~DialogDbPurchases();

private: // fields
  Ui::DialogDbPurchases *ui;
  QSqlRelationalTableModel* m_model = nullptr;
  const QString m_tableName = "purchases";

private slots:
  void onViewModeIndexChanged(const QString &text);
  void onDateUsageIndexChanged(const QString &text);
  void on_pushButtonAppendRow_clicked();
  void on_pushButtonDeleteRow_clicked();

private: // functions
  bool setupTable();
  void setRelationsEnabled(bool enabled);

};

#endif // DIALOGDBPURCHASES_H
