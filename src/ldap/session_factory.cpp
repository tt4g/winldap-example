#include "ldap/session_factory.hpp"

#include "ldap/ldap_error_code.hpp"
#include "ldap/ldap_session_impl.hpp"

#include <windows.h>
#include <winldap.h>

#include <stdexcept>

namespace tt4g
{
namespace ldap
{

namespace
{

ldap_result_info createinvalidArgumentInfo(const char* message)
{
    return ldap_result_info {
            make_ldap_error_code(ldap_error_code::InvalidArgument), message, 0UL };
}

ldap_result_info createLdapApiErrorInfo(const char* message)
{
    return ldap_result_info {
            make_ldap_error_code(ldap_error_code::LdapApiError), message, ::LdapGetLastError() };
}

/**
 * @brief tt4g::ldap::protocol_version to option value.
 *
 * @param protocolVersion tt4g::ldap::protocol_version
 * @return 0 if invalid LDAP protocol version, otherwise non 0.
 */
unsigned long resolveLdapVersion(tt4g::ldap::protocol_version protocolVersion)
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

session_factory::session_factory(
        std::string host,
        unsigned short port)
{
    if (host.empty()) {
        throw std::invalid_argument("[session_factory] host is empty");
    }

    if (port == 0) {
        throw std::invalid_argument("[session_factory] port is 0");
    }
}

ldap_result<std::unique_ptr<ldap_session>> session_factory::create(
        std::string_view user,
        std::string_view password,
        std::string_view domain,
        protocol_version protocolVersion)
{
    if (user.empty()) {
        return boost::outcome_v2::failure(
                createinvalidArgumentInfo("[session_factory] port is 0"));
    }

    if (password.empty()) {
        return boost::outcome_v2::failure(
                createinvalidArgumentInfo("[session_factory] port is 0"));
    }

    if (domain.empty()) {
        return boost::outcome_v2::failure(
                createinvalidArgumentInfo("[session_factory] port is 0"));
    }

    const unsigned long ldapVersion = resolveLdapVersion(protocolVersion);
    if (ldapVersion == 0) {
        return boost::outcome_v2::failure(
                createinvalidArgumentInfo("[session_factory] protocolVersion is invalid"));
    }

    auto sessionImpl =
            std::make_unique<tt4g::ldap::ldap_session_impl>(
                    ::ldap_initA(
                            const_cast<char *>(this->host_.c_str()), this->port_));

    if (sessionImpl->getSession() == nullptr) {
        return boost::outcome_v2::failure(
                createLdapApiErrorInfo("[session_factory] ldap_init failed"));
    }

    if (::ldap_set_option(sessionImpl->getSession(), LDAP_OPT_PROTOCOL_VERSION, &ldapVersion)) {
        return boost::outcome_v2::failure(
                createLdapApiErrorInfo(
                        "[session_factory] ldap_set_option LDAP_OPT_PROTOCOL_VERSION failed"));
    }

    if (::ldap_set_option(sessionImpl->getSession(), LDAP_OPT_AUTO_RECONNECT, LDAP_OPT_ON)) {
        return boost::outcome_v2::failure(
                createLdapApiErrorInfo(
                        "[session_factory] ldap_set_option LDAP_OPT_AUTO_RECONNECT failed"));
    }

    LDAP_TIMEVAL connectionTimeout;
    connectionTimeout.tv_sec = 3;
    connectionTimeout.tv_usec = 0;
    if (::ldap_connect(sessionImpl->getSession(), &connectionTimeout)) {
        return boost::outcome_v2::failure(
                createLdapApiErrorInfo(
                        "[session_factory] ldap_connect failed"));
    }

    SEC_WINNT_AUTH_IDENTITY_A identity;
    identity.User = reinterpret_cast<unsigned char *>(const_cast<char *>(user.data()));
    identity.UserLength = static_cast<unsigned long>(user.size());
    identity.Domain = reinterpret_cast<unsigned char *>(const_cast<char *>(domain.data()));
    identity.DomainLength = static_cast<unsigned long>(domain.size());
    identity.Password = reinterpret_cast<unsigned char *>(const_cast<char *>(password.data()));
    identity.PasswordLength = static_cast<unsigned long>(password.size());
    identity.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;

    // if (ldap_bind_sA(sessionImpl->getSession(), nullptr,
    //         reinterpret_cast<char *>(&identity), LDAP_AUTH_SIMPLE)) {

    //     return boost::outcome_v2::failure(
    //             createLdapApiErrorInfo(
    //                     "[session_factory] ldap_bind_s failed"));
    // }

    // Can use LDAP_AUTH_SIMPLE, LDAP_AUTH_DIGEST, LDAP_AUTH_DPA, etc.
    if (ldap_bind_sA(sessionImpl->getSession(), nullptr,
            reinterpret_cast<char *>(&identity), LDAP_AUTH_DIGEST)) {

        return boost::outcome_v2::failure(
                createLdapApiErrorInfo(
                        "[session_factory] ldap_bind_s failed"));
    }

    auto session = std::make_unique<tt4g::ldap::ldap_session>(std::move(sessionImpl));

    return boost::outcome_v2::success(std::move(session));
}

} // namespace ldap
} // namespace tt4g
