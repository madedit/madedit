#include <base/Base.h>

namespace base {

    Object::~Object()
    { 
        assert(_referenceCounter == 0); 

        if(_weakPtr) {
            detail::WeakPtrImpl* slider = _weakPtr;
            do {
                detail::WeakPtrImpl* next = slider->_next;
                slider->_next = 0;
                slider->_prev = 0;
                slider = next;
            } while(slider != _weakPtr);
        }
    }

}   //namespace base

//eof
//
