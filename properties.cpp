#include "properties.h"
#include "ui_properties.h"
#include "Process.hpp"

Ui::properties *g_ui_p;

properties::properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::properties)
{
    g_ui_p = ui;
    ui->setupUi(this);
}


void p_label(QString name, QString pid){
    QString buff = "Properties of process \"";
    buff.append(name);
    buff.append("\" (PID ");
    buff.append(pid);
    buff.append("):");
    g_ui_p->label->setText(buff);

}

void p_details(Process *proc){
    QTableWidget *table = g_ui_p->tableWidget;
    table->setItem(0,0, new QTableWidgetItem(QString::fromStdString(proc->name)));
}

properties::~properties()
{
    delete ui;
}

