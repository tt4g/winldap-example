#ifndef TT4G_LDAP_LDAP_MESSAGE_HPP_
#define TT4G_LDAP_LDAP_MESSAGE_HPP_

#include <memory>

namespace tt4g
{
namespace ldap
{

class ldap_message_impl;

class ldap_message
{
public:

    explicit ldap_message(std::unique_ptr<ldap_message_impl> impl);

    ldap_message(const ldap_message&) = delete;

    ldap_message(ldap_message&&) = default;

    const ldap_message_impl& getImpl() const;

    ldap_message& operator=(const ldap_message&) = delete;

    ldap_message& operator=(ldap_message&&) = default;

private:

    std::unique_ptr<ldap_message_impl> impl_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_MESSAGE_HPP_
