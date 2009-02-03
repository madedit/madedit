#ifndef ___SCRIPT_DECLARATOR___
#define ___SCRIPT_DECLARATOR___

#include <script/detail/ScriptObject.h>
#include <script/detail/Binding.h>
#include <script/detail/ClassTraits.h>
#include <script/detail/Constructors.h>
#include <script/detail/Classes.h>
#include <script/detail/Fields.h>
#include <script/Events.h>
#include <script/detail/ReturnPolicies.h>

namespace script {
namespace detail {

    ///
    /// The very base declarator. Does nothing, but stores info 
    ///
	class Declarator
	{
    public:
        explicit Declarator(const ScriptObject& hostObject, HSQUIRRELVM vm)
            : _hostObject(hostObject),
            _vm(vm)
        {
        }

        virtual ~Declarator() 
        {
        }

	protected:
        ScriptObject    _hostObject;
        HSQUIRRELVM     _vm;
	};

    ///
    /// Declares new class members in script machine
    ///
    template<typename T>
    class ClassDeclarator: public Declarator
    {
    public:
        typedef T Class;

        ClassDeclarator(ScriptObject& hostObject, HSQUIRRELVM vm)
            :Declarator(hostObject, vm)
        {
        }

        template<typename ReturnPolicy>
        ClassDeclarator&    constructor(const xchar* name)
        {
            return staticMethod<ReturnPolicy>(name, (T* (*)())&constructionFunction<T>);
        }

        template<typename ReturnPolicy, typename A1>
        ClassDeclarator&    constructor(const xchar* name)
        {
            return staticMethod<ReturnPolicy>(name, (T* (*)(A1))&constructionFunction<T, A1>);
        }

        template<typename ReturnPolicy, typename A1, typename A2>
        ClassDeclarator&    constructor(const xchar* name)
        {
            return staticMethod<ReturnPolicy>(name, (T* (*)(A1, A2))&constructionFunction<T, A1, A2>);
        }

        template<typename ReturnPolicy, typename A1, typename A2, typename A3>
        ClassDeclarator&    constructor(const xchar* name)
        {
            return staticMethod<ReturnPolicy>(name, (T* (*)(A1, A2, A3))&constructionFunction<T, A1, A2, A3>);
        }

        template<typename ReturnPolicy, typename A1, typename A2, typename A3, typename A4>
        ClassDeclarator&    constructor(const xchar* name)
        {
            return staticMethod<ReturnPolicy>(name, (T* (*)(A1, A2, A3, A4))&constructionFunction<T, A1, A2, A3, A4>);
        }

        //
        // callee->method
        //

        template<typename ReturnPolicy, typename Func>
        ClassDeclarator&    method(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm, name, -1);
			pushFunctionPointer(_vm, func);
            sq_newclosure(_vm, script::detail::DirectCallInstanceMemberFunction<ReturnPolicy, Class, Func>::Dispatch, 1);

            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping class

            return *this;
        }

        template<typename Func>
        ClassDeclarator&    method(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm, name, -1);
			pushFunctionPointer(_vm, func);
            sq_newclosure(_vm, script::detail::DirectCallInstanceMemberFunction<plain, Class, Func>::Dispatch, 1);

            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping class

            return *this;
        }

        //
        // static method
        //

        template<typename ReturnPolicy, typename Func>
        ClassDeclarator&    staticMethod(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm,name,-1);
            pushStaticFunctionPointer(_vm, func);
            sq_newclosure(_vm, script::detail::DirectCallFunction<ReturnPolicy, Func>::Dispatch,1);
            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping host object

