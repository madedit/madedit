#ifndef ___SCRIPT_EVENTS___
#define ___SCRIPT_EVENTS___

#include <script/VMCore.h>
#include <base/lang.h>
#include <script/detail/ScriptObject.h>
#include <script/detail/ReturnPolicies.h>

namespace script {
namespace detail {

    class EventBase
    {
    public:
        explicit EventBase();
        virtual ~EventBase();

        inline bool isAssigned() const 
        {
            return _assigned && (_vm.isNull() == false);
        }

        EventBase&  clear();
        EventBase&  setHandler(ScriptObject func);
        EventBase&  setHandlerObject(ScriptObject callee, ScriptObject func);

    protected:
        char                    _callee[sizeof(ScriptObject)];
        char                    _func[sizeof(ScriptObject)];
        WeakPtr<VMCore>         _vm;
        bool                    _assigned;
    };

    void createEventInstance(HSQUIRRELVM v, const void* c_this);

    template<typename Callee, typename EventT>
    SQInteger eventObjectGetter(HSQUIRRELVM v)
    {
        //getting "this"
        StackHandler sa(v);
        Callee* instance(0); 
        instance = (Callee*)sa.getInstanceUp(1, 0);
        int paramCount = sa.getParamCount();

        EventT eventPtr = getEventPointer<EventT>(v, paramCount);

        EventBase* eventObject = &(instance->*eventPtr);

        createEventInstance(v, eventObject);

        return 1;
    }


}   //namespace detail
}	//namespace script

namespace script {

#define CHECK_TYPE_IS_VOID( ndx )                               \
    xCOMPILE_CHECK(base::is_void< T##ndx >::value);

#define PUSH_ARGUMENT( ndx )                                    \
    {                                                           \
        xCOMPILE_CHECK( !base::is_void< Type##ndx >::value );   \
        PP##ndx::pushResult<Type##ndx>                          \
                (vm,static_cast<Type##ndx>(a##ndx));            \
        ++argCount;                                             \
    }

#define BEGIN_CALL                                              \
    HSQUIRRELVM vm = _vm->getVM();                              \
    int argCount = 0;                                           \
    sq_pushobject(vm,(*(detail::ScriptObject*)_func).getObjectHandle());    \
    sq_pushobject(vm,(*(detail::ScriptObject*)_callee).getObjectHandle());    

#if jkDEBUG_SCRIPT

#define END_CALL                                                \
    jkSCRIPT_API_VERIFY(sq_call(vm, 1 + argCount, true, true)); \
    if (!types::match(                                          \
            types::TypeSelect<RetType>(), vm,-1)                \
        )                                                       \
    {                                                           \
        xERROR("Invalid script function result in Event")       \
    }                                                           \
    RetType value =                                             \
        types::get(types::TypeSelect<RetType>(), vm,-1);        \
    sq_pop(vm, 2);                                              \
    return value;

#else 

#define END_CALL                                                \
    jkSCRIPT_API_VERIFY(sq_call(vm, 1 + argCount, true, true)); \
    RetType value =                                             \
        types::get(types::TypeSelect<RetType>(), vm,-1);        \
    sq_pop(vm, 2);                                              \
    return value;

#endif

    template<typename RetType, typename PP1 = plain, typename T1 = void, typename PP2 = plain, typename T2 = void, typename PP3 = plain, typename T3 = void, typename PP4 = plain, typename T4 = void, typename PP5 = plain, typename T5 = void, typename PP6 = plain, typename T6 = void>
    class Event: public detail::EventBase
    {
    private:
        typedef T1 Type1;
        typedef T2 Type2;
        typedef T3 Type3;
        typedef T4 Type4;
        typedef T5 Type5;
        typedef T6 Type6;

    public:
        RetType call()
        {
            BEGIN_CALL
            CHECK_TYPE_IS_VOID(1);
            CHECK_TYPE_IS_VOID(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        };

        template<typename T1>
        RetType call(T1 a1)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            CHECK_TYPE_IS_VOID(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2>
        RetType call(T1 a1, T2 a2)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3>
        RetType call(T1 a1, T2 a2, T3 a3)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            PUSH_ARGUMENT(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            PUSH_ARGUMENT(5);
            PUSH_ARGUMENT(6);
            END_CALL
        }
    };

#undef END_CALL

#define END_CALL                                                    \
    jkSCRIPT_API_VERIFY(sq_call(vm, 1 + argCount, false, true));    \
    sq_pop(vm, 1);                                             

#define RetType void

    template<typename PP1, typename T1, typename PP2, typename T2, typename PP3, typename T3, typename PP4, typename T4, typename PP5, typename T5, typename PP6, typename T6>
    class Event<void, PP1, T1, PP2, T2, PP3, T3, PP4, T4, PP5, T5, PP6, T6>: public detail::EventBase
    {
    private:
        typedef T1 Type1;
        typedef T2 Type2;
        typedef T3 Type3;
        typedef T4 Type4;
        typedef T5 Type5;
        typedef T6 Type6;

    public:
        RetType call()
        {
            BEGIN_CALL
            CHECK_TYPE_IS_VOID(1);
            CHECK_TYPE_IS_VOID(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        };

        template<typename T1>
        RetType call(T1 a1)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            CHECK_TYPE_IS_VOID(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2>
        RetType call(T1 a1, T2 a2)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            CHECK_TYPE_IS_VOID(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3>
        RetType call(T1 a1, T2 a2, T3 a3)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            CHECK_TYPE_IS_VOID(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            CHECK_TYPE_IS_VOID(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            PUSH_ARGUMENT(5);
            CHECK_TYPE_IS_VOID(6);
            END_CALL
        }

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        RetType call(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
        {
            BEGIN_CALL
            PUSH_ARGUMENT(1);
            PUSH_ARGUMENT(2);
            PUSH_ARGUMENT(3);
            PUSH_ARGUMENT(4);
            PUSH_ARGUMENT(5);
            PUSH_ARGUMENT(6);
            END_CALL
        }
    };

#undef RetType

#undef BEGIN_CALL
#undef END_CALL
#undef PUSH_ARGUMENT
#undef CHECK_TYPE_IS_VOID

}	//namespace script

#endif//___SCRIPT_EVENTS___
