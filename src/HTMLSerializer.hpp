#ifndef _HTML_SERIALIZER_HPP_
#define _HTML_SERIALIZER_HPP_

#include <iostream>

#include "utils/SerializerInterface.hpp"
#include "database/Database.hpp"
#include "database/DatabaseObjects.hpp"

namespace LogSupervisor::Serializer{
using Database = LogSupervisor::Database::Database;

//! This classes serializes as formatted HTML
class HTML : public Interface{
public:
    HTML(const Database& db);
    ostream& serialize(ostream& lhs) override;
    virtual ~HTML() {}

private:
    const Database& db;

    // various HTML styles
    static const string sUserBlock;
    static const string sUserName;
    static const string sTable;
    static const string sCounter;
    static const string sRowStyleOK[];
    static const string sRowStyleWarning[];
    static const string sRowStyleCritical[];

    ostream& userHeader(ostream& lhs, const User& user, bool login_succeeded);
    ostream& authRow(ostream& lhs, const shared_ptr<Authentication> auth, unsigned int* iRow, User* curUser);
    ostream& userFooter(ostream& lhs);
};

} // namespace

#endif // _HTML_SERIALIZER_HPP_