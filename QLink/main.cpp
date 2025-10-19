#include "menu.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Menu window;
    window.show();
    return app.exec();
}
