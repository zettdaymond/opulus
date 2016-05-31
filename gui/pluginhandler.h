#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

#include <QObject>
#include <QVector>

class Analyser;

class PluginHandler : public QObject
{
public:
	PluginHandler();
	~PluginHandler() = default;
	QVector<QString> objectNames() const;

	Analyser* loadStaticAnalizerPlugin(const QString& objectName);
private:
	QVector<QString> mObjectNames;
};

#endif // PLUGINHANDLER_H
