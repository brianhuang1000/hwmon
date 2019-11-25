#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <QWidget>
#include <QtCharts>

class NetworkGraph : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkGraph(QWidget *parent = nullptr);
    QChartView *view;
    QTimer *timer;

private:
    QtCharts::QChart *chart;
    QLineSeries *send_series;
    QLineSeries *recv_series;
    unsigned int y_max;

signals:

public slots:
    void timerPulse();
};

#endif // NETWORKGRAPH_H
