#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_action_triggered();

  void on_listWidget_doubleClicked(const QModelIndex &index);

private: // fields
  Ui::MainWindow *ui;
  bool isDbOpened_ = false;

private: // functions
  bool connectDb();
  void getDbTablesList();

};
#endif // MAINWINDOW_H
