#include <base/Types.h>
#include <script/Declarator.h>
#include <script/Events.h>

namespace script {
namespace types {

    inline bool	match(TypeSelect<detail::ScriptObject>,HSQUIRRELVM v,int idx)   { return true; }

    inline detail::ScriptObject get(TypeSelect<detail::ScriptObject>,HSQUIRRELVM v,int idx)    { HSQOBJECT handle; sq_getstackobj(v, idx, &handle); detail::ScriptObject p(v, handle); return p; }

}   //namespace types 
}   //namespace script

namespace script {

    SCRIPT_CLASS_REGISTER_BEGIN( script::detail::EventBase )
        .declareNamespace(xSTRING("script"))
            .declareClass<script::detail::EventBase>(xSTRING("Event"))
                .method(xSTRING("isAssigned"),          &script::detail::EventBase::isAssigned)
                .method< alreadyAsThis >(xSTRING("clear"),               &script::detail::EventBase::clear)
                .method< alreadyAsThis >(xSTRING("setHandler"),          &script::detail::EventBase::setHandler)
                .method< alreadyAsThis >(xSTRING("setHandlerObject"),    &script::detail::EventBase::setHandlerObject)
    SCRIPT_CLASS_REGISTER_END( script::detail::EventBase )

    void bindEventsToVMCore(VMCore* coreToBind)
    {
        script::ClassTraits<script::detail::EventBase>::bind(coreToBind);
    };

}	//namespace script

//eof
//
