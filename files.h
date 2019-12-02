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

    void files_label(QString name, QString pid, bool fds);

    bool files_files(pid_t pid);

public:
    Ui::files *ui;
};

#endif // FILES_H
