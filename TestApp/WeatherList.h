#pragma once
#include <QWidget>
#include "ui_WeatherList.h"

class WeatherList : public QWidget {
    Q_OBJECT
public:
    explicit WeatherList(QWidget* parent = nullptr);
    ~WeatherList();

private:
    Ui::Form ui; 
};