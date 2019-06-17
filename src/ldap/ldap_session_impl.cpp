#include "ldap/ldap_session_impl.hpp"

namespace tt4g
{
namespace ldap
{

ldap_session_impl::ldap_session_impl(LDAP* session)
        : session_(session)
{

}

ldap_session_impl::~ldap_session_impl()
{
    // ldap_init() returns nullptr if initialization failed.
    if (this->session_) {
        ::ldap_unbind_s(this->session_);
    }
}

LDAP* ldap_session_impl::getSession() const
{
    return this->session_;
}

} // namespace ldap
} // namespace tt4g
