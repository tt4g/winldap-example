#include "ldap/ldap_session.hpp"

#include "ldap/ldap_error_code.hpp"
#include "ldap/ldap_session_impl.hpp"

#include <windows.h>
#include <winldap.h>

#include <system_error>
#include <utility>

namespace tt4g
{
namespace ldap
{

namespace
{

tt4g::ldap::ldap_result<ldap_session> system_error_result()
{
    std::error_code ec(static_cast<int>(ldap_error_code::LdapApiError), ldap_error_category());

    tt4g::ldap::ldap_result_info ldapResultInfo =
            { std::move(ec), ::LdapGetLastError() };

    return boost::outcome_v2::failure(std::move(ldapResultInfo));
}

/**
 * @brief tt4g::ldap::protocol_version to option value.
 *
 * @param protocolVersion tt4g::ldap::protocol_version
 * @return int 0 if invalid LDAP protocol version, otherwise non 0.
 */
int resolveLdapVersion(tt4g::ldap::protocol_version protocolVersion)
{
    switch (protocolVersion) {
        case tt4g::ldap::protocol_version::Version2:
            return LDAP_VERSION2;
        case tt4g::ldap::protocol_version::Version3:
            return LDAP_VERSION3;
    }

    return 0;
}

} // namespace

ldap_session::ldap_session(std::unique_ptr<ldap_session_impl> impl)
        : impl_(std::move(impl))
{

}

const ldap_session_impl& ldap_session::getImpl() const
{
    return *(this->impl_);
}

tt4g::ldap::ldap_result<tt4g::ldap::ldap_session> ldap_session::connect(
        std::string host, tt4g::ldap::protocol_version protocolVersion)
{
    auto sessionImpl = std::make_unique<tt4g::ldap::ldap_session_impl>(
            ::ldap_initA(const_cast<char *>(host.c_str()), 389));

    if (::ldap_set_option(sessionImpl->getSession(), LDAP_OPT_AUTO_RECONNECT, LDAP_OPT_ON)) {
        return system_error_result();
    }

    if (const int optProtocolVersion = resolveLdapVersion(protocolVersion)) {
        if (::ldap_set_option(sessionImpl->getSession(),
                LDAP_OPT_PROTOCOL_VERSION, &optProtocolVersion)) {
            return system_error_result();
        }
    }

    // TODO: for more ldap_set_option ...

    LDAP_TIMEVAL connectTimeout;
    connectTimeout.tv_sec = 3;
    connectTimeout.tv_usec = 0;
    if (::ldap_connect(sessionImpl->getSession(), &connectTimeout)) {
        return system_error_result();
    }

    // TODO: connection parameter.
    SEC_WINNT_AUTH_IDENTITY_A identity;
    const std::string user = "bar";
    const std::string password = "baz";
    const std::string domain = "localhost";
    // Windows: char is unsinged char.
    identity.User =
            reinterpret_cast<unsigned char *>(const_cast<char *>(user.c_str()));
    identity.UserLength = static_cast<unsigned long>(user.size());
    identity.Domain =
            reinterpret_cast<unsigned char *>(const_cast<char *>(domain.c_str()));
    identity.DomainLength = static_cast<unsigned long>(domain.size());
    identity.Password =
            reinterpret_cast<unsigned char *>(const_cast<char *>(password.c_str()));
    identity.PasswordLength = static_cast<unsigned long>(password.size());
    identity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

    std::string dn = "dc=localhost";
    if (::ldap_bind_sA(sessionImpl->getSession(), const_cast<char *>(dn.c_str()),
            reinterpret_cast<char *>(&identity), LDAP_AUTH_DIGEST)) {
        return system_error_result();
    }

    return boost::outcome_v2::success(
            tt4g::ldap::ldap_session(std::move(sessionImpl)));
}

} // namespace ldap
} // namespace tt4g
