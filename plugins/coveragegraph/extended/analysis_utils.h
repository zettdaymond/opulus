/*
* Copyright (C) 2015 Zett Daymond zettday@gmail.com and catgirl captainslowpoke@gmail.com
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#ifndef ANALYSIS_UTILS_H
#define ANALYSIS_UTILS_H

#include <QString>
class Transition;
class TransitionAnalyser;
class PropertyAnalyser;
class PreservingMatrixAnalyser;
class InvertibilityAnalyser;
QString formingAnalyseResultPage(TransitionAnalyser &ta, PropertyAnalyser &pa,
                          PreservingMatrixAnalyser &pma, InvertibilityAnalyser &ia);
QString bToS(bool b);
QString formTransList(const QSet<Transition *> &transitionSet);
#endif
