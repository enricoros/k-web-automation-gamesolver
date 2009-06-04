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

#include "HueClassifier.h"
#include <QVector>
#include <QColor>
#include <math.h>

struct Spectrum {
    QVector<float> hue;
    QVector<float> power;
    QVector<float> weight;
};

struct ClassItem {
    int index;
    QImage image;
    Spectrum hs, vs;
};

HueClassifier::HueClassifier( const QSize & tileSize, QObject * parent )
    : QObject( parent )
    , m_tileSize( tileSize )
{
}

HueClassifier::~HueClassifier()
{
    qDeleteAll( m_classes );
}

void HueClassifier::addClass( int index, const QImage & image )
{
    if ( m_tileSize.isNull() ) {
        m_tileSize = image.size();
        qWarning( "HueClassifier::addClass: assuming the size (%d, %d) from the first sample", image.width(), image.height() );
    }
    else if ( image.size() != m_tileSize ) {
        qWarning( "HueClassifier::addClass: we only support '%dx%d' size images; won't classify this one", m_tileSize.width(), m_tileSize.height() );
        return;
    }

    // build the information holder (including statistical description)
    ClassItem * ci = new ClassItem();
    m_classes.append( ci );
    ci->index = index;
    ci->image = image;
    calcSpectra( ci->image, &ci->hs, &ci->vs );
}

void HueClassifier::deleteClasses( int index )
{
    QList< ClassItem * >::iterator it = m_classes.begin();
    while ( it != m_classes.end() ) {
        if ( (*it)->index == index ) {
            delete *it;
            it = m_classes.erase( it );
        } else
            ++it;
    }
}

QSize HueClassifier::tileSize() const
{
    return m_tileSize;
}

ClassifyResult HueClassifier::classify( const QImage & image ) const
{
    // get ready with the response structure
    ClassifyResult cr;
    cr.index = -1;
    cr.confidence = 0;
    if ( image.size() != m_tileSize ) {
        qWarning( "HueClassifier::classify: we only support '%dx%d' size images; won't classify this one", m_tileSize.width(), m_tileSize.height() );
        return cr;
    }

    // find out the statistical description about the image
    Spectrum hs, vs;
    calcSpectra( image, &hs, &vs );

    // match the image spectral description with the databases
    foreach ( const ClassItem * c, m_classes ) {
        double confidence = 0.5 * (compareSpectra( &hs, &c->hs ) + compareSpectra( &vs, &c->vs ));
        if ( confidence > cr.confidence ) {
            cr.index = c->index;
            cr.confidence = confidence;
        }
    }

    //qWarning( "i: %d, conf %f", cr.index, cr.confidence );

    return cr;
}

void HueClassifier::calcSpectra( const QImage & image, Spectrum * h, Spectrum * v ) const
{
    int W = image.width();
    int H = image.height();
    const quint32 * imageBasePtr = (const quint32 *)image.bits();

    // H accumulation {hue, power}
    h->hue.resize( H );
    h->power.resize( H );
    double hPower = 0;
    for ( int j = 0; j < H; j++ ) {
        const quint32 * linePtr = imageBasePtr + (W * j);
        quint32 r = 0, g = 0, b = 0;
        for ( int k = 0; k < W; k++ ) {
            quint32 color = *linePtr++;
            r += color & 0xFF;
            g += (color >> 8) & 0xFF;
            b += (color >> 16) & 0xFF;
        }
        QColor color( r / W, g / W, b / W );
        h->hue[ j ] = color.hueF();
        //h->power[ j ] = (float)color.value() / 255.0;
        h->power[ j ] = (float)color.value() * color.saturation() / (255*255);
        hPower += h->power[ j ];
    }
    h->weight.resize( H );
    for ( int j = 0; j < H; j++ )
        h->weight[ j ] = hPower ? H * h->power[ j ] / hPower : 0.0;

    // V accumulation {hue, power}
    v->hue.resize( W );
    v->power.resize( W );
    double vPower = 0;
    for ( int j = 0; j < W; j++ ) {
        const quint32 * columnPtr = imageBasePtr + j;
        quint32 r = 0, g = 0, b = 0;
        for ( int k = 0; k < H; k++ ) {
            quint32 color = *columnPtr;
            r += color & 0xFF;
            g += (color >> 8) & 0xFF;
            b += (color >> 16) & 0xFF;
            columnPtr += W;
        }
        QColor color( r / H, g / H, b / H );
        v->hue[ j ] = color.hueF();
        //v->power[ j ] = (float)color.value() / 255.0;
        v->power[ j ] = (float)color.value() * color.saturation() / (255*255);
        vPower += v->power[ j ];
    }
    v->weight.resize( W );
    for ( int j = 0; j < W; j++ )
        v->weight[ j ] = vPower ? W * v->power[ j ] / vPower : 0.0;
}

double HueClassifier::compareSpectra( const Spectrum * a, const Spectrum * b ) const
{
    double confidence = 0;
    int size = a->hue.size();

    for ( int j = 0; j < size; j++ ) {
        double alpha = 6.28318530718 * (double)(a->hue[ j ] - b->hue[ j ]);
        double fA = cos( alpha ) - fabs( sin( alpha ) );
        confidence += fA * a->weight[ j ] * b->weight[ j ];
    }
    confidence /= size;

    return confidence;
}
