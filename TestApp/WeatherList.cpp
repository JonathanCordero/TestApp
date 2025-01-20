#include "WeatherList.h"

WeatherList::WeatherList(QWidget* parent): QWidget(parent) {
    ui.setupUi(this); // Set up the UI from the .ui file
}

WeatherList::~WeatherList() = default;