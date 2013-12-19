#ifndef H_BOOST_EXT_SCOPED_TYPE
#define H_BOOST_EXT_SCOPED_TYPE

#include "boost/noncopyable.hpp"

namespace boost_ext {
    
    //____________________________________________________________________________//

    // ************************************************************************** //
    // **************                  scoped_type                 ************** //
    // *                                                                        * //
    // * "P" is a class which must implement the following functions            * //
    // *     void _destroy(T& itm) const: delete/destroy/close the item         * //
    // *     T _get_invalid_value() const: return the invalid value for type    * //
    // *                                                                        * //
    // ************************************************************************** //
    template<typename T, typename P>
    class scoped_type : public boost::noncopyable, public P {
    public:
        explicit scoped_type(T itm) throw() : m_itm(itm) {}
        explicit scoped_type() throw() : m_itm(P::_get_invalid_value()) {}
        ~scoped_type() { if (this->is_valid()) { this->_destroy(m_itm); } }
        
        /** Resets the object to a new one - only if it is not the same (destroying the original) */
        scoped_type<T, P>& reset(const T& itm = P::_get_invalid_value()) {
            if (m_itm != itm) {
                if (this->is_valid()) {
                    this->_destroy(m_itm), m_itm = itm;
                } else {
                    m_itm = itm;
                }
            }
            return (*this);
        }
        
        /** Releases the object from control and returns it (without destroying it) */
        T release() {
            T tmp = m_itm;
            m_itm = P::_get_invalid_value();
            return tmp;
        }
        
        /** Returns the underlying object */
        T get() const { return m_itm; }

        /** Returns a pointer to the underlying object */
        T* ptr() { return &(m_itm); }
        
        /** Returns whether or not this object is valid */
        bool is_valid() const { return m_itm != P::_get_invalid_value(); }
        
        /** Operator for casting */
        operator T() const { return this->get(); }

    private:
        T   m_itm;
    };

    //____________________________________________________________________________//

    // ************************************************************************** //
    // **************               based_scoped_type              ************** //
    // ************************************************************************** //
    template<typename B, typename T, typename P>
    class based_scoped_type : public scoped_type<B, P> {
    public:
        explicit based_scoped_type(B itm) throw() : scoped_type<B, P>(itm) {}
        explicit based_scoped_type() throw() : scoped_type<B, P>() {}
        
        /* Override in case we are trying to cast to the subtype */
        // T reset(const T& itm = P::_get_invalid_value()) { return ((T) (scoped_type<B, P>::reset(itm))); }
        T release() { return ((T) (scoped_type<B, P>::release())); }
        T get() const { return ((T) (scoped_type<B, P>::get())); }
        
        operator T() const { return this->get(); }
    };
    
}

#endif
