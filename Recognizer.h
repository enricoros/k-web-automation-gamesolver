/***************************************************************************
 * Copyright (c) 2009 Koral                                                *
 *         2009 Koral <koral@email.it>                                     *
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

#ifndef __Recognizer_h__
#define __Recognizer_h__

#include <QObject>
#include <QPixmap>
#include "HueClassifier.h"

struct RecoResult {
    int rows;
    int columns;
    int total;
    int valid;
    int invalid;

    QVector<int> values;
};

class Recognizer : public QObject
{
    Q_OBJECT
    public:
        Recognizer( HueClassifier * classifier, QObject * parent = 0 );

        // init
        void setup( int hBlocks, int vBlocks );

        // do the job
        RecoResult recognize( const QPixmap & pixmap, float sensitivity, bool verbose );
        QPixmap output() const;

    private:
        HueClassifier * m_classifier;
        int m_hBlocks;
        int m_vBlocks;
        QPixmap m_outPix;

};

#endif
