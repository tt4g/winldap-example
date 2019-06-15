#ifndef TT4G_LDAP_LDAP_SESSION_HPP_
#define TT4G_LDAP_LDAP_SESSION_HPP_

#include <memory>

namespace tt4g
{
namespace ldap
{

class ldap_session_impl;

class ldap_session
{
public:

    explicit ldap_session(std::unique_ptr<ldap_session_impl> impl);

    ~ldap_session();

    ldap_session(const ldap_session&) = delete;

    ldap_session(ldap_session&&) = default;

    const ldap_session_impl& getImpl() const;

    ldap_session& operator=(const ldap_session&) = delete;

    ldap_session& operator=(ldap_session&&) = default;

private:

    std::unique_ptr<ldap_session_impl> impl_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_SESSION_HPP_
