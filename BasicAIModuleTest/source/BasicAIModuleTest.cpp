#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BasicAIModule.h"

#include <iostream>
#include <string>

using std::string;
using std::cin;

namespace BWAPI { Game* Broodwar; }


int main(int argc, char** argv) {

	int ret;
	string keepAlive;

	// The following line must be executed to initialize Google Mock
	// (and Google Test) before running the tests.
	testing::InitGoogleMock(&argc, argv);
	ret = RUN_ALL_TESTS();

	cin >> keepAlive;
	exit(ret);
}