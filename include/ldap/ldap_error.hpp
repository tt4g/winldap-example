#ifndef TT4G_LDAP_LDAP_ERROR_HPP_
#define TT4G_LDAP_LDAP_ERROR_HPP_

#include <exception>
#include <string>

namespace tt4g
{
namespace ldap
{

class ldap_error : public std::exception
{
public:

    /**
     * @brief create object.
     *
     * @param what_arg Explanatory string.
     */
    explicit ldap_error(const std::string& what_arg);

    /**
     * @brief create object.
     *
     * @param what_arg Explanatory string. (null terminated).
     */
    explicit ldap_error(const char* what_arg);

    virtual ~ldap_error() noexcept override;

    virtual const char* what() const noexcept;

private:

    std::string what_;

};

} // namespace ldap
} // namespace tt4g

#endif // TT4G_LDAP_LDAP_ERROR_HPP_
