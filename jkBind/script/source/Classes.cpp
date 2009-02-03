#include <script/detail/Classes.h>
#include <script/VMCore.h>

namespace script {
namespace detail {

    ScriptObject ClassesManager::_findClass(HSQUIRRELVM v, ClassID classType)
    {
        ScriptObject result(v);

        VMCore* vm = (VMCore*)sq_getforeignptr(v);
        HSQOBJECT& types = vm->_classesTable;

        sq_pushobject(v, types);
        sq_pushuserpointer(v, classType);
        jkSCRIPT_API_VERIFY(sq_get(v, -2));

        result.attachToStackObject(-1);
        sq_pop(v, 2);

        return result;
    }

    void ClassesManager::createObjectInstanceOnStackPure(HSQUIRRELVM v, ClassID classType, const void* c_this)
    {
        int top = sq_gettop(v);
        ScriptObject classObj = _findClass(v, classType);
        sq_pushobject(v, classObj.handle());

        //stack: types, class
        //..creating instance
        jkSCRIPT_API_VERIFY(sq_createinstance(v, -1));
        
        ScriptObject intance(v);
        intance.attachToStackObject(-1);

        //setting up memory controller
        sq_setinstanceup(v, -1, const_cast<void*>(c_this));

        sq_settop(v, top);
        sq_pushobject(v, intance.getObjectHandle());
    }

    ScriptObject ClassesManager::createClass(HSQUIRRELVM v, ScriptObject& root, ClassID classType, const xchar* name, ClassID parentClass)
    {
        ScriptObject newClass(v);
        int oldtop = sq_gettop(v);
        sq_pushobject(v, root.getObjectHandle());   //root
        sq_pushstring(v, name, -1);                 //root, classname

        if(parentClass) {
            ScriptObject parent = _findClass(v, parentClass);
            sq_pushobject(v, parent.handle());      //root, classname, [parent]
        }

        jkSCRIPT_API_VERIFY(sq_newclass(v, parentClass ? 1 : 0));    //root, classname, class

        newClass.attachToStackObject(-1);
        sq_settypetag(v,-1, classType);             
        sq_newslot(v,-3, false);                    //root, classname, class
        sq_pop(v,1);                                //root

        //
        // adding type to types table
        //
        VMCore* vm = (VMCore*)sq_getforeignptr(v);
        HSQOBJECT& types = vm->_classesTable;
        sq_pushobject(v, types);
        sq_pushuserpointer(v, classType);
        sq_pushobject(v, newClass.getObjectHandle());
        jkSCRIPT_API_VERIFY(sq_newslot(v, -3, false));
        sq_pop(v, 1);

        jkSCRIPT_LOGIC_ASSERT(oldtop == sq_gettop(v));
        return newClass;
    }

    void ClassesManager::disableCloningForClass(HSQUIRRELVM v, ScriptObject& classObj)
    {
        sq_pushobject(v, classObj.handle());        //class
        sq_pushstring(v, xSTRING("_cloned"), -1);   //class, name
        sq_newclosure(v, &_cloneDisabler, 0);       //class, name, func
        jkSCRIPT_API_VERIFY(sq_newslot(v, -3, true));    //class
        sq_pop(v, 1);
    }

    void ClassesManager::disableCreatingFromScriptForClass(HSQUIRRELVM v, ScriptObject& classObj)
    {
        sq_pushobject(v, classObj.handle());            //class
        sq_pushstring(v, xSTRING("constructor"), -1);   //class, name
        sq_newclosure(v, &_guardedConstructor, 0);      //class, name, func
        jkSCRIPT_API_VERIFY(sq_newslot(v, -3, true));        //class
        sq_pop(v, 1);
    }

    void ClassesManager::createMemoryControllerSlotForClass(HSQUIRRELVM v, ScriptObject& classObj)
    {
        sq_pushobject(v, classObj.handle());
        sq_pushinteger(v, MEMORY_CONTROLLER_PARAM);
        sq_pushnull(v);
        jkSCRIPT_API_VERIFY(sq_createslot(v, -3)); 
        sq_pop(v, 1); //popping class
    }

    //
    // closures
    //

    SQInteger ClassesManager::_cloneDisabler(HSQUIRRELVM v)
    {
        jkSCRIPT_ERROR_CLONE_DISABLED;
        return sq_throwerror(v, xSTRING("Cloning for this class is disabled"));
    }

    SQInteger ClassesManager::_guardedConstructor(HSQUIRRELVM v)
    {
        return sq_throwerror(v, xSTRING("Creating this class from script directly is prohibited"));
    }

    //
    // misc
    //

    ScriptObject createTable(HSQUIRRELVM v)
    {
        ScriptObject ret(v);
        sq_newtable(v);
        ret.attachToStackObject(-1);
        sq_pop(v, 1);
        return ret;
    }

    ScriptObject createArray(HSQUIRRELVM v, int size)
    {
        ScriptObject ret(v);
        sq_newarray(v,size);
        ret.attachToStackObject(-1);
        sq_pop(v, 1);
        return ret;
    }

}	//namespace detail
}	//namespace script

//eof
//
