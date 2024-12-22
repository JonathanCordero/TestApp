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
    void getCoordinates(const QString& city);
    void handleGeocodingReply(QNetworkReply* reply);
    QPair<double, double> parseGeocodingJson(const QByteArray& jsonData);
    void weatherBringer(const QPair<double, double>& coordinates);
    void handleWeatherReply(QNetworkReply* reply);
    QString parseWeatherJson(const QByteArray& jsonData);
    QString getWeatherDescription(int code);
    void updateWeatherLabel(const QString& weatherInfo);

private:
    QNetworkAccessManager* manager;
    Ui::TestAppClass ui;
    QLabel* info; // Example label for displaying weather info
    QString parseWeatherJson(const QString& response);
};

#endif // TESTAPP_H