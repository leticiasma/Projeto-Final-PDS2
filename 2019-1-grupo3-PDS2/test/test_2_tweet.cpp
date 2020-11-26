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

Twitter::User* tweet_user_a = nullptr;
Twitter::User* tweet_user_b = nullptr;

Twitter::Tweet* tweet_tweet_a = nullptr;
Twitter::Tweet* tweet_tweet_b = nullptr;
Twitter::Tweet* tweet_tweet_c = nullptr;

Twitter::Retweet* tweet_retweet_a = nullptr;
Twitter::Retweet* tweet_retweet_b = nullptr;

TEST_CASE("Check if unexistent tweet exists returns false"){
  SUBCASE("Unexistent valid id"){
    CHECK(Twitter::Tweet::exists(0) == false);
  }
  SUBCASE("Unexistent valid large id"){
    CHECK(Twitter::Tweet::exists(999) == false);
  }
  SUBCASE("Unexistent invalid id"){
    CHECK(Twitter::Tweet::exists(-1) == false);
  }
}

TEST_CASE("Twitter::Tweet::get_available_id() returns an unexistent id"){
  CHECK(Twitter::Tweet::exists(Twitter::Tweet::get_available_id()) == false);
}

TEST_CASE("Check if existent tweet exists returns true"){
  tweet_user_a = Twitter::User::get(0);
  std::vector<std::string> hashtags;
  hashtags.push_back("TEST");
  hashtags.push_back("PDS2");
  tweet_tweet_a = Twitter::Tweet::post_tweet(tweet_user_a, "Hello World", hashtags);
  CHECK(Twitter::Tweet::exists(tweet_tweet_a->get_id()) == true);
  delete tweet_user_a;
  delete tweet_tweet_a;
}

TEST_CASE("Twitter::Tweet::get(id) returns the tweet with the desired id"){
  tweet_tweet_a = Twitter::Tweet::get(0);
  CHECK(tweet_tweet_a->get_id() == 0);
  delete tweet_tweet_a;
}

TEST_CASE("Check tweet creation properties"){
  tweet_tweet_a = Twitter::Tweet::get(0);
  CHECK(tweet_tweet_a->get_content() == "Hello World");
  CHECK(tweet_tweet_a->get_user_likes().size() == 0);
  CHECK(tweet_tweet_a->get_hashtags().size() == 2);
  CHECK(tweet_tweet_a->get_replies().size() == 0);
  CHECK(tweet_tweet_a->get_like_count() == 0);
  delete tweet_tweet_a;
}

TEST_CASE("Check delete tweet without any retweets"){
  tweet_user_a = Twitter::User::get(0);
  std::vector<std::string> hashtags;
  hashtags.push_back("TEST");
  hashtags.push_back("PDS2");
  tweet_tweet_b = Twitter::Tweet::post_tweet(tweet_user_a, "Hello world", hashtags);
  CHECK(Twitter::Tweet::exists(tweet_tweet_b->get_id()));
  tweet_tweet_b->delete_tweet(tweet_user_a);
  CHECK(!Twitter::Tweet::exists(tweet_tweet_b->get_id()));
  delete tweet_user_a;
  delete tweet_tweet_b;
}

TEST_CASE("Check if delete tweet removes the tweet from the user's tweets vector"){
  tweet_user_a = Twitter::User::get(0);
  std::vector<std::string> hashtags;
  int tweets_size;

  hashtags.push_back("TEST");
  hashtags.push_back("PDS2");
  tweet_tweet_b = Twitter::Tweet::post_tweet(tweet_user_a, "Hello world", hashtags);
  tweets_size = tweet_user_a->get_tweets().size();
  tweet_tweet_b->delete_tweet(tweet_user_a);
  CHECK(tweets_size-1 == tweet_user_a->get_tweets().size());
  delete tweet_user_a;
  delete tweet_tweet_b;
}

TEST_CASE("Check delete tweet with retweets"){
  tweet_user_a = Twitter::User::get(0);
  std::vector<std::string> hashtags;
  hashtags.push_back("TEST");
  hashtags.push_back("PDS2");
  tweet_tweet_b = Twitter::Tweet::post_tweet(tweet_user_a, "Hello world", hashtags);
  tweet_retweet_a = Twitter::Retweet::post_tweet(tweet_user_a, tweet_tweet_b);
  tweet_retweet_b = Twitter::Retweet::post_tweet(tweet_user_a, tweet_tweet_b);
  CHECK(Twitter::Tweet::exists(tweet_tweet_b->get_id()));
  tweet_tweet_b->delete_tweet(tweet_user_a);
  CHECK(!Twitter::Tweet::exists(tweet_tweet_b->get_id()));
  tweet_retweet_a->delete_tweet(tweet_user_a);
  tweet_retweet_b->delete_tweet(tweet_user_a);
  delete tweet_user_a;
  delete tweet_tweet_b;
  delete tweet_retweet_a;
  delete tweet_retweet_b;
}

TEST_CASE("Check if retweets from a deleted tweet become tweets"){
  tweet_user_a = Twitter::User::get(0);
  std::vector<std::string> hashtags;
  hashtags.push_back("TEST");
  hashtags.push_back("PDS2");
  tweet_tweet_b = Twitter::Tweet::post_tweet(tweet_user_a, "Hello world", hashtags);
  tweet_retweet_a = Twitter::Retweet::post_tweet(tweet_user_a, tweet_tweet_b);
  tweet_tweet_c = Twitter::Tweet::get(tweet_retweet_a->get_id());
  CHECK(tweet_tweet_c->is_retweet());
  tweet_tweet_b->delete_tweet(tweet_user_a);
  delete tweet_tweet_c;
  tweet_tweet_c = Twitter::Tweet::get(tweet_retweet_a->get_id());
  CHECK(!tweet_tweet_c->is_retweet());
  tweet_tweet_c->delete_tweet(tweet_user_a);
  delete tweet_user_a;
  delete tweet_tweet_b;
  delete tweet_tweet_c;
  delete tweet_retweet_a;
}