            return *this;
        }

        template<typename Func>
        ClassDeclarator&    staticMethod(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm,name,-1);
            pushStaticFunctionPointer(_vm, func);
            sq_newclosure(_vm, script::detail::DirectCallFunction<plain, Func>::Dispatch,1);
            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping host object

            return *this;
        }

        //
        // script event
        //

        template<typename Event>
        ClassDeclarator&    scriptEvent(const xchar* name, Event event)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm,name,-1);
            pushEventPointer(_vm, event);
            sq_newclosure(_vm, &detail::template eventObjectGetter<Class, Event>, 1);
            jkSCRIPT_API_VERIFY(sq_createslot(_vm,-3));
            sq_pop(_vm, 1); //popping host object
            return *this;
        }

        //
        // fields setter
        //

        template<typename Field>
        ClassDeclarator&    setter(const xchar* name, Field field)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm, name, -1);
			pushFieldPointer(_vm, field);
            sq_newclosure(_vm, script::detail::fieldSetterFunction<Class, Field>, 1);

            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping class

            return *this;
        }

        template<typename Field>
        ClassDeclarator&    getter(const xchar* name, Field field)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm, name, -1);
			pushFieldPointer(_vm, field);
            sq_newclosure(_vm, script::detail::fieldGetterFunction<plain, Class, Field>, 1);

            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping class

            return *this;
        }

        template<typename ReturnPolicy, typename Field>
        ClassDeclarator&    getter(const xchar* name, Field field)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm, name, -1);
			pushFieldPointer(_vm, field);
            sq_newclosure(_vm, script::detail::fieldGetterFunction<ReturnPolicy, Class, Field>, 1);

            jkSCRIPT_API_VERIFY(sq_newslot(_vm,-3, true));
            sq_pop(_vm, 1); //popping class

            return *this;
        }
    };

    ///
    /// Declares new classes, global functions etc in script machine
    ///
    class GlobalDeclarator: public Declarator
    {
    public:
        GlobalDeclarator(const ScriptObject& hostObject, HSQUIRRELVM vm)
            :Declarator(hostObject, vm)
        {
        }

        GlobalDeclarator    declareNamespace(const xchar* name)
        {
            if(!_hostObject.exists(name)) {
                sq_pushobject(_vm, _hostObject.getObjectHandle());
                sq_pushstring(_vm,name,-1);
                sq_newtable(_vm);
                StackHandler sh(_vm);
                ScriptObject newHost(_vm, sh.getObjectHandle(-1));
                jkSCRIPT_API_VERIFY(sq_createslot(_vm,-3));
                sq_pop(_vm, 1);

                return GlobalDeclarator(newHost, _vm);
            }

            return GlobalDeclarator(_hostObject.getValue(name), _vm);
        }

        template<typename T>
        ClassDeclarator<T>  declareClass(const xchar* scriptClassName)
        {
            int top = sq_gettop(_vm);

            ScriptObject newClass = ClassesManager::createClass(_vm, _hostObject, (SQUserPointer)ClassTraits<T>::classID(), scriptClassName, 0);

            ClassesManager::disableCloningForClass(_vm, newClass);
            ClassesManager::disableCreatingFromScriptForClass(_vm, newClass);
            ClassesManager::createMemoryControllerSlotForClass(_vm, newClass);

            sq_settop(_vm,top);
            return ClassDeclarator<T>(newClass, _vm);
        }

        template<typename T, typename PARENT>
        ClassDeclarator<T>  declareClass(const xchar* scriptClassName)
        {
            int top = sq_gettop(_vm);

            ScriptObject newClass = ClassesManager::createClass(_vm, _hostObject, ClassTraits<T>::classID(), scriptClassName, ClassTraits<PARENT>::classID());

            ClassesManager::disableCloningForClass(_vm, newClass);
            ClassesManager::disableCreatingFromScriptForClass(_vm, newClass);
            ClassesManager::createMemoryControllerSlotForClass(_vm, newClass);

            sq_settop(_vm,top);
            return ClassDeclarator<T>(newClass, _vm);
        }

        template<typename ResultPolicy, typename Func>
        GlobalDeclarator    declareFunction(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm,name,-1);
            pushStaticFunctionPointer(_vm, func);
            sq_newclosure(_vm,script::detail::DirectCallFunction<ResultPolicy, Func>::Dispatch,1);
            jkSCRIPT_API_VERIFY(sq_createslot(_vm,-3));
            sq_pop(_vm, 1); //popping host object

            return GlobalDeclarator(_hostObject, _vm);
        }

        template<typename Func>
        GlobalDeclarator    declareFunction(const xchar* name, Func func)
        {
            sq_pushobject(_vm, _hostObject.getObjectHandle());
            sq_pushstring(_vm,name,-1);
            pushStaticFunctionPointer(_vm, func);
            sq_newclosure(_vm,script::detail::DirectCallFunction<plain, Func>::Dispatch,1);
            jkSCRIPT_API_VERIFY(sq_createslot(_vm,-3));
            sq_pop(_vm, 1); //popping host object

            return GlobalDeclarator(_hostObject, _vm);
        }
    };

}	//namespace detail
}	//namespace script

namespace script {

    class RootDeclarator: public detail::GlobalDeclarator
    {
    public:
        RootDeclarator(script::VMCore* targetVM)
            : GlobalDeclarator(_getRoot(targetVM->getVM()), targetVM->getVM())
        {
        }

    private:
        detail::ScriptObject _getRoot(HSQUIRRELVM vm)
        {
            sq_pushroottable(vm);
            detail::StackHandler sh(vm);
            detail::ScriptObject root(vm, sh.getObjectHandle(sh.getParamCount()));
            sq_pop(vm, 1); 
            return root;
        }
    };

}   //namespace script

#endif//___SCRIPT_DECLARATOR___
