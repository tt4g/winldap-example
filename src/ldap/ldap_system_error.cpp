#include "ldap/ldap_system_error.hpp"

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

ldap_system_error::ldap_system_error(
        const std::string& what_arg, unsigned long ldapSystemErrorCode)
        : tt4g::ldap::ldap_error(
                std::string(what_arg).append(": ").append(::ldap_err2stringA(ldapSystemErrorCode)))
{

}

ldap_system_error::ldap_system_error(
        const char* what_arg, unsigned long ldapSystemErrorCode)
        : tt4g::ldap::ldap_error(
                std::string(what_arg).append(": ").append(::ldap_err2stringA(ldapSystemErrorCode)))
{

}

ldap_system_error::~ldap_system_error()
{

}

} // namespace ldap
} // namespace tt4g
