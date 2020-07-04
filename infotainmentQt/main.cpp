#include "mainscreen.h"
#include <QApplication>

#define WINDOW_WIDTH    990
#define WINDOW_HEIGHT   512

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainScreen w;
    w.setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    w.show();

    return app.exec();
}
