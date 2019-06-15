#ifndef TT4G_LDAP_SESSION_FACTORY_HPP_
#define TT4G_LDAP_SESSION_FACTORY_HPP_

#include "ldap/ldap_result.hpp"
#include "ldap/ldap_session.hpp"
#include "ldap/protocol_version.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace tt4g
{
namespace ldap
{

class session_factory
{
public:

    /**
     * @brief create object.
     *
     * @param host network host or Active Directory service domain name.
     * @param port listen port.
     *             `389` (domain controller) or 3268 (global catalog).
     */
    explicit session_factory(
            std::string host,
            unsigned short port);

    /**
     * @brief Create ldap_session.
     *
     * Connect to the directory service and try LDAP bind.
     *
     * @warning
     *   SSL session not supported.
     *
     * @param user LDAP user (null terminated).
     * @param password LDAP password (null terminated).
     * @param domain Target directory service domain (null terminated).
     * @param protocolVersion LDAP protocol version.
     * @return ldap_session if LDAP session binding succeed,
     *                     otherwise error infomation.
     */
    ldap_result<std::unique_ptr<ldap_session>> create(
            std::string_view user,
            std::string_view password,
            std::string_view domain,
            protocol_version protocolVersion);

private:

    std::string host_;

    unsigned short port_;

    std::string domain_;

};

} // namespace ldap
} // namespace tt4g


#endif // TT4G_LDAP_SESSION_FACTORY_HPP_
