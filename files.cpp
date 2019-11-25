#include "files.h"
#include "ui_files.h"

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
void files_label(QString name, QString pid){
    QString buff = "Files opened by process \"";
    buff.append(name);
    buff.append("\" (PID ");
    buff.append(pid);
    buff.append("):");
    g_ui_files->label->setText(buff);
}

void files_files(){
    QTreeWidget * tree = g_ui_files->files_tree;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString("FD"));
    item->setText(1, "Type");
    item->setText(2, "object");
    tree->addTopLevelItem(item);
}


files::~files()
{
    delete ui;
}
