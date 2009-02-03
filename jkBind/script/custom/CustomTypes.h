#ifndef ___SCRIPT_CUSTOM_TYPES___
#define ___SCRIPT_CUSTOM_TYPES___

//#include <base/Fixed.h>

namespace script {
namespace types {

    //
    // pushes
    //

//    inline void push(HSQUIRRELVM v,fixed::real value)    { push(v, (float)value); }

    //
    // matches
    //

//    inline bool	match(TypeSelect<fixed::real>, HSQUIRRELVM v,int idx)       { return match(TypeSelect<float>(), v, idx); }

/*
    template<typename T>
    inline bool match(TypeSelect< Ptr< T > >, HSQUIRRELVM v,int idx)        { return match(TypeSelect<T*>(), v, idx); }

    template<typename T>
    inline bool match(TypeSelect< const Ptr< T >& >, HSQUIRRELVM v,int idx) { return match(TypeSelect<T*>(), v, idx); }
*/

    //
    // getters
    //

//    inline fixed::real  get(TypeSelect<fixed::real>,HSQUIRRELVM v,int idx)          { return fixed::real(get(TypeSelect<float>(), v, idx)); }

/*
    template<typename T>
    inline Ptr<T> get(TypeSelect< Ptr< T > >, HSQUIRRELVM v,int idx)                { return get(TypeSelect<T*>(), v, idx); }

    template<typename T>
    inline const Ptr<T> get(TypeSelect< const Ptr< T >& >, HSQUIRRELVM v,int idx)   { return get(TypeSelect<T*>(), v, idx); }
*/
	
}	//namespace types
}	//namespace script

#endif//___SCRIPT_CUSTOM_TYPES___
