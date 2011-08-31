
#if !defined(_SCRAT_ARRAY_H_)
#define _SCRAT_ARRAY_H_

#include <squirrel.h>
#include <string.h>

#include "sqratObject.h"
#include "sqratFunction.h"
#include "sqratGlobalMethods.h"

namespace Sqrat {

	class ArrayBase : public Object {
	public:
		ArrayBase(HSQUIRRELVM v = DefaultVM::Get()) : Object(v, true) {
		}

		ArrayBase(const Object& obj) : Object(obj) {
		}

		// Bind a Table or Class to the Array (Can be used to facilitate Namespaces)
		// Note: Bind cannot be called "inline" like other functions because it introduces order-of-initialization bugs.
		void Bind(const SQInteger index, Object& obj) {
			sq_pushobject(vm, GetObject());
			sq_pushinteger(vm, index);
			sq_pushobject(vm, obj.GetObject());
			sq_set(vm, -3);
			sq_pop(vm,1); // pop array
		}
		
		// Bind a raw Squirrel closure to the Array
		ArrayBase& SquirrelFunc(const SQInteger index, SQFUNCTION func) {
			sq_pushobject(vm, GetObject());
			sq_pushinteger(vm, index);
			sq_newclosure(vm, func, 0);
			sq_set(vm, -3);
			sq_pop(vm,1); // pop array
			return *this;
		}

		//
		// Variable Binding
		//
		
		template<class V>
		ArrayBase& SetValue(const SQInteger index, const V& val) {
			sq_pushobject(vm, GetObject());
			sq_pushinteger(vm, index);
			PushVar(vm, val);
			sq_set(vm, -3);
			sq_pop(vm,1); // pop array
			return *this;
		}

		template<class V>
		ArrayBase& SetInstance(const SQInteger index, V* val) {
			BindInstance<V>(name, index, false);
			return *this;
		}

		template<class F>
		ArrayBase& Func(const SQInteger index, F method) {
			BindFunc(index, &method, sizeof(method), SqGlobalFunc(method));
			return *this;
		}

		//template<class F>
		//ArrayBase& Overload(const SQChar* name, F method) {
		//	BindOverload(name, &method, sizeof(method), SqGlobalFunc(method), SqOverloadFunc(method), SqGetArgCount(method));
		//	return *this;
		//}

		//
		// Function Calls
		//

		Function GetFunction(const SQInteger index) {
			HSQOBJECT funcObj;
			sq_pushobject(vm, GetObject());
			sq_pushinteger(vm, index);
			if(SQ_FAILED(sq_get(vm, -2))) {
				sq_pushnull(vm);
			}
			sq_getstackobj(vm, -1, &funcObj);
			Function ret(vm, GetObject(), funcObj);
			sq_pop(vm, 2);

			return ret;
		}

		//
		// Array manipulation
		//

		template<class V>
		ArrayBase& Append(const V& val) {
			sq_pushobject(vm, GetObject());
			PushVar(vm, val);
			sq_arrayappend(vm, -2);
			sq_pop(vm,1); // pop array
			return *this;
		}

		template<class V>
		ArrayBase& Append(V* val) {
			sq_pushobject(vm, GetObject());
			PushVar(vm, val);
			sq_arrayappend(vm, -2);
			sq_pop(vm,1); // pop array
			return *this;
		}

		template<class V>
		ArrayBase& Insert(const SQInteger destpos, const V& val) {
			sq_pushobject(vm, GetObject());
			PushVar(vm, val);
			sq_arrayinsert(vm, -2, destpos);
			sq_pop(vm,1); // pop array
			return *this;
		}

		template<class V>
		ArrayBase& Insert(const SQInteger destpos, V* val) {
			sq_pushobject(vm, GetObject());
			PushVar(vm, val);
			sq_arrayinsert(vm, -2, destpos);
			sq_pop(vm,1); // pop array
			return *this;
		}

		Object Pop() {
			HSQOBJECT slotObj;
			sq_pushobject(vm, GetObject());
			if(SQ_FAILED(sq_arraypop(vm, -1, true))) {
				sq_pop(vm, 1);
				return Object(); // Return a NULL object
			} else {
				sq_getstackobj(vm, -1, &slotObj);
				Object ret(slotObj, vm);
				sq_pop(vm, 2);
				return ret;
			}
		}

		ArrayBase& Remove(const SQInteger itemidx) {
			sq_pushobject(vm, GetObject());
			sq_arrayremove(vm, -1, itemidx);
			sq_pop(vm,1); // pop array
			return *this;
		}

		ArrayBase& Resize(const SQInteger newsize) {
			sq_pushobject(vm, GetObject());
			sq_arrayresize(vm, -1, newsize);
			sq_pop(vm,1); // pop array
			return *this;
		}

		ArrayBase& Reverse() {
			sq_pushobject(vm, GetObject());
			sq_arrayreverse(vm, -1);
			sq_pop(vm,1); // pop array
			return *this;
		}
	};

	class Array : public ArrayBase {
	public:
		Array(HSQUIRRELVM v = DefaultVM::Get(), const SQInteger size = 0) : ArrayBase(v) {
			sq_newarray(vm, size);
			sq_getstackobj(vm,-1,&obj);
			sq_addref(vm, &obj);
			sq_pop(vm,1);
		}

		Array(const Object& obj) : ArrayBase(obj) {
		}
	};
}

#endif
