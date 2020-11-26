#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../lib/hashtag.hpp"
#include "../lib/tweet.hpp"
#include "../lib/retweet.hpp"

// Exceptions
#include "../lib/exceptions/unexistent_reply_exception.hpp"
#include "../lib/exceptions/unexistent_user_exception.hpp"
#include "../lib/exceptions/unexistent_tweet_exception.hpp"
#include "../lib/exceptions/invalid_action_exception.hpp"
#include "../lib/exceptions/forbidden_action_exception.hpp"
#include "../lib/exceptions/resource_not_found_exception.hpp"

using Json = nlohmann::json;
using Hashtag = Twitter::Hashtag;
using Tweet = Twitter::Tweet;
using User = Twitter::User;
using Retweet = Twitter::Retweet;

Retweet::Retweet(int creator_id, Tweet* original_tweet): Tweet(creator_id, original_tweet->get_content(), original_tweet->get_hashtags()){
  this->original_id = original_tweet->get_id();
}

int Retweet::get_original_id(){
  return this->original_id;
}

void Retweet::set_original_id(int original_id){
  this->original_id = original_id;
}

bool Retweet::make_tweet(){
  Tweet* tweet = new Tweet();
  std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  Json tweets_json;

  if(!tweets_file.is_open()){
    throw ResourceNotFoundException("file:/"+Tweet::get_general_filename());
  }
  else if(tweets_file.peek() == std::fstream::traits_type::eof()){
    tweets_file.close();
		throw UnexistentTweetException(this->id);
  }
  tweets_file >> tweets_json;
  tweets_file.close();
  for (Json::iterator it = tweets_json.begin(); it != tweets_json.end(); ++it) {
    Json tweet = *it;
    if(tweet["id"] == this->id){
      *it = {
        {"id", this->id},
        {"is_retweet", false}
      };
    }
  }
  tweets_file.open(Tweet::get_general_filename(), std::fstream::out);
  tweets_file << tweets_json.dump(2);
  tweets_file.close();

  tweet->set_id(this->id);
  tweet->set_content(this->content);
  tweet->set_creator_id(this->creator_id);
  tweet->set_hashtags(this->hashtags);
  tweet->set_user_likes(this->user_likes);
  tweet->set_replies(this->replies);
  tweet->set_like_count();
  try{
    tweet->save();
  }
  catch(...){
    return false;
  }
  delete tweet;
  return true;
}

Retweet* Retweet::post_tweet(User* creator, Tweet* original_tweet){
  std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  std::vector<std::string> hashtags = original_tweet->get_hashtags();
  Json tweets_json;

  if(!User::exists(creator->get_id())){
		if(tweets_file.is_open()) tweets_file.close();
		return nullptr;
  }
  if(!Tweet::exists(original_tweet->get_id())){
		if(tweets_file.is_open()) tweets_file.close();
		return nullptr;
  }
  Retweet* retweet = new Retweet(creator->get_id(), original_tweet);
	if(retweet->save()){
		if(!tweets_file.is_open()){
      tweets_json = Json::array();
      std::cout << "Creating new tweets file" << std::endl;
    }
    else if(tweets_file.peek() == std::fstream::traits_type::eof()){
      tweets_json = Json::array();
      tweets_file.close();
    }
    else{
      tweets_file >> tweets_json;
      tweets_file.close();
    }
    tweets_json.push_back({
      {"id", retweet->id},
      {"original_id", retweet->get_original_id()},
      {"is_retweet", retweet->is_retweet()}
    });
		tweets_file.open(Tweet::get_general_filename(), std::fstream::out);
    tweets_file << tweets_json.dump(2);
    tweets_file.close();

		creator->push_tweet(retweet);
		for (std::vector<std::string>::iterator it = hashtags.begin() ; it != hashtags.end(); ++it){
			std::string hashtag_name = *it;
			if(Hashtag::exists(hashtag_name)){
				Hashtag* hashtag = Hashtag::get(hashtag_name);
				hashtag->increment_use_count();
				delete hashtag;
			}
			else{
	  		Hashtag::create(hashtag_name);
			}
		}
		return retweet;
	}
	else{
		if(tweets_file.is_open()) tweets_file.close();
		return nullptr;
	}
}

std::vector<Retweet*> Retweet::get_retweets(int tweet_id){
	std::ifstream tweets_file(Tweet::get_general_filename());
	Json tweets_json;
	std::vector<Retweet*> retweets;

	if(!tweets_file.is_open()){
  	throw ResourceNotFoundException("file:/"+Tweet::get_general_filename());
	}

	tweets_file >> tweets_json;

	for(Json::iterator it = tweets_json.begin(); it != tweets_json.end(); ++it){
		Json tweet = *it;
		if(tweet["is_retweet"] == true && tweet["original_id"] == tweet_id){
			Retweet* retweet = dynamic_cast<Retweet*>(Tweet::get((int)tweet["id"]));
			retweets.push_back(retweet);
		}
	}

	 tweets_file.close();
	 return retweets;
}

Json Retweet::serialize(){
  Json tweet_json = {
	  {"id", this->id},
		{"creator_id", this->creator_id},
	  {"content", this->content},
	  {"created_at", this->created_at},
	  {"hashtags", this->hashtags},
	  {"like_count", this->like_count},
	  {"user_likes", this->user_likes},
		{"replies", this->replies},
    {"original_id", this->original_id},
    {"is_retweet", this->is_retweet()}
	};
	return tweet_json;
}

void Retweet::deserialize(Json tweet_json){
  this->id = tweet_json["id"];
	this->creator_id = tweet_json["creator_id"];
	this->content = tweet_json["content"];
	this->created_at = tweet_json["created_at"];
	this->like_count = tweet_json["like_count"];
	this->original_id = tweet_json["original_id"];
	for (Json::iterator it = tweet_json["hashtags"].begin(); it != tweet_json["hashtags"].end(); ++it) {
	  this->hashtags.push_back(*it);
	}
	for (Json::iterator it = tweet_json["user_likes"].begin(); it != tweet_json["user_likes"].end(); ++it) {
	  this->user_likes.push_back(*it);
	}
	for (Json::iterator it = tweet_json["replies"].begin(); it != tweet_json["replies"].end(); ++it) {
	  this->replies.push_back(*it);
	}
}
