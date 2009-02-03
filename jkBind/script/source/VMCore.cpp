#include <script/VMCore.h>
#include <base/Types.h>
#include <script/detail/Checking.h>

namespace script {

    void bindEventsToVMCore(VMCore* coreToBind);

    VMCore::VMCore(int initialStackSize)
        : _vm(0),
        _state(vmsOPENING)
    {
        //creating vm
        _vm = sq_open(initialStackSize);
        sq_setforeignptr(_vm, this);

        //creating types table
        sq_resetobject(&_classesTable);
        sq_pushroottable(_vm);
        sq_pushstring(_vm, xSTRING("____TYPES_TABLE____"), -1);
        sq_newtable(_vm);
        sq_getstackobj(_vm, -1, &_classesTable);
        sq_addref(_vm,&_classesTable);
        jkSCRIPT_API_CHECK(sq_newslot(_vm, 1, false));
        sq_pop(_vm, 1);

        bindEventsToVMCore(this);

        _state = vmsACTIVE;
    }

    VMCore::~VMCore()
    {
        _state = vmsCLOSING;

        //releasing types table
        sq_release(_vm, &_classesTable);
        sq_resetobject(&_classesTable);

        //destroing vm
        sq_close(_vm);
        _vm = 0;
    }

    HSQUIRRELVM VMCore::getVM() const
    {
        return _vm;
    }

    void VMCore::collectGarbage()
    {
        sq_collectgarbage(_vm);
    }

}	//namespace script 

//eof
//
