#include "ldap/ldap_result.hpp"

#include "ldap/ldap_error.hpp"
#include "ldap/ldap_system_error.hpp"

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

std::string make_error_messae(const tt4g::ldap::ldap_result_info &ldapResultInfo)
{
    if (ldapResultInfo.ldap_system_error_code == 0) {
        return ldapResultInfo.ec.message();
    }

    // LDAP error code to message.
    const char* systemMessage = ::ldap_err2stringA(ldapResultInfo.ldap_system_error_code);
    if (systemMessage == nullptr) {
        // NULL when invalid error code.
        return ldapResultInfo.ec.message();
    }

    std::string message = ldapResultInfo.ec.message();
    message.append(": ").append(systemMessage);

    return message;
}

void outcome_throw_as_system_error_with_payload(
        tt4g::ldap::ldap_result_info ldapResultInfo)
{
    boost::outcome_v2::try_throw_std_exception_from_error(ldapResultInfo.ec);

    if (ldapResultInfo.ldap_system_error_code == 0) {
        // LDAP API no error.
        throw tt4g::ldap::ldap_error(ldapResultInfo.ec.message());
    } else {
        // LDAP API error.
        throw tt4g::ldap::ldap_system_error(
                ldapResultInfo.ec.message(),
                ldapResultInfo.ldap_system_error_code);
    }
}

} // namespace ldap
} // namespace tt4g
