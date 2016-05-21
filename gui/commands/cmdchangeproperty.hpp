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
