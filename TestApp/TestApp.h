#pragma once

#include "ui_TestApp.h"
#include <QtWidgets/QMainWindow>
#include <ui_TestApp.h>
#ifndef TESTAPP_H
#define TESTAPP_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qlabel>

class TestApp : public QMainWindow {
    Q_OBJECT

public:
     TestApp(QWidget* parent = nullptr);
     ~TestApp();

private slots:
    void weatherBringer(QNetworkReply* reply);
    void updateWeatherLabel(const QString& weatherInfo);
    void getCoordinates(const QString& city);

private:
    QNetworkAccessManager* manager;
    Ui::TestAppClass ui;
    QLabel* info; // Example label for displaying weather info
    QString parseWeatherJson(const QString& response);
};

#endif // TESTAPP_H