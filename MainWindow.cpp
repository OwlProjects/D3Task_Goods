//===
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogDbTable.h"
#include "DialogDbProducts.h"
#include "DialogDbPriceChange.h"

//===
#include <QFileDialog>
//---
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  isDbOpened_ = connectDb();
  if (!isDbOpened_) {
    return;
  }

  getDbTablesList();
}

MainWindow::~MainWindow()
{
  delete ui;
}

bool MainWindow::connectDb()
{
  QString dbfilepath = "goods.db";
  QFile file(dbfilepath);
  if (!file.exists()) {
    ui->statusbar->showMessage("Укажите файл");
    dbfilepath = QFileDialog::getOpenFileName(this, "Укажите путь к БД", "goods.db", "DB (*db)");
  }
  if (dbfilepath.isEmpty()) {
    ui->statusbar->showMessage(tr("БД не указана "));
    return false;
  }
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbfilepath);
  if (!db.open()) {
    ui->statusbar->showMessage(tr("БД не открыта ") + db.lastError().text());
    return false;
  }
  return true;
//  ui->statusbar->showMessage(tr("Статус открытия ") + QString::number(db.isOpen()) + db.lastError().text());
}

void MainWindow::getDbTablesList()
{
  QSqlQuery query;
  if (!query.exec("SELECT * FROM sqlite_master")) {
    ui->statusbar->showMessage(tr("Ошибка чтения списка таблиц ") + query.lastError().text());
    return;
  }
  ui->listWidget->clear();
  while (query.next())
  {
    if (query.value("type").toString() == "table") {
      ui->listWidget->addItem(query.value("name").toString());
    }
  }
}


void MainWindow::on_action_triggered()
{
  isDbOpened_ = connectDb();
  if (!isDbOpened_) {
    return;
  }
  getDbTablesList();
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
  QDialog* dialog;
  QString tableName = index.data().toString();
  if (tableName == "products") {
    dialog = new DialogDbProducts(this);
  } else if (tableName == "price_change") {
    dialog = new DialogDbPriceChange(this);
  } else {
    dialog = new DialogDbTable(tableName, this);
  }
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->setModal(true);
  dialog->show();
}

