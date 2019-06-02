#include "ldap/ldap_message_impl.hpp"

namespace tt4g
{
namespace ldap
{

ldap_message_impl::ldap_message_impl(LDAPMessage* message)
        : message_(message)
{

}

ldap_message_impl::~ldap_message_impl()
{
    ::ldap_msgfree(this->message_);
}

LDAPMessage* ldap_message_impl::getMessage() const
{
    return this->message_;
}

} // namespace ldap
} // namespace tt4g
