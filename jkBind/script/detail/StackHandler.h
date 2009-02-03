#ifndef ___SCRIPT_STACKHANDLER___
#define ___SCRIPT_STACKHANDLER___

#include <script/detail/Checking.h>
#include <script/detail/ScriptObject.h>

#if jkDEBUG_SCRIPT
#   define CHKNDX jkSCRIPT_LOGIC_CHECK( ((idx > 0) && (idx <= _top)) || ((idx < 0) && (-idx <= _top) ));
#else
#   define CHKNDX 
#endif

namespace script {
namespace detail {

    ///
    /// Utility class for stack operations
    ///
    class StackHandler 
    {
    public:
        inline StackHandler(HSQUIRRELVM vm)
            :_top(sq_gettop(vm)),
            v(vm)
        {
        }

        inline SQFloat getFloat(int idx) {
            CHKNDX;
            SQFloat x = 0.0f;
            jkSCRIPT_API_VERIFY(sq_getfloat(v,idx,&x));
            return x;
        }

        inline SQInteger getInt(int idx) {
            CHKNDX
            SQInteger x = 0;
            jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&x));
            return x;
        }

        inline HSQOBJECT getObjectHandle(int idx) {
            CHKNDX
            HSQOBJECT x;
            sq_resetobject(&x);
            jkSCRIPT_API_VERIFY(sq_getstackobj(v,idx,&x));
            return x;
        }

        inline const SQChar* getString(int idx)
        {
            CHKNDX
            const SQChar* x = 0;
            jkSCRIPT_API_VERIFY(sq_getstring(v,idx,&x));
            return x;
        }

        inline SQUserPointer getUserPointer(int idx)
        {
            CHKNDX
            SQUserPointer x = 0;
            jkSCRIPT_API_VERIFY(sq_getuserpointer(v,idx,&x));
            return x;
        }

        inline SQUserPointer getInstanceUp(int idx, SQUserPointer tag)
        {
            CHKNDX
            SQUserPointer self;
            jkSCRIPT_API_VERIFY(sq_getinstanceup(v,idx,(SQUserPointer*)&self,tag));
            return self;
        }

        inline SQUserPointer getUserData(int idx, SQUserPointer tag=0)
        {
            CHKNDX
            SQUserPointer otag;
            SQUserPointer up;
            if(SQ_SUCCEEDED(sq_getuserdata(v,idx,&up,&otag))) {
                if(tag == otag) {
                    return up;
                }
            }
            return 0;
        }

        inline bool getBool(int idx)
        {
            CHKNDX
            SQBool ret = false;
            jkSCRIPT_API_VERIFY(sq_getbool(v,idx,&ret));
            return ret != 0;
        }

        inline SQObjectType getType(int idx)
        {
            CHKNDX
            return sq_gettype(v,idx);
        }

        inline int getParamCount() {
            return _top;
        }

        inline int throwError(const SQChar *error) {
            return sq_throwerror(v,error);
        }

    private:
        int _top;
        HSQUIRRELVM v;
    };

}	//namespace detail
}	//namespace script

#undef CHKNDX

#endif//___SCRIPT_STACKHANDLER___
