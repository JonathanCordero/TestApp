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

    backgroundItem->setPixmap(pixmap);
    backgroundItem->setOpacity(0.7);
    adjustBackgroundSize();
}

void BackgroundManager::adjustBackgroundSize() {
    if (view && scene) {
        QRectF rect = view->rect();
        scene->setSceneRect(rect);

        if (backgroundItem) {
            backgroundItem->setPixmap(backgroundItem->pixmap().scaled(
                rect.size().toSize(),
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation));
        }
    }
}

void BackgroundManager::resizeBackground(int width, int height) {
    if (scene) {
        scene->setSceneRect(0, 0, width, height); // Resize the scene
    }
}