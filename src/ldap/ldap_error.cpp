#include "ldap/ldap_error.hpp"

namespace tt4g
{
namespace ldap
{

ldap_error::ldap_error(const std::string& what_arg)
        : std::exception(),
          what_(what_arg)
{

}

ldap_error::ldap_error(const char* what_arg)
        : std::exception(),
          what_(what_arg)
{

}

ldap_error::~ldap_error() noexcept
{

}

const char* ldap_error::what() const noexcept
{
    return this->what_.c_str();
}

} // namespace ldap
} // namespace tt4g
