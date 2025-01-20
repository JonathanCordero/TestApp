#pragma once

#include "ui_TestApp.h"
#include <QtWidgets/QMainWindow>
#ifndef TESTAPP_H
#define TESTAPP_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qlabel>
#include "WeatherList.h"

#include "BackgroundManager.h"



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
    void showSecondScreen();

private:
    QNetworkAccessManager* manager;
    Ui::TestAppClass ui;
    QLabel* info; // Example label for displaying weather info
    QString parseWeatherJson(const QString& response);

    BackgroundManager* backgroundManager;
    WeatherList* secondScreen;

protected:
    void resizeEvent(QResizeEvent* event) override;


};

#endif // TESTAPP_H