#include "ldap/ldap_message.hpp"

#include "ldap/ldap_message_impl.hpp"

#include <utility>

namespace tt4g
{
namespace ldap
{

ldap_message::ldap_message(std::unique_ptr<ldap_message_impl> impl)
        : impl_(std::move(impl))
{

}

ldap_message::~ldap_message()
{

}

const ldap_message_impl& ldap_message::getImpl() const
{
    return *(this->impl_);
}

} // namespace ldap
} // namespace tt4g
