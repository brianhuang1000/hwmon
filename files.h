#ifndef FILES_H
#define FILES_H

#include <QDialog>

namespace Ui {
class files;
}

class files : public QDialog
{
    Q_OBJECT

public:
    explicit files(QWidget *parent = 0);
    ~files();

public:
    Ui::files *ui;
};

#endif // FILES_H
