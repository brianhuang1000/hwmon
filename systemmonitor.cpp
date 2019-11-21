#include "systemmonitor.h"
#include "ui_systemmonitor.h"

#include <QBoxLayout>
#include<iostream>
#include <QAction>

#define MAX_STR (1024)

char g_mode = 'a';
std::string g_user = "lhandal";
int g_uid = 12345;
Ui::SystemMonitor *g_ui;
QTreeWidgetItem *g_current_item;
int g_current_col;


QString storage_to_str(double size){
    char buff[1024];
    if (size < 1024){
        sprintf(buff, "%.1lf KiB", size);
    } else if (size < pow(1024, 2)){
        sprintf(buff, "%.1lf MiB", size / 1024);
    } else {
        sprintf(buff, "%.1lf GiB", size / pow(1024, 2));
    }
    std::string str = buff;
    QString qstr = QString::fromStdString(str);
    return qstr;
}

void add_device(device *dev){
    QTreeWidget * tree = g_ui->devices;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString(dev->name));
    item->setText(1, QString::fromStdString(dev->directory));
    item->setText(2, QString::fromStdString(dev->type));
    item->setText(3, storage_to_str(dev->total));
    item->setText(4, storage_to_str(dev->free));
    item->setText(5, storage_to_str(dev->available));
    item->setText(6, storage_to_str(dev->used));
    tree->addTopLevelItem(item);
}


void add_process(QTreeWidgetItem *parent, Process *proc){
    bool add = false;
    if(g_mode == 'a'){
        add = true; //all
    } else if (g_mode == 'o' && g_uid == proc ->uid){
        add = true; //owner
    } else if (g_mode == 'r' && proc->state != 's'){
        add = true; //running
    }
    if (!add){
        return;
    }
    QTreeWidgetItem *process_item = new QTreeWidgetItem();
    process_item->setText(0, QString::fromStdString(proc->name));

    switch (proc->state) {
    case 's': process_item->setText(1, "Sleeping");
        break;
    case 'r': process_item->setText(1, "Running");
        break;
    default: process_item->setText(1, "Waiting");
    }
    process_item->setText(2, QString::number(proc->cpu));
    process_item->setText(3, QString::number(proc->ppid));
    process_item->setText(4, QString::number((proc->vmrss + proc->swap)/1000.00));
    std::list<Process *>::iterator it;
    for(it = proc->children.begin(); it != proc->children.end(); it++){
        add_process(process_item, (*it));
    }
    if (!parent){
        QTreeWidget * tree = g_ui->processes;
        tree->addTopLevelItem(process_item);
    } else {
        parent->addChild(process_item);
    }


}


void populate_processes(){
    int num_processes = 10;

    for (int i = 0; i < num_processes; i++) {
        Process proc;
        switch (i%3) {
        case 1: proc.state = 's';
            break;
        case 2: proc.state = 'r';
            break;
        default: proc.state = 'w';
        }
        proc.pid = i+1;
        proc.ppid = i+1;
        proc.cpu = (unsigned int)pow(13,i) % 100;
        proc.name = "name " + std::to_string(i + 1);
        if ( i%3 ){
            proc.uid = g_uid;
        } else {
            proc.uid = g_uid * i;
        }
        proc.cpu = ((float)1.345 * proc.cpu);
        proc.vmrss = (int)proc.cpu % 100;
        proc.swap = 100 - proc.vmrss;
        if(i % 10 == 7){
            Process child;
            child.state = proc.state;
            child.pid = proc.pid+1;
            child.ppid = proc.ppid+1;
            child.cpu = proc.cpu;
            child.name = proc.name + ".1";
            child.uid = proc.uid;
            child.parent = &proc;
            proc.children.push_back(&child);
        }
        add_process(NULL, &proc);
    }

}

void add_system_info(std::string info){
    g_ui->info->setText(QString::fromStdString(info));
}

