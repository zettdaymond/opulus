#include <QString>

class TransitionAnalyser;
class PropertyAnalyser;
class PreservingMatrixAnalyser;
QString formingAnalyseResultPage(TransitionAnalyser &ta, PropertyAnalyser &pa,
                          PreservingMatrixAnalyser &pma);
QString bToStr(bool b);
