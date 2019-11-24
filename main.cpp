#include "systemmonitor.h"
#include "Process.hpp"
#include "list_proc.cpp"

#include <QApplication>


int main(int argc, char *argv[]){
    populate();
    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();
    w.setWindowTitle("System Monitor");
//    print_list();
//    free_proc_map();



    return a.exec();
}
