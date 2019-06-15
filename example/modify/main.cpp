#include "modify/add_or_modify.hpp"

#include "ldap/ldap_result.hpp"
#include "ldap/ldap_session.hpp"
#include "ldap/session_factory.hpp"
#include "ldap/protocol_version.hpp"

#include <iostream>
#include <utility>

int main(int /*  argc */, char** /* argv */) {

    if (true) {
        std::cout << "[WARNING] This program change directory service entry.\n"
                  << "          Should you check source and confirm the danger\n"
                  << "          before modifying the source code."
                  << std::endl;

        return 0;
    }

    using namespace tt4g;

    ldap::session_factory sessionFactory("your domain", 389);

    ldap::ldap_result<std::unique_ptr<ldap::ldap_session>> createSessionResult =
            sessionFactory.create(
                    "user", "password", "localhost.localdomain",
                    ldap::protocol_version::Version3);

    if (!createSessionResult.has_value()) {
        const ldap::ldap_result_info& errorInfo = createSessionResult.error();

        std::cout << ldap::makeErrorMessage(errorInfo) << std::endl;

        return 1;
    }
    std::shared_ptr<ldap::ldap_session> session =
            std::move(createSessionResult).value();

    return tt4g::ldap::addOrModifyDescription(session);
}
