#ifndef ___BASE_BASE_H___
#define ___BASE_BASE_H___

#include <base/Types.h>
#include <crtdbg.h>

namespace base
{
    //
    // Forwards
    //
    namespace detail
    {
        class PtrHelper;
        class WeakPtrImpl;
    }
    template<class TYPE> class Ptr;
    template<class TYPE> class WeakPtr;

    
    ///
    /// Base Object class - used for reference counting and automatic object deletion.
    /// Virtual inheritance is recommended.
    ///
    class Object
    {
    public:
        Object(): _referenceCounter(0), _weakPtr(0) {}
        virtual ~Object();
        
        inline int getReferenceCounter() const      { return _referenceCounter; }
        
    private:
        sint                    _referenceCounter;
        detail::WeakPtrImpl*    _weakPtr;
        
        //friends
        friend class detail::PtrHelper;
        friend class detail::WeakPtrImpl;
    };
    
    namespace detail //detail namespace - to avoid compiler bugs
    {
        class WeakPtrImpl
        {
        public:
            inline WeakPtrImpl(): _next(0), _prev(0) {}
            inline WeakPtrImpl(Object* object)
            {
                attach(object);
            }

            ~WeakPtrImpl()
            {
                _ASSERTE(_next == 0);
                _ASSERTE(_prev == 0);
            }

        protected:
            WeakPtrImpl*    _next;
            WeakPtrImpl*    _prev;
            inline void detach(Object* object)
            {
                if (object) {
                    //target exists
                    _ASSERTE(_next);
                    _ASSERTE(_prev);
                    if(_next == this) {
                        //we are the only weakref
                        _ASSERTE(_next == _prev);
                        object->_weakPtr = 0;
                    } else {
                        //we are in the list - unlinking
                        if(object->_weakPtr == this) {
                            object->_weakPtr = _next;
                        }

                        _next->_prev = _prev;
                        _prev->_next = _next;
                    }
                } else {
                    //no target - clearing self
                    _ASSERTE(_next == 0);
                    _ASSERTE(_prev == 0);
                }

                _next = 0;
                _prev = 0;
            }

            inline void attach(Object* object)
            {
                if (object) {
                    //target exists
                    if(object->_weakPtr) {
                        //some other weaks exists, linking self
                        _next = object->_weakPtr->_next;
                        _prev = object->_weakPtr;
                        object->_weakPtr->_next->_prev = this;
                        object->_weakPtr->_next = this;
                    } else {
                        //first weakref
                        object->_weakPtr = this;
                        _next = this;
                        _prev = this;
                    }
                } else {
                    //no target - clearing self
                    _next = 0;
                    _prev = 0;
                }
            }

        //friends
            friend class ::base::Object;
        };

        class PtrHelper 
        {
        public:
            static inline void incrementReferenceCount(Object* object)
            {
                object->_referenceCounter++;
            }
            
            static inline void decrementReferenceCount(Object* object)
            {
                _ASSERTE(object->_referenceCounter > 0);
                object->_referenceCounter--;
                if (object->_referenceCounter == 0) delete object;
            }
        };
    }

