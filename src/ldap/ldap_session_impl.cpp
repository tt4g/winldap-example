#include "ldap/ldap_session_impl.hpp"

#include <utility>

namespace tt4g
{
namespace ldap
{

// NOTE: The lifetime of SEC_WINNT_AUTH_IDENTITY_A should be longer than LDAP*.
//
// See:  https://docs.microsoft.com/en-us/windows/desktop/api/rpcdce/ns-rpcdce-_sec_winnt_auth_identity_a
// > This structure must remain valid for the lifetime of the binding handle
// > unless pointed to from the RPC_HTTP_TRANSPORT_CREDENTIALS or
// > RPC_HTTP_TRANSPORT_CREDENTIALS_V2 structure.

ldap_session_impl::ldap_session_impl(
        LDAP* session,
        std::unique_ptr<SEC_WINNT_AUTH_IDENTITY_A> identity)
        : session_(session),
          identity_(std::move(identity))
{

}

ldap_session_impl::~ldap_session_impl()
{
    // ldap_init() returns nullptr if initialization failed.
    if (this->session_) {
        ::ldap_unbind_s(this->session_);
    }
}

LDAP* ldap_session_impl::getSession() const
{
    return this->session_;
}

} // namespace ldap
} // namespace tt4g
