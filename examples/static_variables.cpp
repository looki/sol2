#include <sol.hpp>

#include <iostream>

struct test {
	static int muh_variable;
};
int test::muh_variable = 25;


int main() {
	std::cout << "=== static_variables example ===" << std::endl;

	sol::state lua;
	lua.open_libraries();
	lua.new_usertype<test>("test",
		"direct", sol::var(2),
		"global", sol::var(test::muh_variable),
		"ref_global", sol::var(std::ref(test::muh_variable))
	);

	int direct_value = lua["test"]["direct"];
	std::cout << "direct_value: " << direct_value << std::endl;
	// direct_value == 2

	int global = lua["test"]["global"];
	// global == 25
	int global2 = lua["test"]["ref_global"];
	// global2 == 25

	std::cout << "First round of values --" << std::endl;
	std::cout << global << std::endl;
	std::cout << global2 << std::endl;

	test::muh_variable = 542;

	global = lua["test"]["global"];
	// global == 25
	// global is its own memory: was passed by value

	global2 = lua["test"]["ref_global"];
	// global2 == 542
	// global2 was passed through std::ref
	// global2 holds a reference to muh_variable
	// if muh_variable goes out of scope or is deleted
	// problems could arise, so be careful!

	std::cout << "Second round of values --" << std::endl;
	std::cout << "global : " << global << std::endl;
	std::cout << "global2: " << global2 << std::endl;
	std::cout << std::endl;

	return 0;
}
