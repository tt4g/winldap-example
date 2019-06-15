#include "modify/add_or_modify.hpp"

#include "ldap/ldap_session_impl.hpp"
#include "ldap/ldap_message_impl.hpp"

#include <windows.h>
#include <winldap.h>
#include <winber.h>

#include <array>
#include <iostream>
#include <string>

namespace tt4g
{
namespace ldap
{

namespace
{

struct str_value_holder__
{
    str_value_holder__(char **value) : value(value) {}

    ~str_value_holder__()
    {
        if (this->value) {
            ldap_value_freeA(this->value);
        }
    }

    char **value;
};

struct ldap_memfree_caller__
{
    ldap_memfree_caller__(char *value) : value(value) {}

    ~ldap_memfree_caller__()
    {
        if (this->value) {
            ::ldap_memfreeA(this->value);
        }
    }

    char *value;
};

} // namespace

int addOrModifyDescription(std::shared_ptr<ldap_session> session)
{
    static constexpr char Description[] = "description";

    // LDAP search base DN.
    const std::string baseDn = "dc=localhost,dc=localdomain";
    // LDAP filter string.
    // NOTE: Don't forget LDAP escape.
    const std::string filter = "(&(objectClass=user)(sAMAccountName=John Doe))";

    LDAP_TIMEVAL searchTimeout;
    searchTimeout.tv_sec = 3UL;
    searchTimeout.tv_usec = 0UL;

    LDAPMessage *message = nullptr;

    const unsigned long searchResult =
            ::ldap_search_ext_sA(
                    session->getImpl().getSession(),
                    const_cast<char *>(baseDn.c_str()),
                    LDAP_SCOPE_SUBTREE,
                    const_cast<char *>(filter.c_str()),
                    nullptr,
                    0UL,
                    nullptr,
                    nullptr,
                    &searchTimeout,
                    LDAP_NO_LIMIT,
                    &message);

    const ldap_message_impl messageImpl(session, message);

    if (searchResult != LDAP_SUCCESS) {
        std::cout << "ldap_search_ext_sA failed" << std::endl;
        std::wcout << ldap_err2string(searchResult) << std::endl;

        return 1;
    }

    // Don't call ldap_msgfree(LDAPMessage*) from ldap_first_entry()
    // because this memory part of original LDAPMessage* pointer.
    LDAPMessage *entry = ::ldap_first_entry(session->getImpl().getSession(), messageImpl.getMessage());

    if (entry == nullptr) {
        std::cout << "ldap_first_entry returns NULL pointer" << std::endl;

        return 1;
    }

    str_value_holder__ descriptionHolder(
                ::ldap_get_valuesA(session->getImpl().getSession(), entry,
                        const_cast<char *>(Description)));

    // "description" attribute already exists?
    const bool registered = descriptionHolder.value == nullptr ? false : true;

    std::cout << Description << " is "
            << (registered ? "already exists" : "does not exists")
            << std::endl;

    const std::string modifyValue = "Hello, Directory service!";

    // Array of value of "description" attribute (null terminated array).
    std::array<char *, 2> modValues = {
            const_cast<char *>(modifyValue.c_str()),
            nullptr
    };

    LDAPModA updatedMod;
    updatedMod.mod_op = (registered ? LDAP_MOD_REPLACE : LDAP_MOD_ADD);
    updatedMod.mod_type = const_cast<char *>(Description);
    updatedMod.mod_vals.modv_strvals = const_cast<char **>(modValues.data());

    // LDAPMod array (null terminated).
    std::array<LDAPModA *, 2> mods = {
        &updatedMod,
        nullptr
    };

    // ldap_get_dnA は失敗したら nullptr を返す。
    ldap_memfree_caller__ entryDnHolder(::ldap_get_dnA(session->getImpl().getSession(), entry));

    if (entryDnHolder.value == nullptr) {
        std::cout << "ldap_get_dnA failed" << std::endl;
        std::wcout << ldap_err2string(LdapGetLastError()) << std::endl;

        return 1;
    }

    std::cout << "modify target dn: " << entryDnHolder.value << std::endl;
    std::cout << "change attribute '" << Description << "' to \""
              << modifyValue << "\"" << std::endl;

    unsigned long updateResult =
            ::ldap_modify_ext_sA(
                    session->getImpl().getSession(), entryDnHolder.value,
                    const_cast<LDAPModA **>(mods.data()), nullptr, nullptr);

    if (updateResult != LDAP_SUCCESS) {
        std::cout << "ldap_modify_ext_sA failed" << std::endl;
        std::wcout << ldap_err2string(updateResult) << std::endl;

        return 1;
    }

    return 0;
}

} // namespace ldap
} // namespace tt4g
