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
     * @param ldapApiErrorCode Returns the LDAP error code from `LdapGetLastError()`.
     */
    explicit ldap_system_error(
            const std::string& what_arg, unsigned long ldapApiErrorCode);

    /**
     * @brief create object.
     *
     * @param what_arg Explanatory string.
     * @param ldapApiErrorCode Returns the LDAP error code from `LdapGetLastError()`.
     */
    explicit ldap_system_error(
            const char* what_arg, unsigned long ldapApiErrorCode);

    virtual ~ldap_system_error() noexcept override;

    /**
     * @brief Get LDAP API error code.
     *
     * @return LDAP API error code.
     */
    unsigned long getLdapApiErrorCode() const;

private:

    unsigned long ldapApiErrorCode_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_SYSTEM_ERROR_HPP_
