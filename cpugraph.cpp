#include "cpugraph.hpp"
#include "resource_usage.hpp"
#include <iostream>


CPUGraph::CPUGraph(QWidget *parent) : QWidget(parent){
    init_cpu_monitor();
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CPUGraph::timerPulse);
    this->series = QVector<QLineSeries *>(0);
    this->chart = new QChart();
    this->chart->legend()->setShowToolTips(true);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,32);
    axisX->setVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    axisY->setLabelFormat("%.0f%");
    axisY->setTickCount(3);

    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->chart->addAxis(axisY, Qt::AlignLeft);

    std::vector<float> usage = get_cpu_usage();

    for(int i = 0; i < (int)(usage.size()); ++i) {
        this->series.append(new QLineSeries());
        this->chart->addSeries(this->series.at(i));
        if (i == 0) {
            this->series.at(i)->setName("Total CPU");
        } else {
            this->series.at(i)->setName(QString("Core ").append(i + 48));
        }

        this->series.at(i)->attachAxis(axisX);
        this->series.at(i)->attachAxis(axisY);
    }

    //this->chart->setTitle("widget");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->view = chartView;
}

void CPUGraph::timerPulse() {
    std::vector<float> usage = get_cpu_usage();
    for(int i = 0; i < this->series.size() && i < (int)(usage.size()); ++i) {
        QVector<QPointF> points = this->series.at(i)->pointsVector();
        int len = points.size();
        QVector<QPointF> newPoints(0);
        for (int i = 0; i < len; ++i) {
            if(points.at(i).x() > 0) {
                newPoints.append(QPointF(points.at(i).x() - 1, points.at(i).y()));
            }
        }
        newPoints.append(QPointF(30,usage.at(i) * 100));
        this->series.at(i)->replace(newPoints);
    }
}
