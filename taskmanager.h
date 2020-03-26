#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>

#include <QDialog>
#include <QVector>
#include <QBitArray>

QT_BEGIN_NAMESPACE
class QAction;
class QGroupBox;
class QLabel;
class QMenu;
class QPushButton;
class QSystemTrayIcon;
class QTimer;
QT_END_NAMESPACE

#include "Task.h"


class TaskEditor;

class TaskManager : public QDialog
{
    Q_OBJECT

    QString taskmanager_icon;
    int number_of_tasks;
    QTimer *execTimer;
    QVector<Task> taskVector;

    QGroupBox *taskGroupBox;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QPushButton *showTaskDialogButton;
    TaskEditor *taskEditor;

    void createTaskGroupBox();
    void createActions();
    void createTrayIcon();

private slots:
    void setIcon();
    void showTaskEditor(Task &task);
    void runTasks();

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    TaskManager();

};


#endif // TASKMANAGER_H
