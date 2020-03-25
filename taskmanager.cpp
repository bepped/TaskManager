#include "TaskManager.h"
//#include "ui_taskmanager.h"

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QVector>

#include "TaskEditor.h"

TaskManager::TaskManager(ushort number_of_tasks):
    taskmanager_icon{":/img/task_mgr.png"}
{
    taskVector = QVector<Task>(number_of_tasks);
    execTimer = new QTimer(this);
    execTimer->start(1000);
    createTaskGroupBox();
    createActions();
    createTrayIcon();

    connect(execTimer, &QTimer::timeout, this, &TaskManager::runTasks);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(taskGroupBox);
    setLayout(mainLayout);

    setIcon();
    trayIcon->show();

    setWindowTitle(tr("TaskManager"));
    resize(400, 300);
}

void TaskManager::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("TaskManager"),
                                 tr("The programma rimarrà attivo nell'area notifiche. "
                                    "Per terminare il programma , selezionare <b>Esci</b> "
                                    "dal menu contestuale in corrispondenza dell'icona nell'area notifiche."));
        hide();
        event->ignore();
    }
}

void TaskManager::setIcon()
{
    QIcon icon(taskmanager_icon);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip("Task Manager");
}


void TaskManager::createTaskGroupBox()
{
    taskGroupBox = new QGroupBox(tr("Task Box"));

    showTaskDialogButton = new QPushButton(tr("Nuovo Task"));
    showTaskDialogButton->setDefault(true);

    QGridLayout *taskBoxLayout = new QGridLayout;

    int num_of_tasks = taskVector.size();
    for (int i = 0; i < num_of_tasks; ++i) {
            QPushButton *button = new QPushButton("Configura task: " + QString::number(i));
            connect(button, &QPushButton::clicked, [=] { showTaskEditor(taskVector[i]); });
            taskBoxLayout->addWidget(button, i, 3);
    }

    taskBoxLayout->setColumnStretch(3, 1);
    taskBoxLayout->setRowStretch(4, 1);
    taskGroupBox->setLayout(taskBoxLayout);
}

void TaskManager::createActions()
{

    restoreAction = new QAction(tr("&Ripristina"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Esci"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void TaskManager::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}


void TaskManager::showTaskEditor(Task &task)
{
    taskEditor = new TaskEditor(task);
    taskEditor->setAttribute(Qt::WA_DeleteOnClose);
    taskEditor->exec();
    /*
    if (taskEditor->exec() == QDialog::Accepted) {
        refreshTaskGroupBox();
    }
    */
}

void TaskManager::runTasks()
{
    auto pTask = taskVector.begin();
    auto end = taskVector.end();
    for(; pTask != end; ++pTask) {
        pTask->run_if();
    }
}
