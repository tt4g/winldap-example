#ifndef TT4G_LDAP_LDAP_SESSION_IMPL_HPP_
#define TT4G_LDAP_LDAP_SESSION_IMPL_HPP_

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

class ldap_session_impl
{
public:

    explicit ldap_session_impl(LDAP* session);

    ~ldap_session_impl();

    ldap_session_impl(const ldap_session_impl&) = delete;

    ldap_session_impl(ldap_session_impl&&) = delete;

    LDAP* getSession() const;

    ldap_session_impl& operator=(const ldap_session_impl&) = delete;

    ldap_session_impl& operator=(ldap_session_impl&&) = delete;

private:

    LDAP* session_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_SESSION_IMPL_HPP_
