#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <QDialog>

#include "details.hpp"

namespace Ui {
class MemoryMap;
}

class MemoryMap : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryMap(QWidget *parent = 0);
    ~MemoryMap();

    void mm_label(QString name, QString pid);

    void mm_files(mem_read mem);

private:
    Ui::MemoryMap *ui;
};

#endif // MEMORYMAP_H
