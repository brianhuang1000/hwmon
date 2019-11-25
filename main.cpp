#include "systemmonitor.h"
#include "Process.hpp"
#include "list_proc.cpp"

#include <QApplication>


int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();
    w.setWindowTitle("System Monitor");
//    set_parents();
//    get_cpu(get_nprocs_conf());
//    update();
//    free_proc_map();



    return a.exec();
}
