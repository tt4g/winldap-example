#include "ldap/ldap_session.hpp"

#include "ldap/ldap_session_impl.hpp"

#include <utility>

namespace tt4g
{
namespace ldap
{

ldap_session::ldap_session(std::unique_ptr<ldap_session_impl> impl)
        : impl_(std::move(impl))
{

}

ldap_session::~ldap_session()
{

}

const ldap_session_impl& ldap_session::getImpl() const
{
    return *(this->impl_);
}

} // namespace ldap
} // namespace tt4g
