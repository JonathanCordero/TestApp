#include "BackgroundManager.h"
#include <QPixmap>
#include <QDebug>

BackgroundManager::BackgroundManager(QGraphicsView* BG)
    : view(BG), scene(new QGraphicsScene(BG)), backgroundItem(nullptr) {
    
    BG->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align the content
    BG->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow resizing
    
    BG->setScene(scene);

    // Optionally set a transparent background to the scene or view
    BG->setStyleSheet("background: transparent;");

    // Ensure the view is behind other elements
    BG->lower();
}



void BackgroundManager::setBackgroundImage(const QString& path) {
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load background image from:" << path;
        return;
    }

    if (!backgroundItem) {
        backgroundItem = new QGraphicsPixmapItem();
        scene->addItem(backgroundItem);
    }

    // Resize the pixmap to fit the view's size
    QSize viewSize = view->viewport()->size(); // Get the size of the view
    QPixmap scaledPixmap = pixmap.scaled(viewSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    backgroundItem->setPixmap(scaledPixmap);
    backgroundItem->setOpacity(0.7); // Set transparency
    backgroundItem->setZValue(-1);  // Ensure it's in the background
}

void BackgroundManager::adjustBackground(const QSize& newSize) {
    if (!backgroundItem) return;

    // Adjust the background pixmap to fit the new size
    QPixmap scaledPixmap = backgroundItem->pixmap().scaled(
        newSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    backgroundItem->setPixmap(scaledPixmap);

    // Update the scene rectangle to fit the new size
    scene->setSceneRect(0, 0, newSize.width(), newSize.height());
}

void BackgroundManager::resizeBackground(int width, int height) {
    if (scene) {
        scene->setSceneRect(0, 0, width, height); // Resize the scene
    }
}