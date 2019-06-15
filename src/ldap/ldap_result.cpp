#include "ldap/ldap_result.hpp"

#include "ldap/ldap_error.hpp"
#include "ldap/ldap_system_error.hpp"

#include <windows.h>
#include <winldap.h>

namespace tt4g
{
namespace ldap
{

std::string makeErrorMessage(const tt4g::ldap::ldap_result_info &ldapResultInfo)
{
    std::string message = ldapResultInfo.message;

    message.append(": ").append(ldapResultInfo.ec.message());

    return message;
}

void outcome_throw_as_system_error_with_payload(
        tt4g::ldap::ldap_result_info ldapResultInfo)
{
    boost::outcome_v2::try_throw_std_exception_from_error(ldapResultInfo.ec);

    if (ldapResultInfo.ldap_api_error_code == 0) {
        // LDAP API no error.
        throw tt4g::ldap::ldap_error(makeErrorMessage(ldapResultInfo));
    } else {
        // LDAP API error.
        throw tt4g::ldap::ldap_system_error(
                makeErrorMessage(ldapResultInfo),
                ldapResultInfo.ldap_api_error_code);
    }
}

} // namespace ldap
} // namespace tt4g
