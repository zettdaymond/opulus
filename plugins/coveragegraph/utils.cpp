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

#include "utils.h"

#include <QDebug>
#include <QFontMetrics>

#ifdef GRAPHVIZ_STATIC_BACKEND

#include "gvc.h"
#include "graph.h"

extern gvplugin_library_t gvplugin_core_LTX_library;
extern gvplugin_library_t gvplugin_dot_layout_LTX_library;

lt_symlist_t lt_preloaded_symbols[] = {
    { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
    { "gvplugin_dot_layout_LTX_library", (void*)(&gvplugin_dot_layout_LTX_library) },
    { 0, 0 }
};

QByteArray utils::renderToGraph(const QString& graphDesc)
{
    GVC_t* gvc = gvContextPlugins(lt_preloaded_symbols, 0);

    Agraph_t* G = agmemread((char*)graphDesc.toStdString().c_str());

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

#include "gvc.h"

QByteArray utils::renderToGraph(const QString& graphDesc)
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

#ifdef OGDF_BACKEND

#include "ogdf/fileformats/GraphIO.h"
#include <sstream>

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/energybased/FMMMLayout.h>

std::pair<int, int> GetTextSize(const ogdf::GraphIO::SVGSettings& settings, const ogdf::Graph& G, const ogdf::GraphAttributes& GA)
{
    QFont font(settings.fontFamily().c_str(), settings.fontSize());
    QFontMetrics metrics(font);

    auto firstNode = *G.nodes.begin();
    QString firstNodeText = GA.label(firstNode).c_str();

    const auto textWidth = metrics.horizontalAdvance(firstNodeText);
    const auto textHeight = metrics.boundingRect(firstNodeText).height();

    return {textWidth, textHeight};
}

QByteArray utils::renderToGraph(QString graphDesc)
{
    using namespace ogdf;

    auto graphString = graphDesc.toStdString();

    std::istringstream ss(graphString);

    Graph G;
    GraphAttributes GA(G, GraphAttributes::all);

    if(!GraphIO::readDOT(GA, G, ss))
    {
        qWarning() << "Could not read dot format";
        return {};
    }


    GraphIO::SVGSettings settings;
    {
        auto [textWidth, textHeight] = GetTextSize(settings, G, GA);

        GA.setAllHeight(textHeight * 1.4);
        GA.setAllWidth(textWidth * 1.2);
    }

    SugiyamaLayout SL;
    SL.setRanking(new OptimalRanking);
    SL.setCrossMin(new MedianHeuristic);

    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
    SL.setLayout(ohl);

    SL.call(GA);

    std::ostringstream oss;


    settings.bezierInterpolation(true);
    settings.curviness(1);

    if(!GraphIO::drawSVG(GA, oss, settings))
    {
        qWarning() << "Could not write svg file from dot format";
        return {};
    }

    std::ofstream fs("out.svg");
    fs << oss.str();

    return QByteArray::fromStdString(oss.str());
}


#endif

#ifndef GRAPHVIZ_BACKEND
#ifndef GRAPHVIZ_STATIC_BACKEND
#ifndef OGDF_BACKEND

QByteArray utils::renderToGraph(const QString& [[maybe_unused]] graphDesc)
{
    return {};
}

#endif //OGDF_BACKEND
#endif //GRAPHVIZ_BACKEND
#endif //GRAPHVIZ_STATIC_BACKEND
