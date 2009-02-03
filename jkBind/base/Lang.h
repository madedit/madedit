#ifndef ___BASE_LANG___
#define ___BASE_LANG___

namespace base {

    //
    // Base definitions
    //

    template <typename T, T val>
    class integral_constant
    {
    public:
        typedef integral_constant<T, val>   type;
        typedef T                           value_type;

        static const T  value = val;

        operator T() const { return static_cast<T>(this->value); }
    };

    typedef integral_constant<bool,true>    true_type;
    typedef integral_constant<bool,false>   false_type;

    //
    // "is_void" family
    //

    template<typename T>
    class is_void: public false_type {};

    template<>
    class is_void<void>: public true_type {};

}	//namespace base

#define xCOMPILE_CHECK( arg ) { typedef int condition_check[arg]; }

#endif//___BASE_LANG___
