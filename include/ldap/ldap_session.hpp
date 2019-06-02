#ifndef TT4G_LDAP_LDAP_SESSION_HPP_
#define TT4G_LDAP_LDAP_SESSION_HPP_

#include "ldap/ldap_result.hpp"
#include "ldap/protocol_version.hpp"

#include <memory>
#include <string>

namespace tt4g
{
namespace ldap
{

class ldap_session_impl;

class ldap_session
{
public:

    ldap_session(const ldap_session&) = delete;

    ldap_session(ldap_session&&) = default;

    const ldap_session_impl& getImpl() const;

    static tt4g::ldap::ldap_result<tt4g::ldap::ldap_session> connect(
            std::string host, tt4g::ldap::protocol_version protocolVersion);

    ldap_session& operator=(const ldap_session&) = delete;

    ldap_session& operator=(ldap_session&&) = default;

private:

    explicit ldap_session(std::unique_ptr<ldap_session_impl> impl);

    std::unique_ptr<ldap_session_impl> impl_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_SESSION_HPP_
