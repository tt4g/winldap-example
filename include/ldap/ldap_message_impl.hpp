#ifndef TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_
#define TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_

#include <windows.h>
#include <winldap.h>

#include <memory>

namespace tt4g
{
namespace ldap
{

// forward declaration
class ldap_session;
// forward declaration

class ldap_message_impl
{
public:

    explicit ldap_message_impl(
            std::shared_ptr<ldap_session> session,
            LDAPMessage* message);

    ~ldap_message_impl();

    ldap_message_impl(const ldap_message_impl&) = delete;

    ldap_message_impl(ldap_message_impl&&) = default;

    LDAPMessage* getMessage() const;

    ldap_message_impl& operator=(const ldap_message_impl&) = delete;

    ldap_message_impl& operator=(ldap_message_impl&&) = default;

private:

    std::shared_ptr<ldap_session> session_;

    LDAPMessage* message_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_MESSAGE_IMPL_HPP_
