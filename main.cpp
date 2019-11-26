#include "systemmonitor.h"
#include "Process.hpp"
#include "list_proc.cpp"
#include "networkgraph.hpp"

#include <QApplication>


int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();
    w.setWindowTitle("System Monitor");
//    free_proc_map();

    return a.exec();
}
