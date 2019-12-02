#ifndef USAGE_H
#define USAGE_H

#include <QDialog>

namespace Ui {
class Usage;
}

class Usage : public QDialog
{
    Q_OBJECT

public:
    explicit Usage(QWidget *parent = 0);
    ~Usage();

private:
    Ui::Usage *ui;
};

#endif // USAGE_H
