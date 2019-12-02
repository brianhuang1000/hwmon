#include "properties.h"
#include "details.hpp"
#include "ui_properties.h"
#include "Process.hpp"
#include <iostream>

Ui::properties *g_ui_p;
extern proc_prop details(int);
extern QString storage_to_str(double size);

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

void p_details(pid_t pid){
    QTableWidget *table = g_ui_p->tableWidget;
    proc_prop proc = details(pid);
    table->setItem(0,0, new QTableWidgetItem(QString::fromStdString(proc.name)));
    table->setItem(1,0, new QTableWidgetItem(QString::fromStdString(proc.user)));
    table->setItem(2,0, new QTableWidgetItem(QString::fromStdString(proc.status)));
    table->setItem(3,0, new QTableWidgetItem(storage_to_str(proc.memory)));
    table->setItem(4,0, new QTableWidgetItem(storage_to_str(proc.v_mem)));
    table->setItem(5,0, new QTableWidgetItem(storage_to_str(proc.res_mem)));
    table->setItem(6,0, new QTableWidgetItem(storage_to_str(proc.sh_mem)));
    table->setItem(7,0, new QTableWidgetItem(QString::number(proc.cpu)));
    table->setItem(8,0, new QTableWidgetItem(QString::fromStdString(proc.uptime)));
    table->setItem(9,0, new QTableWidgetItem(QString::fromStdString(proc.started)));
    table->setItem(10,0, new QTableWidgetItem(QString::number(proc.id)));
}

properties::~properties()
{
    delete ui;
}

