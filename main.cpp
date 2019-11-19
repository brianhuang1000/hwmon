#include "systemmonitor.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();
    w.setWindowTitle("System Monitor");

    return a.exec();
}
