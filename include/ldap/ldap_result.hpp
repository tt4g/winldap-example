#ifndef TT4G_LDAP_LDAP_RESULT_HPP_
#define TT4G_LDAP_LDAP_RESULT_HPP_

#include <boost/outcome.hpp>

#include <string>
#include <system_error>

namespace tt4g
{
namespace ldap
{

struct ldap_result_info
{
    /**
     * @brief std::error_code
     *
     * The `value` of std::error_code is tt4g::ldap::ldap_error_code,
     * and `category` is tt4g::ldap::ldap_error_category().
     */
    std::error_code ec;

    /**
     * @brief Error reason message.
     */
    std::string message;

    /**
     * @brief LDAP API error code.
     *
     * If LDAP API failed, set non 0, otherwise 0 (LDAP_SUCCESS).
     */
    unsigned long ldap_api_error_code;

};

/**
 * @brief Make error message.
 *
 * @param ldapResultInfo ldap_result_info.
 * @return std::string message.
 */
std::string makeErrorMessage(const tt4g::ldap::ldap_result_info &ldapResultInfo);

// Boost.Outcome: convertion function for failure_info of boost::outcome_v2::result to std::error_code
inline const std::error_code& make_error_code(const tt4g::ldap::ldap_result_info &ldapResultInfo)
{
    return ldapResultInfo.ec;
}

// Boost.Outcome: throw custom error when invalid value access.
void outcome_throw_as_system_error_with_payload(
        tt4g::ldap::ldap_result_info ldapResultInfo);

// Boost.Outcome custom payloads.
template<typename T>
using ldap_result = boost::outcome_v2::result<T, tt4g::ldap::ldap_result_info>;

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_RESULT_HPP_
