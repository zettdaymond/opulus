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
        return "true";
    else
        return "false";
}
