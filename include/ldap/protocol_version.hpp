#ifndef TT4G_LDAP_PROTOCOL_VERSION_HPP_
#define TT4G_LDAP_PROTOCOL_VERSION_HPP_

namespace tt4g
{
namespace ldap
{

/**
 * @brief LDAP protocol version.
 */
enum class protocol_version : int
{

    /**
     * @brief LDAP v2.
     */
    Version2 = 2,

    /**
     * @brief LDAP v3.
     */
    Version3 = 3

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_PROTOCOL_VERSION_HPP_
