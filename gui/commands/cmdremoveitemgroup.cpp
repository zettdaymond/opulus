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

#include "cmdremoveitemgroup.h"
#include "petrinet.h"
#include "transition.h"
#include "place.h"
#include "item.h"
#include <QDebug>

CmdRemoveItemGroup::CmdRemoveItemGroup(QVector<Item*>& items)
{
	for (auto item : items) {
		Q_ASSERT(item);
		mIds.push_back(item->id());
	}
	Q_ASSERT(items.size() > 0);

	mPetriNet = items[0]->petriNet();
	Q_ASSERT(mPetriNet);

	setText(tr("Remove item"));
}

CmdRemoveItemGroup::~CmdRemoveItemGroup() {
	qDeleteAll(mItems);
}

void CmdRemoveItemGroup::undo() {
	std::reverse(mItems.begin(), mItems.end());
	QVector<Item*> v;
	for (auto* i : mItems) {
		v.push_back(i);
	}
	mPetriNet->addItemGroup(v);
	mItems.clear();
}

void CmdRemoveItemGroup::redo() {
	//FIXME: does we need to call destructor?
	mItems.clear();
	mItems += mPetriNet->removeItemGroup(mIds);
}
