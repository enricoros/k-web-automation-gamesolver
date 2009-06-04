#ifndef GAMESOLVER_H
#define GAMESOLVER_H

#include <QtGui/QWidget>

namespace Ui
{
    class GameSolver;
}

class GameSolver : public QWidget
{
    Q_OBJECT

public:
    GameSolver(QWidget *parent = 0);
    ~GameSolver();

private:
    Ui::GameSolver *ui;
};

#endif // GAMESOLVER_H
