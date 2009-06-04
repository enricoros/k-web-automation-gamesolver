#include <QtGui/QApplication>
#include "GameSolver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameSolver w;
    w.show();
    return a.exec();
}
