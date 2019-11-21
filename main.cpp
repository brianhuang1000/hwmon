#include "systemmonitor.h"
#include "Process.hpp"
#include "list_proc.cpp"

#include <QApplication>


int main(int argc, char *argv[]){
    populate();
    print_tree();
    free_proc_map();

    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();
    w.setWindowTitle("System Monitor");

    return a.exec();
}
