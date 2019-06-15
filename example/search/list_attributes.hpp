#ifndef TT4G_LDAP_EXAMPLE_SEARCH_LIST_ATTRIBUTES_HPP_
#define TT4G_LDAP_EXAMPLE_SEARCH_LIST_ATTRIBUTES_HPP_

#include "ldap/ldap_session.hpp"

#include <memory>

namespace tt4g
{
namespace ldap
{

/**
 * @brief LDAP search and list attributes.
 *
 * @param session LDAP session.
 * @return exit code.
 */
int listAttributes(std::shared_ptr<ldap_session> session);

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_EXAMPLE_SEARCH_LIST_ATTRIBUTES_HPP_
