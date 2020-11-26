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

Twitter::Reply* reply_reply_a = nullptr;
Twitter::User* reply_user_a = nullptr;
Twitter::Tweet* reply_tweet_a = nullptr;

TEST_CASE("Check if Twitter::Reply::get_like_count return 0 a newly create reply"){
  reply_user_a = Twitter::User::get(0);
  reply_tweet_a = Twitter::Tweet::get(0);
  reply_reply_a = Twitter::Reply::post_reply(reply_user_a, reply_tweet_a, "my reply");

  CHECK(reply_reply_a->get_like_count() == 0);

  delete reply_user_a;
  delete reply_reply_a;
  delete reply_tweet_a;
}

TEST_CASE("Checks id the user who liked the reply is present on the user_likes array"){
  reply_reply_a = Twitter::Reply::get(0);
  reply_reply_a->like(0);

  CHECK(reply_reply_a->get_user_likes().at(0) == 0);

  delete reply_reply_a;
}

TEST_CASE("Check if Twitter::Reply::get_like_count return 1 after an user likes new reply"){
  reply_reply_a = Twitter::Reply::get(0);

  CHECK(reply_reply_a->get_like_count() == 1);

   delete reply_reply_a;
}

TEST_CASE("Check delete reply"){
  reply_reply_a = Twitter::Reply::get(0);
  reply_user_a = Twitter::User::get(0);
  CHECK(Twitter::Reply::exists(reply_reply_a->get_id()));
  reply_reply_a->delete_reply(reply_user_a);
  CHECK(!Twitter::Reply::exists(reply_reply_a->get_id()));
  delete reply_reply_a;
  delete reply_user_a;
}

TEST_CASE("Twitter::Reply::get_available_id() returns an unexistent id"){
  CHECK(Twitter::Reply::exists(Twitter::Reply::get_available_id()) == false);
}
