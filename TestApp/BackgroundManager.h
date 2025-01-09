#pragma once

#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QString>

class BackgroundManager {
public:
    explicit BackgroundManager(QGraphicsView* view);
    void setBackgroundImage(const QString& path);
    void adjustBackground(const QSize& newSize);
    void resizeBackground(int width, int height);

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* backgroundItem;
};

#endif // BACKGROUNDMANAGER_H