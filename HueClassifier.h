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

#ifndef __HueClassifier_h__
#define __HueClassifier_h__

#include <QObject>
#include <QImage>
#include <QSize>

struct Spectrum;
struct ClassItem;
struct ClassifyResult {
    int index;
    double confidence;

    ClassifyResult() : index( -1 ), confidence( 0 ) {}
};

class HueClassifier : public QObject
{
    Q_OBJECT
    public:
        HueClassifier( const QSize & tileSize = QSize(), QObject * parent = 0 );
        ~HueClassifier();

        // train the classifier
        void addClass( int index, const QImage & image );
        void deleteClasses( int index );

        // do the classification
        QSize tileSize() const;
        ClassifyResult classify( const QImage & image ) const;

    private:
        void calcSpectra( const QImage & image, Spectrum * h, Spectrum * v ) const;
        double compareSpectra( const Spectrum * a, const Spectrum * b ) const;
        QSize m_tileSize;
        QList< ClassItem * > m_classes;
};

#endif
