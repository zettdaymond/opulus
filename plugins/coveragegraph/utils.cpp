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

#include "gvc.h"

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
