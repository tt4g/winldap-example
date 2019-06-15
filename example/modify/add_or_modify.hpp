#ifndef TT4G_LDAP_EXAMPLE_SEARCH_ADD_OR_MODIFY_HPP_
#define TT4G_LDAP_EXAMPLE_SEARCH_ADD_OR_MODIFY_HPP_

#include "ldap/ldap_session.hpp"

#include <memory>

namespace tt4g
{
namespace ldap
{

/**
 * @brief LDAP modify "description" attribute.
 *
 * @param session LDAP session.
 * @return exit code.
 */
int addOrModifyDescription(std::shared_ptr<ldap_session> session);

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_EXAMPLE_SEARCH_ADD_OR_MODIFY_HPP_
