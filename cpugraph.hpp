#ifndef CPUGRAPH_H
#define CPUGRAPH_H

#include <QWidget>
#include <QtCharts>

class CPUGraph : public QWidget
{
    Q_OBJECT
public:
    explicit CPUGraph(QWidget *parent = nullptr);
    QChartView *view;
    QTimer *timer;

private:
    QtCharts::QChart *chart;
    QVector<QLineSeries *> series;

signals:

public slots:
    void timerPulse();
};

#endif // CPUGRAPH_H
