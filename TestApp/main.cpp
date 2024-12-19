#include "TestApp.h"
#include <QApplication>
#include "TestApp.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    TestApp window;
    window.show();

    return app.exec();
}
