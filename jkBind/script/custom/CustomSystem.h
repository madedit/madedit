#ifndef ___SCRIPT_CUSTOM_SYSTEM___
#define ___SCRIPT_CUSTOM_SYSTEM___
/// \file
/// This is main script binding engine configuration file.
///

#include <crtdbg.h>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////
/// This group of defines sets target script language of the bind system.
///
/// WARNING: current version supports only squirrel.
///
#define jkSCRIPT_LANGUAGE_IS_SQUIRREL 
//#define jkSCRIPT_LANGUAGE_IS_LUA


/////////////////////////////////////////////////////////////////////////////
/// This define specifies definition of the new() and delete() operators.
/// Can be used for custom (debug?) memory allocation.
#define jkSCRIPT_NEW        new
#define jkSCRIPT_DELETE     delete


/////////////////////////////////////////////////////////////////////////////
/// This define should be defined to 1 or 0. If it is defined to 1, all
/// availible checks are performed when interfacing with script, use
/// this value if scripts are unsafe.
/// If scripts are safe, define this to 0 - this will increase perfomace
/// (and memory requirement) of execution.
/// If you dont specify this value, some defaulting will occur.
///

#define jkDEBUG_SCRIPT 1
//#define jkDEBUG_SCRIPT 0


////////////////////////////////////////////////////////////////////////////
/// This define declares assertion, used to check script api call results.
/// It should define a assertion function, which passes if confition is true,
/// and fails if condition is false. Can be defined to raise an exception.
///

#define jkSCRIPT_API_ASSERTION( condidion )             \
    {                                                   \
        const char reason[] = { #condidion };           \
        if((condidion) != true)                         \
            __debugbreak();                             \
    }


////////////////////////////////////////////////////////////////////////////
/// This define declares assertion, used to check runtime logic conditions.
/// It should define a assertion function, which passes if confition is true,
/// and fails if condition is false. Can be defined to raise an exception.
///

#define jkSCRIPT_LOGIC_ASSERTION( condidion ) jkSCRIPT_API_ASSERTION( condidion )


////////////////////////////////////////////////////////////////////////////
/// This key specifies, if several virtual machines can be created at the same time.
/// If it is set to 1, many v-machines can be created and their binded classes can
/// differ (actually, v-machines will be completly separated, even thread-safe).
///
/// If this key set to 0, only one vm can exists at the same time, but perfomance 
/// is better (it's a good choise in most cases. I havn't seen game application
/// with several v-machines yet). 
///
/// WARNING: single vm mode is NOT supported in this release
///

#define jkSCRIPT_MULTI_VM 1
//#define jkSCRIPT_SINGLE_VM 0

#define jkSCRIPT_ERROR_CLONE_DISABLED jkSCRIPT_API_ASSERTION(("Cloning for this class is disabled", 0));

#endif//___SCRIPT_CUSTOM_SYSTEM___
