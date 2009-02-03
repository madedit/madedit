#ifndef ___SCRIPT_CUSTOM_REFS___
#define ___SCRIPT_CUSTOM_REFS___

#include <base/Base.h>

namespace script {
namespace ref {

/*
    class Ptr
    {
    public:
        template<typename T>
        static inline void addRef(T* obj)
        {
            base::detail::PtrHelper::incrementReferenceCount(obj);
        }

        template<typename T>
        static inline void releaseRef(T* obj)
        {
            base::detail::PtrHelper::decrementReferenceCount(obj);
        }
    };
*/

}	//namespace ref
}	//namespace script

#endif//___SCRIPT_CUSTOM_REFS___
