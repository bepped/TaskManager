#include "TaskManager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   Q_INIT_RESOURCE(taskmanager);

   QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

   QApplication app(argc, argv);

   QApplication::setQuitOnLastWindowClosed(false);

   TaskManager tskmgr(5);
   tskmgr.show();
   return app.exec();
}
