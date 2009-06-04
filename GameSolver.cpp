#include "GameSolver.h"
#include "ui_GameSolverGUI.h"

GameSolver::GameSolver(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameSolver)
{
    ui->setupUi(this);
}

GameSolver::~GameSolver()
{
    delete ui;
}
