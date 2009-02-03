#ifndef ___SCRIPT_DETAIL_RETURN_POLICIES___
#define ___SCRIPT_DETAIL_RETURN_POLICIES___
/// \file
/// Return policies of the functions
///

#include <script/detail/Classes.h>

#include <script/detail/CommonTypes.h>
#include <script/custom/CustomTypes.h>

#include <script/detail/CommonPointers.h>
#include <script/custom/CustomPointers.h>

#include <script/custom/CustomRefs.h>

#include <script/custom/CustomSystem.h>

namespace script {

    ///
    /// default policy, used to push plain types to stack
    ///
    class plain
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            types::push(v, result);
        }
    };

    ///
    /// used to return already on-stack values
    ///
    enum AlreadyAsArgNum {
        argThis = 1,
        arg1,
        arg2,
        arg3,
        arg4,
        arg5,
        arg6,
        arg7,
        arg8
    };

    template<int argNum>
    class alreadyAsArg
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            HSQOBJECT existingObj;
            sq_resetobject(&existingObj);
            sq_getstackobj(v, argNum, &existingObj);
            sq_pushobject(v, existingObj);
        }
    };

    ///
    /// used to put calee back to stack
    ///
    class alreadyAsThis: public alreadyAsArg<argThis>
    {
    };

    ///
    /// creates new instance with no relations with host object lifetime
    ///
    class objNoCare
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            detail::ClassesManager::createObjectInstanceOnStackPure(v, ClassTraits< ptr::pointer<RT>::HostType >::classID(), ptr::pointer<RT>::to(result));
        }
    };

    ///
    /// creates new instance, which deletes host object on destruction
    ///
    class objOwn
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            detail::ClassesManager::createObjectInstanceOnStackPure(v, ClassTraits<ptr::pointer<RT>::HostType>::classID(), ptr::pointer<RT>::to(result));
            sq_setreleasehook(v, -1, _memoryControllerHook<ptr::pointer<RT>::HostType>);
        }

    private:
        template<typename T>
        static SQInteger _memoryControllerHook(SQUserPointer p,SQInteger size)
        {
            T* data = (T*)p;
            jkSCRIPT_DELETE data;
            return 1;
        }
    };

    ///
    /// creates new instance, which control host objects lifetime by custom ptr-class
    ///
    template<typename PtrClass>
    class objPtr
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            detail::ClassesManager::createObjectInstanceOnStackPure(v, ClassTraits<ptr::pointer<RT>::HostType>::classID(), ptr::pointer<RT>::to(result));
            sq_pushinteger(v, detail::ClassesManager::MEMORY_CONTROLLER_PARAM);
            new(sq_newuserdata(v, sizeof(RT))) RT(result);
            sq_setreleasehook(v, -1, _memoryControllerHook<RT>);
            jkSCRIPT_VERIFY(sq_set(v, -3));
        }

    private:
        template<typename T>
        static SQInteger _memoryControllerHook(SQUserPointer p,SQInteger size)
        {
            T* data = (T*)p;
            data->~T();
            return 1;
        }
    };

    ///
    /// creates new instance, which controls host object lifetime by addRef-releaseRef custom methods
    ///
    template<typename RefPolicy>
    class objRefCount
    {
    public:
        template<typename RT>
        static inline void pushResult(HSQUIRRELVM v, RT result)
        {
            typedef typename ptr::pointer<RT>::HostType HostType;
            HostType* obj = ptr::pointer<RT>::to(result);
            RefPolicy::addRef(obj);
            detail::ClassesManager::createObjectInstanceOnStackPure(v, ClassTraits<HostType>::classID(), obj);
            sq_setreleasehook(v, -1, _memoryControllerHook<HostType>);
        }

    private:
        template<typename T>
        static SQInteger _memoryControllerHook(SQUserPointer p,SQInteger size)
        {
            T* data = (T*)p;
            RefPolicy::releaseRef(data);
            return 1;
        }
    };

}	//namespace script

#endif//___SCRIPT_DETAIL_RETURN_POLICIES___
