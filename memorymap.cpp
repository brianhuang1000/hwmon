#include "memorymap.h"
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

void mm_files(){
    QTreeWidget * tree = g_ui_mm->treeWidget;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString("Filename"));
    item->setText(1, "VM Start");
    item->setText(2, "VM End");
    item->setText(3, "VM size");
    item->setText(4, "Flags");
    item->setText(5, "VM Offset");
    item->setText(6, "Private Clean");
    item->setText(7, "Private Dirty");
    item->setText(8, "Shared Clean");
    item->setText(9, "Shared Dirty");

    tree->addTopLevelItem(item);
}
