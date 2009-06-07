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

#ifndef __Image_h__
#define __Image_h__

#include "bindings/Bindings.h"
#include <QPixmap>

namespace Bindings { namespace CV {

/// CV::Image
class Image : public QObject, public QScriptable {
    Q_OBJECT
    ADD_QTSCRIPT_FACTORY(Image);
    public: static QScriptValue __factory(const QPixmap &, QScriptContext *, QScriptEngine *);
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    public Q_SLOTS:
        int width() const { return m_pixmap.width(); }
        int height() const { return m_pixmap.height(); }
        void trim(int left, int top, int right, int bottom);
        QScriptValue copy(int left, int top, int width, int height);
        QScriptValue split(int rows, int columns);
        void save(const QString & fileName);
        void show(bool modal = false);
    public:
        void setPixmap(const QPixmap & pixmap) { m_pixmap = pixmap; }
        QPixmap pixmap() const { return m_pixmap; }
    private:
        QPixmap m_pixmap;
};

} }

#endif
