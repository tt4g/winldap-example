#include "ldap/ldap_message_impl.hpp"

#include <memory>

namespace tt4g
{
namespace ldap
{

ldap_message_impl::ldap_message_impl(
        std::shared_ptr<ldap_session> session,
        LDAPMessage* message)
        : message_(message),
          session_(std::move(session))
{

}

ldap_message_impl::~ldap_message_impl()
{
    // ldap_search() returns nullptr if search failed.
    if (this->message_) {
        // LDAP* must survive until LDAPMessage* is released with ldap_msgfree().
        ::ldap_msgfree(this->message_);
    }
    this->session_.reset();
}

LDAPMessage* ldap_message_impl::getMessage() const
{
    return this->message_;
}

} // namespace ldap
} // namespace tt4g
