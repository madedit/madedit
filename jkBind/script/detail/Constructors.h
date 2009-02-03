#ifndef ___SCRIPT_DETAIL_CONSTRUCTORS___
#define ___SCRIPT_DETAIL_CONSTRUCTORS___

#include <script/custom/CustomSystem.h>

namespace script {
namespace detail {

    template<typename T>
    T* constructionFunction()
    {
        return jkSCRIPT_NEW T();
    }

    template<typename T, typename A1>
    T* constructionFunction(A1 a1)
    {
        return jkSCRIPT_NEW T(a1);
    }

    template<typename T, typename A1, typename A2>
    T* constructionFunction(A1 a1, A2 a2)
    {
        return jkSCRIPT_NEW T(a1, a2);
    }

    template<typename T, typename A1, typename A2, typename A3>
    T* constructionFunction(A1 a1, A2 a2, A3 a3)
    {
        return jkSCRIPT_NEW T(a1, a2, a3);
    }

    template<typename T, typename A1, typename A2, typename A3, typename A4>
    T* constructionFunction(A1 a1, A2 a2, A3 a3, A4 a4)
    {
        return jkSCRIPT_NEW T(a1, a2, a3, a4);
    }

}   //namespace detail
}   //namespace script

#endif//___SCRIPT_DETAIL_CONSTRUCTORS___
