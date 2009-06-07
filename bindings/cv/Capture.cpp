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

#include "bindings/cv/Capture.h"
#include "bindings/cv/Image.h"
#include "ScreenCapture.h"
namespace Bindings { namespace CV {

Capture::Capture(ScreenCapture * capture)
    : m_capture(capture)
{
}

QScriptValue Capture::capture()
{
    QPixmap cPixmap = m_capture->capture();
    return Bindings::CV::Image::__factory(cPixmap, context(), engine());
}

}} // namespace Bindings::CV
