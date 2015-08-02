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

#include "analysis_utils.h"
#include "transitionanaliser.h"

#include "propertyanalyser.h"
#include "invertibilityanalyser.h"
#include "preservingmatrixanalyser.h"
#include "transition.h"
QString formingAnalyseResultPage(TransitionAnalyser & ta, PropertyAnalyser & pa,
                                 PreservingMatrixAnalyser & pma, InvertibilityAnalyser & ia)
{
    QString out = QObject::tr(
                "<h1>Analysis Results</h1>"
                "<hr />"
                "<h2>Transitions:</h2>"
                "<ul>"
                "<li>Dead : %1 </li>"
                "<li>Potential Dead : %2 </li>"
                "<li>Potential Live :%3 </li>"
                "<li>Live : %4 </li>"
                "<li>Stable : %5 </li>"
                "</ul>").arg(formTransList(ta.deadTransitions()),
                             formTransList(ta.potentialDeadTransitions()),
                             formTransList(ta.potentialLiveTransitions()),
                             formTransList(ta.liveTransitions()),
                             formTransList(ta.stableTransitions())
                             );

    out += QObject::tr(
                "<h2>Petri Net:</h2>"
                "<ul>"
                "<li>Safe: %1 </li>"
                "<li>Bounded: %2 </li>"
                "<li>Live: %3 </li>"
                "<li>Preserve: %4 </li>"
                "<li>Stritly preserve: %5 </li>"
                "<li>Stable: %6 </li>"
                "<li>Parralel: %7 </li>"
                "<li>Conflicted: %8 </li>"
                "<li>Invertable: %9 </li>"
                "</ul>").arg(bToS(pa.isSafety()),
                             bToS(pa.isRestricted()),
                             bToS(ta.isNetLive()),
                             bToS(pma.isPreserving()),
                             bToS(pma.isStrictlyPreserving()),
                             bToS(ta.isNetStable()),
                             bToS(pa.isParallel()),
                             bToS(pa.isConflict()),
                             bToS(ia.isInvertibility())
                             );

    return out;
}


QString bToS(bool b)
{
    return b ? QObject::tr("true") :  QObject::tr("false");
}
QString formTransList(const QSet<Transition*> &transitionSet)
{
    QString out = "";
    for (Transition * t : transitionSet) {
        out += t->name() + ", ";
    }
    if (out != "") {
        out.remove(out.size() - 2 , 2);
    }
    return out;
}
