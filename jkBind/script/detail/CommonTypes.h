#ifndef ___SCRIPT_COMMON_TYPES___
#define ___SCRIPT_COMMON_TYPES___

namespace script {
namespace types {

    //
    // push functions - to pass values into script
    //

    inline void push(HSQUIRRELVM v,char value)           { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,unsigned char value)  { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,short value)          { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,unsigned short value) { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,int value)            { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,unsigned int value)   { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,long value)           { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,unsigned long value)  { sq_pushinteger(v,value); }
    inline void push(HSQUIRRELVM v,double value)         { sq_pushfloat(v,(SQFloat)value); }
    inline void push(HSQUIRRELVM v,float value)          { sq_pushfloat(v,(SQFloat)value); }
    inline void push(HSQUIRRELVM v,const SQChar * value) { sq_pushstring(v,value,-1); }
    inline void push(HSQUIRRELVM v,bool value)           { sq_pushbool(v,value); }

    //
    // match functions - to check type of the argument from script
    //

    template<typename T> class TypeSelect {};

    inline bool	match(TypeSelect<bool>, HSQUIRRELVM v,int idx)           { return sq_gettype(v,idx) == OT_BOOL; }
    inline bool	match(TypeSelect<char>, HSQUIRRELVM v,int idx)           { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<unsigned char>, HSQUIRRELVM v, int idx) { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<short>, HSQUIRRELVM v,int idx)          { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<unsigned short>, HSQUIRRELVM v,int idx) { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<int>, HSQUIRRELVM v,int idx)            { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<unsigned int>, HSQUIRRELVM v,int idx)   { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<long>, HSQUIRRELVM v,int idx)           { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<unsigned long>, HSQUIRRELVM v,int idx)  { return sq_gettype(v,idx) == OT_INTEGER; }
    inline bool	match(TypeSelect<float>, HSQUIRRELVM v,int idx)          { int type = sq_gettype(v,idx); return type == OT_FLOAT; }
    inline bool	match(TypeSelect<double>, HSQUIRRELVM v,int idx)         { int type = sq_gettype(v,idx); return type == OT_FLOAT; }
    inline bool	match(TypeSelect<const SQChar *>, HSQUIRRELVM v,int idx) { return sq_gettype(v,idx) == OT_STRING; }

    template<typename T>
    inline bool match(TypeSelect< const T* >, HSQUIRRELVM v,int idx)     { return sq_gettype(v,idx) == OT_INSTANCE; }

    template<typename T>
    inline bool match(TypeSelect< T* >, HSQUIRRELVM v,int idx)     { return sq_gettype(v,idx) == OT_INSTANCE; }

    template<typename T>
    inline bool match(TypeSelect< const T& >, HSQUIRRELVM v,int idx)     { return sq_gettype(v,idx) == OT_INSTANCE; }

    template<typename T>
    inline bool match(TypeSelect< T& >, HSQUIRRELVM v,int idx)     { return sq_gettype(v,idx) == OT_INSTANCE; }

    //
    // get functions - to get values from the script
    //

    inline bool           get(TypeSelect<bool>,HSQUIRRELVM v,int idx)            { SQBool b; jkSCRIPT_API_VERIFY(sq_getbool(v,idx,&b)); return b != 0; }
    inline char           get(TypeSelect<char>,HSQUIRRELVM v,int idx)            { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<char>(i); }
    inline unsigned char  get(TypeSelect<unsigned char>,HSQUIRRELVM v,int idx)   { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<unsigned char>(i); }
    inline short          get(TypeSelect<short>,HSQUIRRELVM v,int idx)           { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<short>(i); }
    inline unsigned short get(TypeSelect<unsigned short>,HSQUIRRELVM v,int idx)  { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<unsigned short>(i); }
    inline int            get(TypeSelect<int>,HSQUIRRELVM v,int idx)             { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return i; }
    inline unsigned int   get(TypeSelect<unsigned int>,HSQUIRRELVM v,int idx)    { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<unsigned int>(i); }
    inline long           get(TypeSelect<long>,HSQUIRRELVM v,int idx)            { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<long>(i); }
    inline unsigned long  get(TypeSelect<unsigned long>,HSQUIRRELVM v,int idx)   { SQInteger i; jkSCRIPT_API_VERIFY(sq_getinteger(v,idx,&i)); return static_cast<unsigned long>(i); }
    inline float          get(TypeSelect<float>,HSQUIRRELVM v,int idx)           { SQFloat f; jkSCRIPT_API_VERIFY(sq_getfloat(v,idx,&f)); return f; }
    inline double         get(TypeSelect<double>,HSQUIRRELVM v,int idx)          { SQFloat f; jkSCRIPT_API_VERIFY(sq_getfloat(v,idx,&f)); return static_cast<double>(f); }
    inline const SQChar * get(TypeSelect<const SQChar *>,HSQUIRRELVM v,int idx)  { const SQChar * s; jkSCRIPT_API_VERIFY(sq_getstring(v,idx,&s)); return s; }

    template<typename T>
    inline const T&       get(TypeSelect< const T& >, HSQUIRRELVM v,int idx)             { T* p; jkSCRIPT_API_VERIFY(sq_getinstanceup(v, idx, (SQUserPointer*)&p, 0));  return *p; }

    template<typename T>
    inline T&             get(TypeSelect< T& >, HSQUIRRELVM v,int idx)                   { T* p; jkSCRIPT_API_VERIFY(sq_getinstanceup(v, idx, (SQUserPointer*)&p, 0));  return *p; }

    template<typename T>
    inline const T*       get(TypeSelect< const T* >, HSQUIRRELVM v,int idx)             { T* p; jkSCRIPT_API_VERIFY(sq_getinstanceup(v, idx, (SQUserPointer*)&p, 0));  return p; }

    template<typename T>
    inline T*             get(TypeSelect< T* >, HSQUIRRELVM v,int idx)                   { T* p; jkSCRIPT_API_VERIFY(sq_getinstanceup(v, idx, (SQUserPointer*)&p, 0));  return p; }
	
}	//namespace types
}	//namespace script

#endif//___SCRIPT_COMMON_TYPES___