    ///
    /// WeakPtr class - weak pointer for Object
    ///
    template<class TYPE> class WeakPtr: private detail::WeakPtrImpl
    {
    public:
        typedef TYPE Type;
        
        WeakPtr(): _object(0) {}
        
        WeakPtr(Type *object) 
            : detail::WeakPtrImpl(static_cast<Object*>(object)) 
        { 
            _object = object; 
        }

        template<typename OtherType>
        WeakPtr(const WeakPtr<OtherType>& ptr) 
            : detail::WeakPtrImpl(static_cast<Object*>(ptr.pointer())) 
        {
            _object = ptr.pointer();
        }

        template<typename OtherType>
        WeakPtr(const Ptr<OtherType>& ptr) 
            : detail::WeakPtrImpl(static_cast<Object*>(ptr.pointer())) 
        {
            _object = ptr.pointer();
        }

        WeakPtr(const WeakPtr& ptr) 
            : detail::WeakPtrImpl(static_cast<Object*>(ptr.pointer())) 
        {
            _object = ptr.pointer();
        }
        
        ~WeakPtr() { detach(pointer()); }
        
        inline WeakPtr& operator=(Type* object) 
        {
            reassign(object);
            return *this;
        }
        
        inline WeakPtr& operator=(const Ptr<Type>& ptr)
        {
            reassign(ptr.pointer());
            return *this;
        }

        inline WeakPtr& operator=(const WeakPtr& ptr)
        {
            reassign(ptr.pointer());
            return *this;
        }
        
        inline Type* operator->() const
        {
            return pointer();
        }
        
        inline Type& operator*() const
        {
            return *pointer();
        }
        
        inline Type* pointer() const
        {
            if(_next) {
                //linked - so object exists
                _ASSERTE(_prev != 0);
                return _object;
            }
            return 0;
        }
        
        inline bool isNull() const
        {
            return _next == 0;
        }
        
        inline bool operator == (const WeakPtr& other) const
        {
            return pointer() == other.pointer();
        }

        inline bool operator == (const Type* other) const
        {
            return pointer() == other;
        }

        inline bool operator != (const WeakPtr& other) const
        {
            return pointer() != other.pointer();
        }

        inline bool operator != (const Type* other) const
        {
            return pointer() != other;
        }

    private:
        void reassign(Type* newValue) 
        {
            detach(pointer());
            attach(newValue);
            _object = newValue;
        }

        Type* _object;
    };
    
    ///
    /// Ptr class - smart pointer with reference counting support
    ///
    template<class TYPE> class Ptr
    {
    public:
        typedef TYPE Type;
        
    private:
        Type* _object;
        
    public:
        Ptr() : _object(0)
        {}
        
        Ptr(Type *object) : _object(object)
        {
            if (_object != 0)
            {
                detail::PtrHelper::incrementReferenceCount(_object);
            }
        }

        template<typename OtherType>
            Ptr(const Ptr<OtherType>& ptr) : _object(static_cast<Type*>(ptr.pointer()))
        {
            if (_object != 0)
            {
                detail::PtrHelper::incrementReferenceCount(_object);
            }
        }

        template<typename OtherType>
            Ptr(const WeakPtr<OtherType>& ptr) : _object(static_cast<Type*>(ptr.pointer()))
        {
            if (_object != 0)
            {
                detail::PtrHelper::incrementReferenceCount(_object);
            }
        }
        
        Ptr(const Ptr& ptr) : _object(ptr._object)
        {
            if (_object != 0)
            {
                detail::PtrHelper::incrementReferenceCount(_object);
            }
        }
        
        ~Ptr()
        {
            if (_object != 0)
            {
                detail::PtrHelper::decrementReferenceCount(_object);
            }
        }
        
        inline Ptr& operator=(Type* object) 
        {
            if(_object != object)
            {
                if (object != 0)
                {
                    detail::PtrHelper::incrementReferenceCount(object);
                }
                if (_object != 0)
                {
                    detail::PtrHelper::decrementReferenceCount(_object);
                }
                _object = object;
            }
            
            return *this;
        }
        
        inline Ptr& operator=(const Ptr& ptr)
        {
            if(_object != ptr._object)
            {
                if (ptr._object != 0)
                {
                    detail::PtrHelper::incrementReferenceCount(ptr._object);
                }
                if (_object != 0)
                {
                    detail::PtrHelper::decrementReferenceCount(_object);
                }
                _object = ptr._object;
            }
            return *this;
        }
        
        inline Type* operator->() const
        {
            return _object;
        }
        
        inline Type& operator*() const
        {
            return *_object;
        }
        
        inline Type* pointer() const
        {
            return _object;
        }
        
        inline bool isNull() const
        {
            return _object == 0;
        }
        
        inline bool operator == (const Ptr& other) const
        {
            return _object == other._object;
        }

        inline bool operator == (const Type* other) const
        {
            return pointer() == other;
        }

        inline bool operator != (const Ptr& other) const
        {
            return _object != other._object;
        }

        inline bool operator != (const Type* other) const
        {
            return pointer() != other;
        }
    };
}

using base::Ptr;
using base::WeakPtr;

#endif  //___BASE_BASE_H___
