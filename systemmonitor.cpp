#include "systemmonitor.h"
#include "ui_systemmonitor.h"
#include "files.h"
#include "memorymap.h"
#include "properties.h"
#include "details.hpp"
#include "FileSystem.hpp"
#include "networkgraph.hpp"
#include "resource_usage.hpp"
#include "memorygraph.hpp"
#include "cpugraph.hpp"
#include "usage.h"

#include <QBoxLayout>
#include <iostream>
#include <fstream>
#include <QAction>
#include <QApplication>
#include <sys/utsname.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>

#define MAX_STR (1024)

extern int populate();
extern std::vector<FileSystem> get_fs_list();
extern bool set_parents();
extern void update();
extern void files_label(QString name, QString pid, bool fds);
extern bool files_files(pid_t pid);
extern void mm_label(QString name, QString pid);
extern void mm_files(mem_read mem);
extern std::list<mem_read> mem_map(int pid);
extern void p_label(QString name, QString pid);
extern void p_details(pid_t pid);
extern void free_proc_map();

extern std::list<Process *> g_parents;

char g_mode = 'a';
int g_uid;
Ui::SystemMonitor *g_ui;
QTreeWidgetItem *g_current_item;
int g_current_col;


QString storage_to_str(double size){
  char buff[1024];
  if(size < 1024){
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

void add_device(FileSystem dev){
  if(dev.total_blocks == 0){
    return;
  }
  QTreeWidget * tree = g_ui->devices;
  QTreeWidgetItem *item = new QTreeWidgetItem();
  item->setText(0, QString::fromStdString(dev.name));
  item->setText(1, QString::fromStdString(dev.dir));
  item->setText(2, QString::fromStdString(dev.type));
  item->setText(3, storage_to_str(dev.total_blocks));
  item->setText(4, storage_to_str(dev.available_blocks));
  item->setText(5, storage_to_str(dev.used_blocks));
  item->setText(6, storage_to_str(dev.free_blocks));
  tree->addTopLevelItem(item);
}


void add_process(QTreeWidgetItem *parent, Process *proc){
  if(!proc){
    return;
  }
  bool add = false;
  if(g_mode == 'a'){
    add = true; //all
  } else if (g_mode == 'o' && g_uid == proc ->uid){
    add = true; //owner
  } else if (g_mode == 'r' && proc->state == 'R'){
    add = true; //running
  }
  if (!add){
    if(proc->children.size() > 0){
      std::list<Process *>::iterator it;
      for(it = proc->children.begin(); it != proc->children.end(); it++){
        add_process(NULL, (*it));
      }
    }
    return;
  }
  QTreeWidgetItem *process_item = new QTreeWidgetItem();
  process_item->setText(0, QString::fromStdString(proc->name));


  switch (proc->state) {
    case 'S':
    process_item->setText(1, "Sleeping");
    break;
    case 'R':
    process_item->setText(1, "Running");
    break;
    case 'D':
    process_item->setText(1, "Disk Sleep");
    break;
    case 'T':
    process_item->setText(1, "Stopped");
    break;
    case 'X':
    process_item->setText(1, "Dead");
    break;
    case 'I':
    process_item->setText(1, "Idle");
    break;
    default:
    std::string s(1, proc->state);
    process_item->setText(1, QString::fromStdString(s));
  }
  process_item->setText(2, QString::number(proc->cpu_precent));
  process_item->setText(3, QString::number(proc->pid));
  process_item->setText(4, storage_to_str((proc->vmrss + proc->swap)/1024.0));
  if(proc->children.size() > 0){
    std::list<Process *>::iterator it;
    for(it = proc->children.begin(); it != proc->children.end(); it++){
      add_process(process_item, (*it));
    }
  }
  if (parent == NULL){
    QTreeWidget * tree = g_ui->processes;
    tree->addTopLevelItem(process_item);
  } else {
    parent->addChild(process_item);
  }
}

void populate_devides() {
  std::vector<FileSystem> devices = get_fs_list();
  foreach (auto device, devices) {
    add_device(device);
  }
}

void update_screen(){
  g_ui->processes->clear();
  std::list<Process *>::iterator it;
  Process *proc;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
    proc = (*it);
    add_process(NULL, proc);
  }
}


void populate_processes() {
  populate();
  set_parents();
  update_screen();
}


void add_system_info(){
  QString info;
  char buff[MAX_STR];
  struct utsname OS;
  if(uname(&OS)){
    info.append("Error retrieving OS data\n");
  } else {
    sprintf(buff, "Name: %s\nrelease: %s\nKernel version: %s\n\nTotal RAM:",
                                            OS.sysname, OS.release, OS.version);
    info.append(buff);

  }
  struct sysinfo hardware;
  if(sysinfo(&hardware)){
    info.append("Error retrieving total RAM\n");
  } else {
    info.append(storage_to_str(hardware.totalram  / 1024.0));
  }

  std::ifstream cpuinfo("/proc/cpuinfo");
  std::string line;
  while (!cpuinfo.eof()){
    std::getline(cpuinfo, line);
    strcpy(buff, line.c_str());
    if (!buff || !strlen(buff)){
      continue;
    }
    if (strstr(buff, "model name")){
      info.append("\nProcessor");
      info.append(QString::fromStdString(strchr(buff, ':')));
      info.append("\n");
      break;
    }
  }
  struct statvfs disk;
  if(statvfs("/",&disk) < 0){
    info.append("Error retrieving available storage\n");
  } else {
    info.append("Available space disk: ");
    info.append(storage_to_str(disk.f_bavail * disk.f_bsize / 1024.0));
  }


  g_ui->info->setText(info);
}

void set_up_graphs(){
  CPUGraph *cpu = new CPUGraph();
  cpu->timer->start(1000);
  cpu->view->setFixedSize(g_ui->graphicsView->size());
  cpu->view->setParent(g_ui->graphicsView);

  MemoryGraph *memory = new MemoryGraph();
  memory->timer->start(1000);
  memory->view->setFixedSize(g_ui->graphicsView_2->size());
  memory->view->setParent(g_ui->graphicsView_2);

  NetworkGraph *network = new NetworkGraph();
  network->timer->start(1000);
  network->view->setFixedSize(g_ui->graphicsView_3->size());
  network->view->setParent(g_ui->graphicsView_3);
}

SystemMonitor::SystemMonitor(QWidget *parent)
: QMainWindow(parent), ui(new Ui::SystemMonitor){
  g_ui = ui;
  ui->setupUi(this);
  ui->menubar->show();
  ui->tabWidget->setTabText(0, "System");
  add_system_info();
  ui->tabWidget->setTabText(1, "Processes");
  ui->processes->header()->resizeSection(0, 256);
  g_uid = getuid();
  populate_processes();
  ui->tabWidget->setTabText(2, "Resources");
  ui->tabWidget->setTabText(3, "File System");
  ui->devices->header()->resizeSection(0, 256);
  set_up_graphs();
  populate_devides();
}

SystemMonitor::~SystemMonitor()
{
  delete ui;
}



void SystemMonitor::on_actionActive_processes_triggered(){
  g_mode = 'r';
  update_screen();
}

void SystemMonitor::on_actionAll_processes_triggered(){
  g_mode = 'a';
  update_screen();

}

void SystemMonitor::on_actionMy_processes_triggered(){
  g_mode = 'o';
  update_screen();
}

pid_t get_pid(QTreeWidgetItem *item){
  return (pid_t)(item->text(3).toInt());
}
void process_update(){
  update();
  update_screen();
}

void SystemMonitor::stop_process(){
  kill(get_pid(g_current_item), SIGSTOP);
  process_update();
}
void SystemMonitor::continue_process(){
  kill(get_pid(g_current_item), SIGQUIT);
  process_update();
}

void SystemMonitor::end_process(){
  kill(get_pid(g_current_item), SIGQUIT);
  process_update();
}

void SystemMonitor::kill_process(){
  kill(get_pid(g_current_item), SIGKILL);
  process_update();
}

void SystemMonitor::memory_m(){
  MemoryMap mm;
  mm.setModal(true);
  mm.setWindowTitle("Memory Maps");
  mm_label(g_current_item->text(0), g_current_item->text(3));

  std::list<mem_read> list = mem_map(get_pid(g_current_item));
  std::list<mem_read>::iterator it;
  for(it = list.begin(); it != list.end(); it++){
    mm_files((*it));
  }
  if (list.empty()){
    mm.setWindowTitle("check pid");
  }

  mm.exec();
}

void SystemMonitor::open_file(){
  files f;
  f.setModal(true);
  f.setWindowTitle("Open Files");
  files_label(g_current_item->text(0), g_current_item->text(3),
                                          files_files(get_pid(g_current_item)));
  f.exec();
}

void SystemMonitor::process_prop(){
  properties p;
  p.setModal(true);
  p.setWindowTitle("Properties");
  p_label(g_current_item->text(0), g_current_item->text(3));
  p_details(get_pid(g_current_item));
  p.exec();
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
  QAction *properties =  new QAction("Properties", menu);

  connect(stop_p, SIGNAL(triggered()), this, SLOT(stop_process()));
  connect(continue_p, SIGNAL(triggered()), this, SLOT(continue_process()));
  connect(end_p, SIGNAL(triggered()), this, SLOT(end_process()));
  connect(kill_p, SIGNAL(triggered()), this, SLOT(kill_process()));
  connect(memory_m, SIGNAL(triggered()), this, SLOT(memory_m()));
  connect(open_f, SIGNAL(triggered()), this, SLOT(open_file()));
  connect(properties, SIGNAL(triggered()), this, SLOT(process_prop()));

  menu->addAction(stop_p);
  menu->addAction(continue_p);
  menu->addSeparator();
  menu->addAction(end_p);
  menu->addAction(kill_p);
  menu->addSeparator();
  menu->addAction(memory_m);
  menu->addAction(open_f);
  menu->addSeparator();
  menu->addAction(properties);

  QRect rect = item->treeWidget()->visualItemRect(item);
  QPoint pos = rect.bottomLeft();
  pos = mapToGlobal(pos);

  if(column){
    pos.setX(pos.x() + 246 + 100 * (column - 1));
  }

  pos.setY(pos.y() + 74);
  menu->popup(pos, nullptr);
}

void SystemMonitor::on_actionRefresh_triggered(){
  process_update();
}

void SystemMonitor::on_actionUsage_triggered(){
    Usage u;
    u.setModal(true);
    u.setWindowTitle("Open Files");
    u.exec();
}
