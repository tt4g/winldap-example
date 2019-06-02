#ifndef TT4G_LDAP_LDAP_ERROR_CODE_HPP_
#define TT4G_LDAP_LDAP_ERROR_CODE_HPP_

#include <string>
#include <system_error>

namespace tt4g
{
namespace ldap
{

/**
 * @brief LDAP error code.
 */
enum class ldap_error_code : int
{

    /**
     * @brief LDAP API error.
     */
    LdapApiError = 1

};

/**
 * @brief Error category of tt4g::ldap::ldap_error_code.
 */
class ldap_error_code_category : public std::error_category
{
public:

    const char* name() const noexcept override
    {
        return "ldap error";
    }

    std::string message(int ev) const override
    {
        switch (static_cast<ldap_error_code>(ev)) {
            case ldap_error_code::LdapApiError:
                return "LDAP api error";
        }

        return "unknown error";
    }

};

const std::error_category& ldap_error_category()
{
    static tt4g::ldap::ldap_error_code_category ldapErrorCategory;

    return ldapErrorCategory;
}

std::error_condition make_error_condition(tt4g::ldap::ldap_error_code ldapErrorCode)
{
    return std::error_condition(
            static_cast<int>(ldapErrorCode), tt4g::ldap::ldap_error_category());
}

} // namespace ldap
} // namespace tt4g

namespace std
{

    template<>
    struct is_error_condition_enum<tt4g::ldap::ldap_error_code> : true_type {};

} // namespace std

#endif // TT4G_LDAP_LDAP_ERROR_CODE_HPP_
