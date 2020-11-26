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

Twitter::Hashtag* hashtag_hashtag_a = nullptr;
Twitter::Hashtag* hashtag_hashtag_b = nullptr;

TEST_CASE("Check if unexistent hashtags exists returns false"){
  CHECK(!(Twitter::Hashtag::exists(0)));
}

TEST_CASE("Check if create creates a hashtag"){
  hashtag_hashtag_a = Twitter::Hashtag::create("test");
  CHECK(Twitter::Hashtag::exists(0));
  delete hashtag_hashtag_a;
}

TEST_CASE("Check if Hashtag::get(int) returns a hashtag with expected name"){
  hashtag_hashtag_a = Twitter::Hashtag::get(0);
  CHECK(hashtag_hashtag_a->get_name() == "test");
  delete hashtag_hashtag_a;
}

TEST_CASE("Check if Hashtag::get(std::string) returns a hashtag with expected id"){
  hashtag_hashtag_a = Twitter::Hashtag::get("test");
  CHECK(hashtag_hashtag_a->get_id() == 0);
  delete hashtag_hashtag_a;
}

TEST_CASE("Check if the use_count is equal to 1 on a newly created hashtag"){
	hashtag_hashtag_a = Twitter::Hashtag::get(0);

	CHECK(hashtag_hashtag_a->get_use_count() == 1);

	delete hashtag_hashtag_a;
}

TEST_CASE("Check if increment_use_count increments the use_count"){
  hashtag_hashtag_a = Twitter::Hashtag::get(0);
  hashtag_hashtag_a->increment_use_count();

  CHECK(hashtag_hashtag_a->get_use_count() == 2);

  delete hashtag_hashtag_a;
}

TEST_CASE("Check if Hashtag::get(std::vector<int>) returns a hashtags with expected ids"){
  hashtag_hashtag_b = Twitter::Hashtag::create("test2");
  std::vector<int> ids;
  ids.push_back(0);
  ids.push_back(1);
  std::vector<Twitter::Hashtag*> hashtags;
  hashtags = Twitter::Hashtag::get(ids);

  CHECK(hashtags.at(0)->get_name() == "test");
  CHECK(hashtags.at(1)->get_name() == "test2");

  delete hashtag_hashtag_b;

  for (std::vector<Twitter::Hashtag*>::iterator it = hashtags.begin(); it != hashtags.end(); ++it) {
    delete *it;
  }
}

TEST_CASE("Twitter::Hashtag::get_available_id() returns an unexistent id"){
  CHECK(Twitter::Hashtag::exists(Twitter::Hashtag::get_available_id()) == false);
}
