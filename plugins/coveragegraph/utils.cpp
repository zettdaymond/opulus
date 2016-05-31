#include "utils.h"

#include <QDebug>

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
