/***************************************************************************
 * Copyright (c) 2009 Enrico Ros                                           *
 *         2009 Enrico Ros <enrico.ros@email.it>                           *
 *                                                                         *
 * Permission is hereby granted, free of charge, to any person             *
 * obtaining a copy of this software and associated documentation          *
 * files (the "Software"), to deal in the Software without                 *
 * restriction, including without limitation the rights to use,            *
 * copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the               *
 * Software is furnished to do so, subject to the following                *
 * conditions:                                                             *
 *                                                                         *
 * The above copyright notice and this permission notice shall be          *
 * included in all copies or substantial portions of the Software.         *
 *                                                                         *
 ***************************************************************************/

#ifndef __GameSolver_h__
#define __GameSolver_h__

#include <QWidget>
class ScreenCapture;
class ScriptEnvironment;
namespace Ui { class GameSolver; }

class GameSolver : public QWidget
{
    Q_OBJECT
    public:
        GameSolver(QWidget *parent = 0);
        ~GameSolver();

    private:
        Ui::GameSolver * ui;
        ScreenCapture * m_capture;
        ScriptEnvironment * m_script;

    private slots:
        void on_pushButton_2_clicked();
        void on_pushButton_clicked();
};

#endif
