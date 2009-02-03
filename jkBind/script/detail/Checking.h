#ifndef ___SCRIPT_CHECKING___
#define ___SCRIPT_CHECKING___

#include <script/custom/CustomSystem.h>

//
// Defaulting main script debug switch
//
#ifndef jkDEBUG_SCRIPT
#   ifdef xDEBUG
#       define jkDEBUG_SCRIPT 1
#   else
#       define jkDEBUG_SCRIPT 0
#   endif
#endif

#if jkDEBUG_SCRIPT
#   define jkSCRIPT_API_CHECK(arg)       jkSCRIPT_API_ASSERTION( SQ_SUCCEEDED((arg)) )
#   define jkSCRIPT_API_ASSERT(arg)      jkSCRIPT_API_ASSERTION( SQ_SUCCEEDED((arg)) )
#   define jkSCRIPT_API_VERIFY(arg)      jkSCRIPT_API_ASSERTION( SQ_SUCCEEDED((arg)) )

#   define jkSCRIPT_LOGIC_CHECK(arg)     jkSCRIPT_LOGIC_ASSERTION( arg )
#   define jkSCRIPT_LOGIC_ASSERT(arg)    jkSCRIPT_LOGIC_ASSERTION( arg )
#   define jkSCRIPT_LOGIC_VERIFY(arg)    jkSCRIPT_LOGIC_ASSERTION( arg )
#else
#   define jkSCRIPT_API_CHECK(arg)       jkSCRIPT_API_ASSERTION( SQ_SUCCEEDED((arg)) )
#   define jkSCRIPT_API_ASSERT(arg)      
#   define jkSCRIPT_API_VERIFY(arg)      (arg)

#   define jkSCRIPT_LOGIC_CHECK(arg)     jkSCRIPT_LOGIC_ASSERTION( arg )
#   define jkSCRIPT_LOGIC_ASSERT(arg)      
#   define jkSCRIPT_LOGIC_VERIFY(arg)    (arg)
#endif


#endif//___SCRIPT_CHECKING___
