#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>
#include <string>

#include "../lib/doctest/doctest.h"
#include "../lib/user.hpp"
#include "../lib/hashtag.hpp"
#include "../lib/tweet.hpp"
#include "../lib/interface.hpp"

// Exceptions
#include "../lib/exceptions/unexistent_user_exception.hpp"
#include "../lib/exceptions/invalid_action_exception.hpp"
#include "../lib/exceptions/forbidden_action_exception.hpp"
#include "../lib/exceptions/resource_not_found_exception.hpp"

Twitter::User* user_user_a = nullptr;
Twitter::User* user_user_b = nullptr;

TEST_CASE("Check if unexistent users exists returns false"){
  SUBCASE("Unexistent valid id"){
    CHECK(Twitter::User::exists(0) == false);
  }
  SUBCASE("Unexistent valid large id"){
    CHECK(Twitter::User::exists(999) == false);
  }
  SUBCASE("Unexistent invalid id"){
    CHECK(Twitter::User::exists(-1) == false);
  }
}

TEST_CASE("Twitter::User::get_available_id() returns an unexistent id"){
  CHECK(Twitter::User::exists(Twitter::User::get_available_id()) == false);
}

TEST_CASE("Check if existent users exists returns true"){
  user_user_a = Twitter::User::create("TEST_USER_A", "123456");
  CHECK(Twitter::User::exists(user_user_a->get_id()) == true);
  delete user_user_a;
}

TEST_CASE("Create user with an existent user name throws InvalidActionException"){
  CHECK_THROWS_AS(Twitter::User::create("TEST_USER_A", "123456"), InvalidActionException);
}

TEST_CASE("Twitter::User::get(id) returns the user with the desired id"){
  user_user_a = Twitter::User::get(0);
  CHECK(user_user_a->get_id() == 0);
  delete user_user_a;
}

TEST_CASE("Check user creation get_username"){
  user_user_a = Twitter::User::get(0);
  CHECK(user_user_a->get_username() == "TEST_USER_A");
  delete user_user_a;
}

TEST_CASE("Check user creation get_username"){
  user_user_a = Twitter::User::get(0);
  CHECK(user_user_a->get_followers().size() == 0);
  delete user_user_a;
}

TEST_CASE("Check user creation get_username"){
  user_user_a = Twitter::User::get(0);
  CHECK(user_user_a->get_followees().size() == 0);
  delete user_user_a;
}

TEST_CASE("Check user creation get_username"){
  user_user_a = Twitter::User::get(0);
  CHECK(user_user_a->get_tweets().size() == 0);
  delete user_user_a;
}

TEST_CASE("Get unexistent users throws UnexistentUserException"){
  SUBCASE("Unexistent valid id"){
    CHECK_THROWS_AS(Twitter::User::get(1), UnexistentUserException);
  }
  SUBCASE("Unexistent valid large id"){
    CHECK_THROWS_AS(Twitter::User::get(999), UnexistentUserException);
  }
  SUBCASE("Unexistent invalid id"){
    CHECK_THROWS_AS(Twitter::User::get(1), UnexistentUserException);
  }
}

TEST_CASE("User A follow user B"){
  user_user_a = Twitter::User::get(0);
  user_user_b = Twitter::User::create("TEST_USER_B", "123456");
  user_user_a->follow(user_user_b);
  CHECK(user_user_a->get_followees().size() == 1);
  CHECK(user_user_b->get_followers().size() == 1);
  delete user_user_b;
  delete user_user_a;
}

TEST_CASE("User A tries to follow user B more than once"){
  user_user_a = Twitter::User::get(0);
  user_user_b = Twitter::User::get(1);
  CHECK_THROWS_AS(user_user_a->follow(user_user_b), InvalidActionException);
  delete user_user_b;
  delete user_user_a;
}

TEST_CASE("User A unfollow user B"){
  user_user_a = Twitter::User::get(0);
  user_user_b = Twitter::User::get(1);
  user_user_a->unfollow(user_user_b);
  CHECK(user_user_a->get_followees().size() == 0);
  CHECK(user_user_b->get_followers().size() == 0);
  delete user_user_b;
  delete user_user_a;
}

TEST_CASE("User A tries to unfollow user B more than once"){
  user_user_a = Twitter::User::get(0);
  user_user_b = Twitter::User::get(1);
  CHECK_THROWS_AS(user_user_a->unfollow(user_user_b), InvalidActionException);
  delete user_user_b;
  delete user_user_a;
}

TEST_CASE("User A tries to follow himself"){
  user_user_a = Twitter::User::get(0);
  CHECK_THROWS_AS(user_user_a->follow(user_user_a), InvalidActionException);
  delete user_user_a;
}

TEST_CASE("Twitter::User::login(login, password) returns nullptr when the user does not exists"){
  user_user_a = Twitter::User::login("UNEXISTENT_USER", "123456");
  CHECK(user_user_a == nullptr);
}

TEST_CASE("Twitter::User::login(login, password) returns nullptr when the password is incorrect"){
  user_user_a = Twitter::User::login("TEST_USER_A", "654321");
  CHECK(user_user_a == nullptr);
}

TEST_CASE("Twitter::User::login(login, password) returns the desired user when login and password are correct"){
  user_user_a = Twitter::User::login("TEST_USER_A", "123456");
  CHECK(user_user_a->get_id() == 0);
  CHECK(user_user_a->get_username() == "TEST_USER_A");
  delete user_user_a;
}

TEST_CASE("Passing std::vector<int> to Twitter::User::get returns std::vector<Twitter::User*> with the desired users"){
  std::vector<int> ids;
  std::vector<Twitter::User*> users;
  ids.push_back(1);
  ids.push_back(0);
  users = Twitter::User::get(ids);
  CHECK(users.at(0)->get_id() == 1);
  CHECK(users.at(1)->get_id() == 0);
  for (std::vector<Twitter::User*>::iterator it = users.begin(); it != users.end(); ++it) {
    delete *it;
  }
}

TEST_CASE("Searching users returns all possible matches"){
  SUBCASE("Ambiguous search"){
    std::vector<Twitter::User*> users;
    users = Twitter::User::search("TEST");
    CHECK(users.size() == 2);
    for (std::vector<Twitter::User*>::iterator it = users.begin(); it != users.end(); ++it) {
      delete *it;
    }
  }
  SUBCASE("Unique search"){
    std::vector<Twitter::User*> users;
    users = Twitter::User::search("TEST_USER_A");
    CHECK(users.size() == 1);
    for (std::vector<Twitter::User*>::iterator it = users.begin(); it != users.end(); ++it) {
      delete *it;
    }
  }
}

TEST_CASE("Searching users returns empty vector when no matches are found"){
  std::vector<Twitter::User*> users;
  users = Twitter::User::search("NONE");
  CHECK(users.size() == 0);
}
