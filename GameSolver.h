#ifndef GAMESOLVER_H
#define GAMESOLVER_H

#include <QWidget>
#include <QtScript>

namespace Ui { class GameSolver; }

class GameSolver : public QWidget
{
    Q_OBJECT
    public:
        GameSolver(QWidget *parent = 0);
        ~GameSolver();

    private:
        Ui::GameSolver * ui;
        QScriptEngine * m_scriptEngine;

private slots:
    void on_pushButton_clicked();
};

#endif
