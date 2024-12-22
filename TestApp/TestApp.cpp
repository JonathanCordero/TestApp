#include "TestApp.h"
#include <QDebug>
#include <Qlabel>
#include <QNetworkAccessManager>
#include <QtWidgets/QApplication>
#include <QNetworkRequest>
#include <qpushbutton>
#include <QUrl>
#include <qthread.h>
#include <qjsonarray>
#include <qjsondocument.h>
#include <qjsonobject.h>

TestApp::TestApp(QWidget* parent) : QMainWindow(parent), manager(new QNetworkAccessManager(this)) {

    ui.setupUi(this);
    ui.lineEdit->setPlaceholderText("Enter city name");
    ui.Display->setText("Display Weather");
    info = ui.info;

    connect(ui.Display, &QPushButton::clicked, this, [this]() {
        qDebug() << "Button clicked!";

        QString city = ui.lineEdit->text();
        if (city.isEmpty()) {
            info->setText("Please enter a city name.");
            return;
        }
        getCoordinates(city);
        ui.lineEdit->setPlaceholderText("Enter another city name");
        });
}

void TestApp::getCoordinates(const QString& city)
{
    // Construct the Nominatim API URL
    QString url = QString("https://nominatim.openstreetmap.org/search?q=%1&format=json").arg(city);
    QUrl apiUrl(url);

    // Validate URL
    if (!apiUrl.isValid()) {
        qDebug() << "Invalid URL:" << apiUrl.toString();
        return;
    }
    
    QNetworkRequest request(apiUrl);

    // Connect the reply signal to the slot that handles the response
    disconnect(manager, &QNetworkAccessManager::finished, nullptr, nullptr);
    connect(manager, &QNetworkAccessManager::finished, this, &TestApp::handleGeocodingReply);

    manager->get(request); // Send the request
}

void TestApp::handleGeocodingReply(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Geocoding error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();

    qDebug() << "Geocoding API Response:" << response; // Log the raw response

    QPair<double, double> coordinates = parseGeocodingJson(response);
    if (coordinates.first != 0.0 || coordinates.second != 0.0) {
        weatherBringer(coordinates);
    }
    else {
        info->setText("Could not fetch coordinates for the city.");
    }

    reply->deleteLater();
}

QPair<double, double> TestApp::parseGeocodingJson(const QByteArray& jsonData) {
   
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Geocoding response is not a JSON array.";
        return { 0.0, 0.0 };
    }

    QJsonArray array = doc.array();
    if (array.isEmpty()) {
        qDebug() << "Geocoding response is empty.";
        return { 0.0, 0.0 };
    }

    QJsonObject firstResult = array.first().toObject();
    QString latStr = firstResult.value("lat").toString();
    QString lonStr = firstResult.value("lon").toString();

    if (latStr.isEmpty() || lonStr.isEmpty()) {
        qDebug() << "Missing latitude or longitude in geocoding response.";
        return { 0.0, 0.0 };
    }

    double latitude = latStr.toDouble();
    double longitude = lonStr.toDouble();

    qDebug() << "Parsed coordinates: Latitude:" << latitude << ", Longitude:" << longitude;
    return { latitude, longitude };
}

void TestApp::weatherBringer(const QPair<double, double>& coordinates) {
    QString url = QString("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current_weather=true")
        .arg(coordinates.first)
        .arg(coordinates.second);
    QUrl apiUrl(url);
    qDebug() << "Weather API URL:" << url;

    if (!apiUrl.isValid()) {
        qDebug() << "Invalid weather API URL.";
        return;
    }

    QNetworkRequest request(apiUrl);
    disconnect(manager, &QNetworkAccessManager::finished, nullptr, nullptr);
    connect(manager, &QNetworkAccessManager::finished, this, &TestApp::handleWeatherReply);

    manager->get(request);
}

void TestApp::handleWeatherReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Weather API error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    if (response.isEmpty()) {
        qDebug() << "Weather API response is empty.";
        reply->deleteLater();
        return;
    }

    qDebug() << "Weather API Response:" << response;

    QString weatherInfo = parseWeatherJson(response);
    updateWeatherLabel(weatherInfo);

    reply->deleteLater();
}

QString TestApp::parseWeatherJson(const QByteArray& jsonData) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qDebug() << "Invalid weather JSON.";
        return "Error: Could not parse weather data.";
    }

    QJsonObject root = doc.object();
    if (!root.contains("current_weather")) {
        qDebug() << "Missing 'current_weather' in weather JSON.";
        return "Error: Weather data is incomplete.";
    }
    QJsonObject currentWeather = root["current_weather"].toObject();

    int weatherCode = currentWeather["weathercode"].toInt();
    QString weatherDescription = getWeatherDescription(weatherCode);

    double temperature = currentWeather["temperature"].toDouble();
    double windSpeed = currentWeather["windspeed"].toDouble();
    QString weather = currentWeather["weathercode"].toString();

    return QString(
        "Temperature: %1°C\n"
        "Wind Speed: %2 km/h\n"
        "Weather: %3"
    ).arg(temperature).arg(windSpeed).arg(weather);
}

void TestApp::updateWeatherLabel(const QString& weatherInfo) {
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, [this, weatherInfo]() {
            info->setText(weatherInfo);
            }, Qt::QueuedConnection);
    }
    else {
        info->setText(weatherInfo);
    }
}

QString TestApp::getWeatherDescription(int code) {
    switch (code) {
    case 0: return "Clear Sky";
    case 1: return "Partly Cloudy";
    case 2: return "Cloudy";
        // Add more cases based on the API documentation
    default: return "Unknown Weather Condition";
    }
}

TestApp::~TestApp() {
    qDebug() << "Destroying WeatherApp";
    if (manager) {
        manager->deleteLater();
        manager = nullptr;
    }
}