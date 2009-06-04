/***************************************************************************
 * Copyright (c) 2009 Enrico Ros                                           *
 *         2009 Enrico Ros <enrico.ros@email.it>                           *
 *         2009 Alberto Scarpa <skaal.sl@gmail.com>                        *
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

#ifndef __ScreenCapture_h__
#define __ScreenCapture_h__

#include <QObject>
#include <QBasicTimer>
#include <QList>
#include <QImage>
#include <QPixmap>

class ScreenCapture : public QObject
{
    Q_OBJECT
    public:
        // to capture a QWidget
        ScreenCapture( QWidget * target, QObject * parent = 0 );

        // to capture the Screen Contents
        ScreenCapture( QObject * parent = 0 );

        // manual capture
        QPixmap capture( int x = 0, int y = 0, int w = -1, int h = -1 );

        // automatic configuration (will emit 'gotPixmap')
        void setEnabled( bool enabled );
        bool enabled() const;
        void setGeometry( const QRect & geometry );
        QRect geometry() const;
        void setFrequency( int fps );
        int frequency() const;
        QPixmap lastPixmap() const;

    Q_SIGNALS:
        void gotPixmap( const QPixmap & pixmap, const QPoint & cursorPos );

    protected:
        void timerEvent( QTimerEvent * event );

    private:
        QWidget * m_targetWidget;
        QBasicTimer m_timer;
        QPixmap m_pixmap;
        QRect m_geometry;
        bool m_enabled;
        int m_fps;
};

#endif
