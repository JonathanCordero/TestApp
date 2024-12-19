#include "TestApp.h"

#include "TestApp.h"
#include <QDebug>
#include <Qlabel>
#include <QNetworkAccessManager>
#include <QtWidgets/QApplication>
#include <QNetworkRequest>
#include <qpushbutton>
#include <QUrl>
#include <qthread.h>
#include<qjsonarray>
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
    disconnect(manager, &QNetworkAccessManager::finished, this, &TestApp::weatherBringer);
    connect(manager, &QNetworkAccessManager::finished, this, &TestApp::weatherBringer);

    manager->get(request); // Send the request
}

void TestApp::weatherBringer(QNetworkReply* reply)
{
    // Handle any errors
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error in geocoding request:" << reply->errorString();
        return;
    }

   // Parse the JSON response
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (!doc.isArray()) {
        qDebug() << "Invalid JSON or no array found!";
        return;
    }

    QJsonArray json = doc.array();

    // If a result is found, extract the coordinates
    if (!json.isEmpty()) {
        double latitude = json[0].toObject()["lat"].toDouble();
        double longitude = json[0].toObject()["lon"].toDouble();
        qDebug() << "Coordinates of city: Latitude:" << latitude << ", Longitude:" << longitude;

        // Use the coordinates (e.g., pass them to your weather API or display on the UI)
    }
    else {
        qDebug() << "No results found for the city.";
    }

    // Clean up the reply object
    reply->deleteLater();
}

QString TestApp::parseWeatherJson(const QString& response) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        QString name = jsonObj["location"].toObject()["name"].toString();
        double tempC = jsonObj["current"].toObject()["temp_c"].toDouble();
        double tempF = jsonObj["current"].toObject()["temp_f"].toDouble();
        QString conditionText = jsonObj["current"].toObject()["condition"].toObject()["text"].toString();
        double windMph = jsonObj["current"].toObject()["wind_mph"].toDouble();
        int humidity = jsonObj["current"].toObject()["humidity"].toInt();
        int cloud = jsonObj["current"].toObject()["cloud"].toInt();

        return QString(
            "City: %1\n"
            "Temperature: %2°C / %3°F\n"
            "Condition: %4\n"
            "Wind Speed: %5 mph\n"
            "Humidity: %6%%\n"
            "Cloud Coverage: %7%%"
        ).arg(name).arg(tempC).arg(tempF).arg(conditionText).arg(windMph).arg(humidity).arg(cloud);
    }
    else {
        return "Invalid JSON response.";
    }
}

void TestApp::updateWeatherLabel(const QString& weatherInfo) {
    qDebug() << "Current thread:" << QThread::currentThread();
    if (QThread::currentThread() != this->thread()) {
        QMetaObject::invokeMethod(this, [this, weatherInfo]() {
            info->setText(weatherInfo);
            }, Qt::QueuedConnection);
    }
    else {
        info->setText(weatherInfo);
    }
}

TestApp::~TestApp() {
    qDebug() << "Destroying WeatherApp";
    if (manager) {
        manager->deleteLater();
        manager = nullptr;
    }
}