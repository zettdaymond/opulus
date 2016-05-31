#include "pluginhandler.h"

#include <QPluginLoader>
#include <QDebug>

#include "analyser.h"

PluginHandler::PluginHandler() : QObject()
{
#ifdef STATIC_PLUGINS
		mObjectNames.push_back(QLatin1String("CoverageGraph"));
		mObjectNames.push_back(QLatin1String("ReachabilityPlugin"));
#endif
}

QVector<QString> PluginHandler::objectNames() const
{
	return mObjectNames;
}

QSet<QString> PluginHandler::loadedPlugins() const
{
	return mLoadedPlugins;
}

Analyser* PluginHandler::loadStaticPlugin(const QString& objectName)
{
	for(QObject* object : QPluginLoader::staticInstances())
	{
		//qDebug() << "Static Plugins found : " << object->objectName();
		if(object->objectName() == objectName)
		{
			Analyser* plugin = qobject_cast< Analyser* >(object);

			if(plugin != 0)
			{
				mLoadedPlugins.insert(objectName);
				return plugin;
			}
		}
	}

	qCritical() << "Could not load static plug-in:" << objectName;

	return nullptr;
}

#ifdef STATIC_PLUGINS
	Q_IMPORT_PLUGIN(CoverageGraph)
	Q_IMPORT_PLUGIN(ReachabilityPlugin)
#endif // STATIC_PLUGINS
