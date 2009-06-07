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

#ifndef __Capture_h__
#define __Capture_h__

#include "bindings/Bindings.h"
#include <QPixmap>
class ScreenCapture;

namespace Bindings { namespace CV {

/// CV::Capture
class Capture : public QObject, public QScriptable {
    Q_OBJECT
    public:
        Capture(ScreenCapture * capture);

    public Q_SLOTS:
        QScriptValue capture();

    private:
        ScreenCapture * m_capture;
};

} }

#endif
