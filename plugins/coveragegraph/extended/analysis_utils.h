#include <QString>

class TransitionAnalyser;
class PropertyAnalyser;
class PreservingMatrixAnalyser;
class InvertibilityAnalyser;
QString formingAnalyseResultPage(TransitionAnalyser &ta, PropertyAnalyser &pa,
                          PreservingMatrixAnalyser &pma, InvertibilityAnalyser &ia);
QString bToStr(bool b);
