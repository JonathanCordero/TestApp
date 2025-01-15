#include "WeatherList.h"
#include <QVBoxLayout>
#include <QLabel>

WeatherList::WeatherList(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    auto label = new QLabel("Welcome to the second screen!", this);
    layout->addWidget(label);
    setLayout(layout);
}