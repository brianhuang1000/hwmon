#ifndef MEMORYGRAPH_H
#define MEMORYGRAPH_H

#include <QWidget>
#include <QtCharts>

class MemoryGraph : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryGraph(QWidget *parent = nullptr);
    QChartView *view;
    QTimer *timer;

private:
    QtCharts::QChart *chart;
    QLineSeries *mem_series;
    QLineSeries *swap_series;

signals:

public slots:
    void timerPulse();
};

#endif // MEMORYGRAPH_H
