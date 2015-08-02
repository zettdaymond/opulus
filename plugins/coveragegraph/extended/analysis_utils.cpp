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
    QString out = "";

    out +=  QObject::tr("<h1>Analysis Results</h1>") +
            QObject::tr("<hr />") +
            QObject::tr("<h2>Transitions:</h2>");
    out +=  QObject::tr("<ul>");
    out +=  QObject::tr("<li> Dead :");
    foreach (Transition * t, ta.deadTransitions()) {
        out += t->name() + ", ";
    }
    out += "</li>";

    out +=   QObject::tr("<li> Potential Dead :");
    foreach (Transition * t, ta.potentialDeadTransitions()) {
        out += t->name() + ", ";
    }
    out += "</li>";

    out +=  QObject::tr("<li>Potential Live :");
    foreach (Transition * t, ta.potentialLiveTransitions()) {
        out += t->name() + ", ";
    }
    out += "</li>";

    out +=  QObject::tr("<li>Live :");
    foreach (Transition * t, ta.liveTransitions()) {
        out += t->name() + ", ";
    }
    out += "</li>";

    out +=  QObject::tr("<li>Stable :");
    foreach (Transition * t, ta.stableTransitions()) {
        out += t->name() + ", ";
    }
    out += "</li>";

    out += "</ul>";

    out += QObject::tr("<h2>Petri Net:</h2>");
    out += "<ul>";
    out += QObject::tr("<li>Safe: ") + bToStr(pa.isSafety()) + "</li>";
    out += QObject::tr("<li>Bounded: ") + bToStr(pa.isRestricted()) + "</li>";
    out += QObject::tr("<li>Live: ") + bToStr(ta.isNetLive())+ "</li>";
    out += QObject::tr("<li>Preserve: ") + bToStr(pma.isPreserving())+ "</li>";
    out += QObject::tr("<li>Stritly preserving: ") + bToStr(pma.isStrictlyPreserving())+ "</li>";
    out += QObject::tr("<li>Stable: ") + bToStr(ta.isNetStable())+ "\n";
    out += QObject::tr("<li>Parralel: ") + bToStr(pa.isParallel())+ "</li>";
    out += QObject::tr("<li>Conflicted: ") + bToStr(pa.isConflict())+ "</li>";
    out += QObject::tr("<li>Invertable: ") + bToStr(ia.isInvertibility())+ "</li>";
    out += "</ul>";

    return out;
}


QString bToStr(bool b)
{
    if (b)
        return QObject::tr("true");
    else
        return QObject::tr("false");
}
