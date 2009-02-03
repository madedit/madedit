#ifndef ___SCRIPT_CLASSES___
#define ___SCRIPT_CLASSES___

#include <script/VMCore.h>
#include <script/detail/ScriptObject.h>
#include <script/detail/ClassTraits.h>
#include <script/detail/StackHandler.h>
#include <memory.h>

namespace script {
namespace detail {

    class ClassesManager
    {
    public:
        static ScriptObject createClass(HSQUIRRELVM v, ScriptObject& root, ClassID classType, const xchar* name, ClassID parentClass);
        static void createObjectInstanceOnStackPure(HSQUIRRELVM v, ClassID classType, const void* c_this);
        static void createObjectInstanceOnStackPure2(HSQUIRRELVM v, ClassID classType, const void* c_this);

        static void disableCloningForClass(HSQUIRRELVM v, ScriptObject& classObj);
        static void disableCreatingFromScriptForClass(HSQUIRRELVM v, ScriptObject& classObj);
        static void createMemoryControllerSlotForClass(HSQUIRRELVM v, ScriptObject& classObj);

        static const int MEMORY_CONTROLLER_PARAM    = 0x4567C0DE;

    private:
        static ScriptObject _findClass(HSQUIRRELVM v, ClassID classType);
        static SQInteger    _cloneDisabler(HSQUIRRELVM v);
        static SQInteger    _guardedConstructor(HSQUIRRELVM v);
    };

    ScriptObject createTable(HSQUIRRELVM v);

    ScriptObject createArray(HSQUIRRELVM v, int size);

    ScriptObject createFunction(HSQUIRRELVM v, ScriptObject& root, SQFUNCTION func, const SQChar* scriptFuncName, const xchar* typeMask=0);

    //
    // thiscall Functions
    //

	template<typename Func>
	void pushFunctionPointer(HSQUIRRELVM v, Func func)
	{
        SQUserPointer data = sq_newuserdata(v, sizeof(func));
        memcpy(data, &func, sizeof(Func));
	}

    template<typename Func>
    Func getFunctionPointer(HSQUIRRELVM v, int ndx)
    {
        StackHandler sh(v);
        return *(Func*)sh.getUserData(ndx);
    }

    //
    // Static functions
    //

    template<typename Func>
    void pushStaticFunctionPointer(HSQUIRRELVM v, Func func)
    {
        sq_pushuserpointer(v, (void*)func);
    }

    template<typename Func>
    Func getStaticFunctionPointer(HSQUIRRELVM v, int ndx)
    {
        StackHandler sh(v);
        return (Func)sh.getUserPointer(ndx);
    }

    //
    // Events objects
    //

    template<typename EventType>
    void pushEventPointer(HSQUIRRELVM v, EventType ev)
    {
        SQUserPointer data = sq_newuserdata(v, sizeof(ev));
        memcpy(data, &ev, sizeof(EventType));
    }

    template<typename EventType>
    EventType getEventPointer(HSQUIRRELVM v, int ndx)
    {
        StackHandler sh(v);
        return *(EventType*)sh.getUserData(ndx);
    }

    //
    // Fields
    //

	template<typename Field>
	void pushFieldPointer(HSQUIRRELVM v, Field field)
	{
        SQUserPointer data = sq_newuserdata(v, sizeof(field));
        memcpy(data, &field, sizeof(Field));
	}

    template<typename Field>
    Field getFieldPointer(HSQUIRRELVM v, int ndx)
    {
        StackHandler sh(v);
        return *(Field*)sh.getUserData(ndx);
    }

}	//namespace detail
}	//namespace script

#endif//___SCRIPT_CLASSES___
