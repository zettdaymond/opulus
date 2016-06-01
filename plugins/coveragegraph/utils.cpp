#include "utils.h"

#include <QDebug>

#ifdef GRAPHVIZ_STATIC_BACKEND

#include "graphviz/gvplugin.h"
#include "graphviz/gvc.h"

extern gvplugin_library_t gvplugin_core_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;

lt_symlist_t lt_preloaded_symbols[] = {
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
    { "gvplugin_dot_layout_LTX_library", (void*)(&gvplugin_dot_layout_LTX_library) },
    { 0, 0 }
};

QByteArray utils::renderToGraph(QString graphDesc)
{
    GVC_t* gvc = gvContextPlugins(lt_preloaded_symbols, 0);

	Agraph_t* G = agmemread(graphDesc.toStdString().c_str());

	if (G == nullptr) {
		qCritical() << "Could not create graph from generated string.";
	}
    gvLayout (gvc, G, "dot");

	char* result = nullptr;
	unsigned int length;
    gvRenderData (gvc, G, "svg", &result, &length);

	//Do
	//gvFreeRenderData(result);

    gvFreeLayout(gvc, G);
    agclose (G);

	return QByteArray(result, length);
}

#endif //GRAPHVIZ_STATIC_BACKEND

#ifdef GRAPHVIZ_BACKEND

#include "graphviz/gvc.h"

QByteArray utils::renderToGraph(QString graphDesc)
{
	GVC_t* gvc = gvContext();

	Agraph_t* G = agmemread(graphDesc.toStdString().c_str());

	if (G == nullptr) {
		qCritical() << "Could not create graph from generated string.";
	}
	gvLayout (gvc, G, "dot");

	char* result = nullptr;
	unsigned int length;
	gvRenderData (gvc, G, "svg", &result, &length);

	//Do
	//gvFreeRenderData(result);

	gvFreeLayout(gvc, G);
	agclose (G);

	return QByteArray(result, length);
}

#endif //GRAPHVIZ_BACKEND

#ifndef GRAPHVIZ_BACKEND
#ifndef GRAPHVIZ_STATIC_BACKEND

QByteArray utils::renderToGraph(QString graphDesc)
{
    //TODO: Return "not supportefd in this build" image.
    return QByteArray(0);
}

#endif //OGDF_BACKEND
#endif //GRAPHVIZ_BACKEND
