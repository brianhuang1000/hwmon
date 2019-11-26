#include "networkgraph.hpp"
#include "resource_usage.hpp"
#include <iostream>
#define INITIAL_Y_MAX 1 //kB/s

NetworkGraph::NetworkGraph(QWidget *parent) : QWidget(parent){
    init_network_monitor();
    this->y_max = INITIAL_Y_MAX;
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkGraph::timerPulse);
    this->chart = new QChart();
    this->chart->legend()->setShowToolTips(true);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,32);
    axisX->setVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, this->y_max); // kB/s
    axisY->setLabelFormat("%.1f kB/s");
    axisY->setTickCount(3);
    axisY->setTitleText("Network activity");

    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->chart->addAxis(axisY, Qt::AlignLeft);

    this->send_series = new QLineSeries();
    this->chart->addSeries(this->send_series);
    this->send_series->setName("Outbound");

    this->send_series->attachAxis(axisX);
    this->send_series->attachAxis(axisY);

    this->recv_series = new QLineSeries();
    this->chart->addSeries(this->recv_series);
    this->recv_series->setName("Inbound");

    this->recv_series->attachAxis(axisX);
    this->recv_series->attachAxis(axisY);

    //this->chart->setTitle("widget");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->view = chartView;
}

void NetworkGraph::timerPulse() {
    std::vector<double> usage = get_network_usage();
    QVector<QPointF> send_points = this->send_series->pointsVector();
    QVector<QPointF> recv_points = this->recv_series->pointsVector();
    int len = send_points.size();
    QVector<QPointF> new_send_points(0);
    QVector<QPointF> new_recv_points(0);
    for (int i = 0; i < len; ++i) {
        if(send_points.at(i).x() > 0) {
            new_send_points.append(QPointF(send_points.at(i).x() - 1, send_points.at(i).y()));
            new_recv_points.append(QPointF(recv_points.at(i).x() - 1, recv_points.at(i).y()));
        }
    }
    double new_send = (usage.at(0) / usage.at(2)) / 1024.0;
    double new_recv = (usage.at(1) / usage.at(2)) / 1024.0;
    if(new_send > this->y_max || new_recv > this->y_max) {
        if(new_send > this->y_max) {
            this->y_max = static_cast<unsigned int>(new_send);
        }
        if(new_recv > this->y_max) {
            this->y_max = static_cast<unsigned int>(new_recv);
        }
        this->chart->axisY()->setRange(0,this->y_max);
    }
    new_send_points.append(QPointF(30, new_send));
    new_recv_points.append(QPointF(30, new_recv));
    this->send_series->replace(new_send_points);
    this->recv_series->replace(new_recv_points);
}
