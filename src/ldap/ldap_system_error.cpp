#include "ldap/ldap_system_error.hpp"

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

ldap_system_error::ldap_system_error(
        const std::string& what_arg, unsigned long ldapApiErrorCode)
        : tt4g::ldap::ldap_error(what_arg),
          ldapApiErrorCode_(ldapApiErrorCode)
{

}

ldap_system_error::ldap_system_error(
        const char* what_arg, unsigned long ldapApiErrorCode)
        : tt4g::ldap::ldap_error(what_arg),
          ldapApiErrorCode_(ldapApiErrorCode)
{

}

ldap_system_error::~ldap_system_error()
{

}

unsigned long ldap_system_error::getLdapApiErrorCode() const
{
    return this->ldapApiErrorCode_;
}

} // namespace ldap
} // namespace tt4g
