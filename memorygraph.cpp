#include "memorygraph.hpp"
#include "resource_usage.hpp"
#include <iostream>

MemoryGraph::MemoryGraph(QWidget *parent) : QWidget(parent)
{
    std::vector<unsigned long> usage = get_mem_usage();
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MemoryGraph::timerPulse);
    this->chart = new QChart();
    this->chart->legend()->setShowToolTips(true);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,32);
    axisX->setVisible(false);

    QValueAxis *axisYMem = new QValueAxis;
    axisYMem->setRange(0,usage.at(1) / 1048576.0);
    axisYMem->setLabelFormat("%.1f GB");
    axisYMem->setTickCount(5);
//    axisYMem->setTitleText("Memory");

    QValueAxis *axisYSwap = new QValueAxis;
    axisYSwap->setRange(0,usage.at(3) / 1048576.0);
    axisYSwap->setLabelFormat("%.1f GB");
    axisYSwap->setTickCount(5);
    axisYSwap->setTitleText("Swap space");

    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->chart->addAxis(axisYMem, Qt::AlignLeft);
    this->chart->addAxis(axisYSwap, Qt::AlignRight);

    this->mem_series = new QLineSeries();
    this->chart->addSeries(this->mem_series);
    this->mem_series->setName("Used Memory");

    this->mem_series->attachAxis(axisX);
    this->mem_series->attachAxis(axisYMem);

    this->swap_series = new QLineSeries();
    this->chart->addSeries(this->swap_series);
    this->swap_series->setName("Used Swap Space");

    this->swap_series->attachAxis(axisX);
    this->swap_series->attachAxis(axisYSwap);

    //this->chart->setTitle("widget");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->view = chartView;
}

void MemoryGraph::timerPulse() {
    std::vector<unsigned long> usage = get_mem_usage();
    QVector<QPointF> mem_points = this->mem_series->pointsVector();
    QVector<QPointF> swap_points = this->swap_series->pointsVector();
    int len = mem_points.size();
    QVector<QPointF> new_mem_points(0);
    QVector<QPointF> new_swap_points(0);
    for (int i = 0; i < len; ++i) {
        if(mem_points.at(i).x() > 0) {
            new_mem_points.append(QPointF(mem_points.at(i).x() - 1, mem_points.at(i).y()));
            new_swap_points.append(QPointF(swap_points.at(i).x() - 1, swap_points.at(i).y()));
        }
    }
    new_mem_points.append(QPointF(30, (usage.at(1) - usage.at(0)) / 1048576.0));
    new_swap_points.append(QPointF(30, (usage.at(3) - usage.at(2)) / 1048576.0));
    this->mem_series->replace(new_mem_points);
    this->swap_series->replace(new_swap_points);
}
