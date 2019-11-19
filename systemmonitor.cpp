#include "systemmonitor.h"
#include "ui_systemmonitor.h"

#include <QBoxLayout>
#include<iostream>

#define MAX_STR (1024)

char g_mode = 'a';
std::string g_user = "lhandal";
Ui::SystemMonitor *g_ui;


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

void add_process(QTreeWidgetItem *parent, process *proc){
    bool add = false;
    if(g_mode == 'a'){
        add = true; //all
    } else if (g_mode == 'o' && !g_user.compare(proc->owner)){
        add = true; //owner
    } else if (g_mode == 'r'){
        add = proc->status; //running
    }
    if (!add){
        return;
    }
    QTreeWidgetItem *process_item = new QTreeWidgetItem();
    process_item->setText(0, QString::fromStdString(proc->name));
    if (proc->status){
        process_item->setText(1, "Running ");
    } else {
        process_item->setText(1, "Sleeping ");
    }
    process_item->setText(2, QString::number((double)proc->cpu));
    process_item->setText(3, QString::number(proc->id));
    process_item->setText(4, QString::number((double)proc->memory));
    for (int i = 0; i < proc->children_num; i++){
        add_process(process_item, &proc->children[i]);
    }
    if (!parent){
        QTreeWidget * tree = g_ui->processes;
        tree->addTopLevelItem(process_item);
    } else {
        parent->addChild(process_item);
    }


}

void populate_processes(){
    process proc;
    process child;
    unsigned int num_processes = 10;

    for (unsigned int i = 0; i < num_processes; i++) {
        proc.status = i%2 == 0;
        proc.id = i+1;
        proc.cpu = (unsigned int)pow(13,i) % 100;
        proc.name = "name " + std::to_string(i);
        if ( i%3 ){
            proc.owner = "lhandal";
        } else {
            proc.owner = "owner";
        }
        proc.memory = (float)((float)1.345 * proc.cpu);
        proc.children_num = 0;
        if(i == 7){
            process child;
            proc.children_num = 1;
            proc.children = &child;
            child.status = i%2 == 0;
            child.id = i+1;
            child.cpu = (unsigned int)pow(13,i) % 100;
            child.name = proc.name + ".1";
            if ( i%3 ){
                child.owner = "lhandal";
            } else {
                child.owner = "owner";
            }
            child.memory = (float)((float)1.345 * proc.cpu);
            child.children_num = 0;
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


void SystemMonitor::on_processes_itemClicked(QTreeWidgetItem *item, int column){
    item->setText(0, "killed");
//    QAction *pAddAction = new QAction("Kill",this);

//    QMenu *pContextMenu = new QMenu( this);
//    pContextMenu->addAction(pAddAction);
//    delete pContextMenu;
//    pContextMenu = NULL;
}
