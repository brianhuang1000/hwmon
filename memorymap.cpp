#include "memorymap.h"
#include "details.hpp"
#include "ui_memorymap.h"

Ui::MemoryMap *g_ui_mm;

MemoryMap::MemoryMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemoryMap)
{
    g_ui_mm = ui;
    ui->setupUi(this);
    ui->treeWidget->header()->resizeSection(0, 256);
}

MemoryMap::~MemoryMap()
{
    delete ui;
}

void mm_label(QString name, QString pid){
    QString buff = "Memory map for process \"";
    buff.append(name);
    buff.append("\" (PID ");
    buff.append(pid);
    buff.append("):");
    g_ui_mm->label->setText(buff);
}

void mm_files(mem_read mem){
    QTreeWidget * tree = g_ui_mm->treeWidget;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString(mem.path));
    item->setText(1, QString::fromStdString(mem.start));
    item->setText(2, QString::fromStdString(mem.end));
    item->setText(3, QString::number(mem.size));
    item->setText(4, QString::fromStdString(mem.perms));
    item->setText(5, QString::fromStdString(mem.offset));
    item->setText(6, QString::number(mem.p_clean));
    item->setText(7, QString::number(mem.p_dirty));
    item->setText(8, QString::number(mem.s_clean));
    item->setText(9, QString::number(mem.s_dirty));

    tree->addTopLevelItem(item);
}