SystemMonitor::SystemMonitor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SystemMonitor){
    g_ui = ui;
    ui->setupUi(this);
    ui->menubar->show();
    ui->tabWidget->setTabText(0, "System");
    add_system_info("OS release version\nkernel version\namount of memory\nprocessor version\ndisk storage, etc");
    ui->tabWidget->setTabText(1, "Processes");
    ui->processes->header()->resizeSection(0, 256);
    populate_processes();


    ui->tabWidget->setTabText(2, "Resources");
    ui->tabWidget->setTabText(3, "File System");
    device dev;
    dev.name = "hhhh";
    dev.directory = "/homes//";
    dev.type = "fattt";
    dev.free = 28388;
    dev.available = 500000;
    dev.used = 600000;
    dev.total = dev.free + dev.available + dev.used;
    add_device(&dev);
}

SystemMonitor::~SystemMonitor()
{
    delete ui;
}


void SystemMonitor::on_actionActive_processes_triggered(){
    g_mode = 'r';
    g_ui->processes->clear();
    populate_processes();
}

void SystemMonitor::on_actionAll_processes_triggered(){
    g_mode = 'a';
    g_ui->processes->clear();
    populate_processes();

}

void SystemMonitor::on_actionMy_processes_triggered(){
    g_mode = 'o';
    g_ui->processes->clear();
    populate_processes();
}

void SystemMonitor::stop_process(){
    g_current_item->setText(g_current_col, "stop process");
}
void SystemMonitor::continue_process(){
    g_current_item->setText(g_current_col, "continue process");
}

void SystemMonitor::end_process(){
    g_current_item->setText(g_current_col, "end process");
}

void SystemMonitor::kill_process(){
    g_current_item->setText(g_current_col, "kill process");
}

void SystemMonitor::memory_m(){
    g_current_item->setText(g_current_col, "memory map");
}

void SystemMonitor::open_file(){
    g_current_item->setText(g_current_col, "open file");
}

void SystemMonitor::properties(){
    g_current_item->setText(g_current_col, "properties");
}




void SystemMonitor::on_processes_itemClicked(QTreeWidgetItem *item, int column){
    g_current_col = column;
    g_current_item = item;

    QMenu *menu=new QMenu(tr("Context menu"), this);

    QAction *stop_p = new QAction(tr("Stop Process"), menu);
    QAction *continue_p  = new QAction("Continue Process", menu);
    QAction *end_p =  new QAction("End Process", menu);
    QAction *kill_p  = new QAction("Kill Process", menu);
    QAction *memory_m =  new QAction("Memory Maps", menu);
    QAction *open_f  = new QAction("Open Files", menu);
    QAction *properties =  new QAction("Memory Maps", menu);

    connect(stop_p, SIGNAL(triggered()), this, SLOT(stop_process()));
    connect(continue_p, SIGNAL(triggered()), this, SLOT(continue_process()));
    connect(end_p, SIGNAL(triggered()), this, SLOT(end_process()));
    connect(kill_p, SIGNAL(triggered()), this, SLOT(kill_process()));
    connect(memory_m, SIGNAL(triggered()), this, SLOT(memory_m()));
    connect(open_f, SIGNAL(triggered()), this, SLOT(open_file()));
    connect(properties, SIGNAL(triggered()), this, SLOT(properties()));

    menu->addAction(stop_p);
    menu->addAction(continue_p);
    menu->addSeparator();
    menu->addAction(end_p);
    menu->addAction(kill_p);
    menu->addSeparator();
    menu->addMenu("Change Priority");
    menu->addSeparator();
    menu->addAction(memory_m);
    menu->addAction(open_f);
    menu->addSeparator();
    menu->addAction(properties);

    QRect rect = item->treeWidget()->visualItemRect(item);
    QPoint pos = rect.bottomLeft();
    if(column){
        pos.setX(pos.x() + 246 + 100 * (column - 1));
    }

    pos.setY(pos.y() + 110);
    menu->popup(pos, nullptr);
}
