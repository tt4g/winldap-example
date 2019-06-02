#ifndef TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_
#define TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

class ldap_message_impl
{
public:

    explicit ldap_message_impl(LDAPMessage* message);

    ~ldap_message_impl();

    ldap_message_impl(const ldap_message_impl&) = delete;

    ldap_message_impl(ldap_message_impl&&) = default;

    LDAPMessage* getMessage() const;

    ldap_message_impl& operator=(const ldap_message_impl&) = delete;

    ldap_message_impl& operator=(ldap_message_impl&&) = default;

private:

    LDAPMessage* message_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_
