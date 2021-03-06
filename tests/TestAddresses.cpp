#include "catch.hpp"

#include "../src/utils/AddressRange.hpp"
#include "../src/utils/Address.hpp"
using namespace std;
using namespace LogSupervisor;


TEST_CASE( "IPv4 single addresses" ){
    CHECK_THROWS( Address("10.11.12.13/8") );
    CHECK_THROWS( Address("This is not an address") );

    // Constructors
    const char c_str_addrOne[] = "192.168.0.1";
    const string str_addrTwo("192.168.0.2");
    Address addr(c_str_addrOne);
    Address addr2(str_addrTwo);
}




static void check_contains_ipv4(const AddressRange& range){
    CHECK( range.contains(Address("10.20.30.40")) == true );
    CHECK( range.contains(Address("1.2.3.4")) == false );
    CHECK( range.contains(Address("::1")) == false );
}

TEST_CASE( "IPv4 address ranges" ) {
    // Constructors
    const char c_str_range[] = "10.0.0.0/8";
    AddressRange range(c_str_range);

    const string str_range("10.0.0.0/8");
    AddressRange range2(str_range);

    // Contains
    check_contains_ipv4(range);
    check_contains_ipv4(range2);

    // to_string
    CHECK( range.to_string()->compare(c_str_range) == 0 );
    CHECK( range2.to_string()->compare(str_range) == 0 );
}



TEST_CASE( "IPv6 single addresses" ){
    CHECK_THROWS( Address("::1/8") );
    CHECK_THROWS( Address("This is not an address") );

    // Constructors
    const char c_str_addrOne[] = "2001:2222:2222:b10c::1";
    const string str_addrTwo("2001:2222:2222:b10c::99");
    Address addr(c_str_addrOne);
    Address addr2(str_addrTwo);
}


static void check_contains_ipv6(const AddressRange& range){
    CHECK( range.contains(Address("2001:2222:2222:b10c::1")) == true );
    CHECK( range.contains(Address("2001:2222:2222:b10c::99")) == true );
    CHECK( range.contains(Address("2001:2222:2222:b10c:11:12:13:14")) == true );
    CHECK( range.contains(Address("2001:2222:2222:b10c:11::ffff")) == true );
    CHECK( range.contains(Address("1.2.3.4")) == false );
    CHECK( range.contains(Address("2001:9999:9999:b10c:11::ffff")) == false );
}
TEST_CASE( "IPv6 address ranges" ) {
    // Constructors
    const char c_str_range[] = "2001:2222:2222:b10c::/64";
    AddressRange range(c_str_range);

    const string str_range("2001:2222:2222:b10c::/64");
    AddressRange range2(str_range);

    // Contains
    check_contains_ipv6(range);
    check_contains_ipv6(range2);

    // to_string
    CHECK( range.to_string()->compare(c_str_range) == 0 );
    CHECK( range2.to_string()->compare(str_range) == 0 );
}