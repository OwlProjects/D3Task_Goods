#ifndef DIALOGDBPRICECHANGE_H
#define DIALOGDBPRICECHANGE_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>

namespace Ui {
class DialogDbPriceChange;
}

class DialogDbPriceChange : public QDialog
{
  Q_OBJECT

public: // c-tors/d-tors
  explicit DialogDbPriceChange(QWidget *parent = nullptr);
  ~DialogDbPriceChange();

private: // fields
  Ui::DialogDbPriceChange *ui;
  QSqlRelationalTableModel* m_model = nullptr;
  const QString m_tableName = "price_change";

private slots:
  void onViewModeIndexChanged(const QString &text);
  void onDateUsageIndexChanged(const QString &text);

  void on_pushButtonAddRow_clicked();

private: // functions
  bool setupTable();
  void setRelationsEnabled(bool enabled);

};



#endif // DIALOGDBPRICECHANGE_H