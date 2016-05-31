#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

#include <QObject>
#include <QSet>
#include <QVector>

class Analyser;

class PluginHandler : public QObject
{
public:
	PluginHandler();
	~PluginHandler() = default;
	QVector<QString> objectNames() const;

	Analyser* loadStaticPlugin(const QString& objectName);
	QVector<Analyser*> loadPlugins();
	QSet<QString> loadedPlugins() const;

private:
	QVector<QString> mObjectNames;
	QSet<QString> mLoadedPlugins;
};

#endif // PLUGINHANDLER_H
