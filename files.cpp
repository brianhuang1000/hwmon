#include "files.h"
#include "ui_files.h"
#include "list_proc.hpp"

#include <QBoxLayout>
#include <iostream>
#include <QAction>


Ui::files *g_ui_files;

files::files(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::files)
{
    g_ui_files = ui;
    ui->setupUi(this);
    ui->files_tree->header()->resizeSection(0, 64);
}
void files_label(QString name, QString pid, bool fds){
    QString buff = "No files opened by process \"";
    if (fds){
        buff = "Files opened by process \"";
    }
    buff.append(name);
    buff.append("\" (PID ");
    buff.append(pid);
    buff.append("):");
    g_ui_files->label->setText(buff);
}

bool files_files(pid_t pid){
    QTreeWidget * tree = g_ui_files->files_tree;
    std::list<fds>fd_list = openfiles(pid);
    std::list<fds>::iterator it;
    for (it = fd_list.begin(); it != fd_list.end(); it++) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::number(it->number));
        item->setText(1, QString::fromStdString(it->type));
        item->setText(2, QString::fromStdString(it->loc));
        tree->addTopLevelItem(item);
    }
    return !fd_list.empty();
}


files::~files()
{
    delete ui;
}
