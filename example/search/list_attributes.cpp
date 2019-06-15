#include "search/list_attributes.hpp"

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

struct ber_holder__
{
    ber_holder__(BerElement *ber) : ber(ber) {}

    ~ber_holder__()
    {
        if (this->ber) {
            ::ber_free(this->ber, 0);
        }
    }

    BerElement *ber;
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


int listAttributes(std::shared_ptr<ldap_session> session)
{
    // LDAP search base DN.
    const std::string baseDn = "dc=localhost,dc=localdomain";
    // LDAP filter string.
    // NOTE: Don't forget LDAP escape.
    const std::string filter = "(&(objectClass=user)(description=\\\"\\\"))";

    // LDAP search fetch target attribute names (null terminated array).
    // i.e.
    //     const std::array<char *, 2> attrArray = {
    //         "sn",
    //         nullptr
    //     };
    const std::array<char *, 1> attrArray = {
        nullptr
    };

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
                    const_cast<char **>(attrArray.data()),
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

    std::cout << "list all entry and attribute" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    // Don't call ldap_msgfree(LDAPMessage*) from ldap_first_entry() or
    // ldap_next_entry() because this memory part of original LDAPMessage* pointer.
    for (LDAPMessage *entry = ::ldap_first_entry(session->getImpl().getSession(), messageImpl.getMessage());
            entry != nullptr;
            entry = ldap_next_entry(session->getImpl().getSession(), entry)) {

        const ldap_memfree_caller__ dnHolder(::ldap_get_dnA(session->getImpl().getSession(), entry));
        if (dnHolder.value != nullptr) {
            std::cout << "DN: " << dnHolder.value << std::endl;
        } else {
            std::cout << "ldap_get_dn failed" << std::endl;
        }

        ber_holder__ berHolder(nullptr);
        char *attribute = nullptr;

        std::cout << "=========================================================" << std::endl;
        for (attribute = ldap_first_attributeA(session->getImpl().getSession(), entry, &(berHolder.ber));
                attribute != nullptr;
                attribute = ldap_next_attributeA(session->getImpl().getSession(), entry, berHolder.ber)) {

            const str_value_holder__ valueHolder(ldap_get_valuesA(session->getImpl().getSession(), entry, attribute));
            if (valueHolder.value != nullptr) {
                for (int i = 0; valueHolder.value[i] != nullptr; ++i) {
                    std::cout << attribute << ": " << valueHolder.value[i] << std::endl;
                }
            } else {
                std::cout << attribute << " is empty!" << std::endl;
            }

            ::ldap_memfreeA(attribute);
        }

        std::cout << "=========================================================" << std::endl;
    }

    std::cout << "---------------------------------------------------------" << std::endl;

    return 0;
}

} // namespace ldap
} // namespace tt4g
