#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>
#include <QTreeWidgetItem>
#include <cmath>

#include "Process.hpp"

typedef struct process_struct{
    std::string name;
    std::string owner;
    float cpu;
    float memory;
    unsigned int id;
    bool status;
    struct process_struct *children;
    int children_num;
} process;

QT_BEGIN_NAMESPACE
namespace Ui { class SystemMonitor; }
QT_END_NAMESPACE

class SystemMonitor : public QMainWindow
{
    Q_OBJECT

public:
    SystemMonitor(QWidget *parent = nullptr);
    ~SystemMonitor();

    void add_process(QTreeWidgetItem *parent, process *proc);

private slots:
    void on_actionActive_processes_triggered();

    void on_actionAll_processes_triggered();

    void on_actionMy_processes_triggered();

    void on_processes_itemClicked(QTreeWidgetItem *item, int column);

    void stop_process();

    void continue_process();

    void end_process();

    void kill_process();

    void memory_m();

    void open_file();

    void process_prop();

    void on_actionRefresh_triggered();

private:
    Ui::SystemMonitor *ui;
};

typedef struct device_struct{
    std::string name;
    std::string directory;
    std::string type;
    double total;
    double free;
    double available;
    double used;
} device;
#endif // SYSTEMMONITOR_H
