#include "cuttermainpage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CutterMainPage w;
    w.Initialize();
    w.show();

    return a.exec();
}
