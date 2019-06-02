#ifndef TT4G_LDAP_LDAP_SYSTEM_ERROR_HPP_
#define TT4G_LDAP_LDAP_SYSTEM_ERROR_HPP_

#include "ldap/ldap_error.hpp"

#include <string>

namespace tt4g
{
namespace ldap
{

class ldap_system_error : public tt4g::ldap::ldap_error
{
public:

    /**
     * @brief create object.
     *
     * @param what_arg Explanatory string.
     * @param ldapSystemErrorCode Returns the LDAP error code from `LdapGetLastError()`.
     */
    explicit ldap_system_error(
            const std::string& what_arg, unsigned long ldapSystemErrorCode);

    /**
     * @brief create object.
     *
     * @param what_arg Explanatory string.
     * @param ldapSystemErrorCode Returns the LDAP error code from `LdapGetLastError()`.
     */
    explicit ldap_system_error(
            const char* what_arg, unsigned long ldapSystemErrorCode);

    virtual ~ldap_system_error() noexcept override;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_SYSTEM_ERROR_HPP_
