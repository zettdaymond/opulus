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
#ifndef CMD_CHANGE_PROPERTY_H
#define CMD_CHANGE_PROPERTY_H

#include <QUndoCommand>
#include "exceptions.h"
#include "itemid.h"
#include "petrinet.h"


template<class TObj, class TFunOwner, class TParameter>
class CmdChangeProperty : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(CmdChangeProperty)
public:
    CmdChangeProperty(TObj *obj, 
                      void(TFunOwner::* setter)(TParameter),
                      TParameter newValue,
                      TParameter oldValue,
                      PetriNet* net) :
    mSetter(setter),
    mNewValue(newValue),
    mOldValue(oldValue),
    mNet(net)
    {
        Q_ASSERT(obj);
        mId = obj->id();
    }

    ~CmdChangeProperty() = default;

    void undo() 
    {
        TObj* obj = dynamic_cast<TObj*>(mNet->item(mId));
        Q_ASSERT(obj);

        try {
            (obj->*mSetter)(mOldValue);
        } catch (Exception& e) {
            throw e;
        }   
    }

    void redo() 
    {
        TObj* obj = dynamic_cast<TObj*>(mNet->item(mId));
        Q_ASSERT(obj);

        try {
            (obj->*mSetter)(mNewValue);
        } catch (Exception& e) {
            throw e;
        }
    }
private:
    void(TFunOwner::* mSetter)(TParameter);
    TParameter mNewValue;
    TParameter mOldValue;
    ItemId mId;
    PetriNet* mNet;
};

#endif // CMD_CHANGE_PROPERTY_H
