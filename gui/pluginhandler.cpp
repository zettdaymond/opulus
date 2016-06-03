/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
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
