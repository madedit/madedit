#include <script/Events.h>
#include <script/detail/Classes.h>
#include <script/Declarator.h>
#include <stddef.h>
#include <new>

#include <memory.h>

namespace script {
namespace detail {

    EventBase::EventBase()
        :_assigned(false),
        _vm(0)
    {
        clear();
    }

    EventBase::~EventBase()
    {
        clear();
    }

    EventBase& EventBase::clear()
    {
        if(_assigned) {
            if(_vm.isNull() == false) {
                if(_vm->state() == VMCore::vmsACTIVE) {
                    ((ScriptObject*)_func)->~ScriptObject();
                    ((ScriptObject*)_callee)->~ScriptObject();
                }
            }
        }
        memset(_func, 0, sizeof(ScriptObject));
        memset(_callee, 0, sizeof(ScriptObject));
        _vm = 0;
        _assigned = false;

        return *this;
    }

    EventBase& EventBase::setHandler(ScriptObject func)
    {
        _vm = reinterpret_cast<VMCore*>(sq_getforeignptr(func.getHostVirtualMachine()));

        sq_pushroottable(_vm->getVM());
        HSQOBJECT x;
        sq_resetobject(&x);
        jkSCRIPT_API_VERIFY(sq_getstackobj(_vm->getVM(),-1,&x));

        ScriptObject callee(_vm->getVM(), x);

        sq_pop(_vm->getVM(), 1);

        setHandlerObject(callee, func);

        return *this;
    }

    EventBase& EventBase::setHandlerObject(ScriptObject callee, ScriptObject func)
    {
        new (_func) ScriptObject(func);
        new (_callee) ScriptObject(callee);
        _vm = reinterpret_cast<VMCore*>(sq_getforeignptr(func.getHostVirtualMachine()));

        jkSCRIPT_LOGIC_ASSERT(func.getHostVirtualMachine() == callee.getHostVirtualMachine());
        jkSCRIPT_LOGIC_ASSERT(func.isNull() == false);
        jkSCRIPT_LOGIC_ASSERT(callee.isNull() == false);
        jkSCRIPT_LOGIC_ASSERT(_vm->getVM() != 0);

        _assigned = true;

        return *this;
    }

    void createEventInstance(HSQUIRRELVM v, const void* c_this)
    {
        detail::ClassesManager::createObjectInstanceOnStackPure(v, ClassTraits< EventBase >::classID(), c_this);
    }

}   //namespace detail
}   //namespace script

//eof
//
