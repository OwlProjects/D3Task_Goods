#ifndef DIALOGDBPRODUCTS_H
#define DIALOGDBPRODUCTS_H

#include <QStyledItemDelegate>
#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>

namespace Ui {
class DialogDbProducts;
}

class DialogDbProducts : public QDialog
{
  Q_OBJECT

public: // c-tors/d-tors
  explicit DialogDbProducts(QWidget *parent = nullptr);
  ~DialogDbProducts();

public: // functions
  QString tableName() const;

private: // fields
  Ui::DialogDbProducts *ui;
  QSqlRelationalTableModel* m_model = nullptr;
  const QString m_tableName = "products";

private slots:
  void onViewModeIndexChanged(const QString &text);

private: // functions
  bool setupTable();
  void setRelationsEnabled(bool enabled);

};

#endif // DIALOGDBPRODUCTS_H
