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

Tweet::Tweet(){}

Tweet::Tweet(int id){
	// Recuperar tweet com o id recebido (Ver exemplo em 'User::User(int id)')
	std::ifstream tweet_file;
	Json tweet_json;

	tweet_file.open(Tweet::get_filename(id));
	if(!tweet_file.is_open()) {
    throw ResourceNotFoundException("file:/"+Tweet::get_filename(id));
	}
	else {
		tweet_file >> tweet_json;
		this->deserialize(tweet_json);
		tweet_file.close();
	}
}

Tweet::Tweet(int creator_id, std::string content, std::vector<std::string> hashtags){
	// Verificar dependências e salvar o tweet com o método 'bool Tweet::save()'
	if (User::exists(creator_id)){
		auto current_time = std::chrono::system_clock::now();
		this->id = get_available_id();
		this->creator_id = creator_id;
		this->like_count = 0;
		this->content = content;
		this->hashtags = hashtags;
		this->created_at = std::chrono::system_clock::to_time_t(current_time);
	}
}

int Tweet::get_id(){
	return this->id;
}

int Tweet::get_creator_id(){
	return this->creator_id;
}

int Tweet::get_like_count(){
 	return this->like_count;
}

std::string Tweet::get_content(){
	return this->content;
}

std::vector<std::string> Tweet::get_hashtags(){
	return this->hashtags;
}

std::vector<int> Tweet::get_user_likes(){
	return this->user_likes;
}

std::vector<int> Tweet::get_replies(){
	return this->replies;
}

time_t Tweet::get_created_at(){
	return this->created_at;
}

void Tweet::set_id(int id){
	this->id = id;
}

void Tweet::set_creator_id(int creator_id){
	this->creator_id = creator_id;
}

void Tweet::set_content(std::string content){
	this->content = content;
}

void Tweet::set_like_count(){
	this->like_count = this->user_likes.size();
}

void Tweet::set_hashtags(std::vector<std::string> hashtags){
	this->hashtags = hashtags;
}

void Tweet::set_user_likes(std::vector<int> user_likes){
	this->user_likes = user_likes;
}

void Tweet::set_replies(std::vector<int> replies){
	this->replies = replies;
}

std::string Tweet::dump(int indent){
	return this->serialize().dump(indent);
}

void Tweet::add_reply(Reply* reply){
	if (Reply::exists(reply->get_id())){
		for (std::vector<int>::iterator it = this->replies.begin() ; it != this->replies.end(); ++it){
			if (*it == reply->get_id()){
				throw InvalidActionException("Tweet::add_reply", "Reply is already done");
			}
		}
		this->replies.push_back(reply->get_id());
		try{
			this->save();
		}
		catch(ResourceNotFoundException e){
			throw e;
		}
	}
	else{
		throw UnexistentReplyException(reply->get_id());
	}
}

bool Tweet::remove_reply(Reply* reply){
	std::vector<int> replies = this->get_replies();
	for(std::vector<int>::iterator it = this->replies.begin(); it != this->replies.end(); ++it){
		if(*it == reply->get_id()){
			this->replies.erase(it);
			break;
		}
	}
  return this->save();
}

bool Tweet::save(){
	std::ofstream tweet_file;
	Json tweet_json = this->serialize();

	// Salvando no arquivo do tweet
	tweet_file.open(Tweet::get_filename(this->id));
	if(!tweet_file.is_open()){
    throw ResourceNotFoundException("file:/"+Tweet::get_filename(this->id));
	}
	tweet_file << tweet_json.dump(2);
	tweet_file.close();
	return true;
}

bool Tweet::like_tweet(int user_id){
	// Realizar as verificações de conssitência (se o usuário existe, se ele ainda n curtiu o tweet e etc)
	// Atualizar o numero de likes, adicionar o usuário no vetor de usuários que curtiram
	// Salvar o tweet no arquivo com o método 'bool Tweet::save()'
	if (User::exists(user_id)){
		for (std::vector<int>::iterator it = this->user_likes.begin() ; it != this->user_likes.end(); ++it){
			if (*it == user_id){
				throw InvalidActionException("Tweet::like", "User already likes the tweet");
			}
		}
		this->user_likes.push_back(user_id);
		this->set_like_count();
		try{
			return this->save();
		}
		catch(ResourceNotFoundException e){
			this->user_likes.pop_back();
			this->set_like_count();
			throw e;
		}
	}
	else{
		throw UnexistentUserException(user_id);
	}
}

bool Tweet::dislike_tweet(int user_id){
	if (User::exists(user_id)){
		for (std::vector<int>::iterator it = this->user_likes.begin() ; it != this->user_likes.end(); ++it){
			if (*it == user_id){
				this->user_likes.erase(it);
				this->set_like_count();
				try{
					return this->save();
				}
				catch(ResourceNotFoundException e){
					this->user_likes.push_back(user_id);
					throw e;
				}
			}
		}
	}
	else{
		throw UnexistentUserException(user_id);
	}
}

bool Tweet::delete_tweet(User* user){
  std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
	std::vector<Retweet*> retweets;
	std::vector<Reply*> replies;
  Json tweets_json;
  Json new_tweets_json = Json::array();

	if(user->get_id() != this->creator_id){
		throw ForbiddenActionException("Tweet::delete_tweet", "User is not tweet's creator");
	}
	replies = Reply::get(this->get_replies());
	for (std::vector<Reply*>::iterator it = replies.begin(); it != replies.end(); ++it) {
		Reply* reply = *it;
		User* reply_creator = User::get(reply->get_creator_id());
		reply->delete_reply(reply_creator);
		delete reply_creator;
		delete *it;
	}
	retweets = Retweet::get_retweets(this->id);
	for (std::vector<Retweet*>::iterator it = retweets.begin(); it != retweets.end(); ++it) {
		Retweet* retweet = *it;
		try{
			retweet->make_tweet();
		}
		catch(...){
			continue;
		}
		delete *it;
	}
	remove(Tweet::get_filename(this->id).c_str());
	if(!tweets_file.is_open()){
    throw ResourceNotFoundException("file:/"+Tweet::get_general_filename());
  }
  else if(tweets_file.peek() == std::fstream::traits_type::eof()){
    tweets_file.close();
		throw UnexistentTweetException(this->id);
  }
  else{
    tweets_file >> tweets_json;
    tweets_file.close();
  }
  for (Json::iterator it = tweets_json.begin(); it != tweets_json.end(); ++it) {
    Json tweet = *it;
    if(tweet["id"] != this->id){
      new_tweets_json.push_back(tweet);
    }
  }
  tweets_file.open(Tweet::get_general_filename(), std::fstream::out);
  tweets_file << new_tweets_json.dump(2);
  tweets_file.close();
	user->remove_tweet(this);
  return true;
}

//------------------------------- Métodos estáticos --------------------------//

bool Tweet::exists(int id){
	std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  std::ifstream tweet_file(Tweet::get_filename(id));
  Json tweets_json;

  if(!tweets_file.is_open() || !tweet_file.is_open()){
		if(tweets_file.is_open()) tweets_file.close();
		if(tweet_file.is_open()) tweet_file.close();
    return false;
  }
  else if(tweets_file.peek() == std::fstream::traits_type::eof()){
    tweets_file.close();
		tweet_file.close();
    return false;
  }
  else{
    tweets_file >> tweets_json;
    tweets_file.close();
		tweet_file.close();
  }
  for (Json::iterator it = tweets_json.begin(); it != tweets_json.end(); ++it) {
    Json tweet = *it;
    if(tweet["id"] == id){
      return true;
    }
  }
  return false;
}

int Tweet::get_available_id(){
  std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  Json tweets_json;
  int id = -1;

	if(!tweets_file.is_open()){
		return 0;
	}
	else if(tweets_file.peek() == std::fstream::traits_type::eof()){
		tweets_file.close();
		return 0;
	}
	else{
		tweets_file >> tweets_json;
		tweets_file.close();
	}
	for (Json::iterator it = tweets_json.begin(); it != tweets_json.end(); ++it) {
		Json tweet = *it;
		if(tweet["id"] > id){
			id = tweet["id"];
		}
	}
	return id+1;
}

Tweet* Tweet::post_tweet(int creator_id, std::string content, std::vector<std::string> hashtags){
	// Realizar as verificações de conssitência (se o usuário existe e etc)
	// Chamar o construtor de Tweet apropiado
	// Tratar as hashtagas (criar as inexistentes e incrementar o contador de uso das existentes)
	// adicionar tweet no vetor de tweets do usuário
	// Salvar o Tweet nos arquivos
	std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  Json tweets_json;
	User* creator;

	if (User::exists(creator->get_id())){
		creator = User::get(creator_id);
		Tweet* tweet = new Tweet(creator->get_id(), content, hashtags);

		try{
			if(tweet->save()){
				if(!tweets_file.is_open()){
		      tweets_json = Json::array();
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
		      {"id", tweet->id},
		      {"is_retweet", tweet->is_retweet()}
		    });
				tweets_file.open(Tweet::get_general_filename(), std::fstream::out);
		    tweets_file << tweets_json.dump(2);
		    tweets_file.close();

				creator->push_tweet(tweet);
				for (std::vector<std::string>::iterator it = hashtags.begin() ; it != hashtags.end(); ++it){
					std::string hashtag_name = *it;
					if(Hashtag::exists(hashtag_name)){
						Hashtag* hashtag = Hashtag::get(hashtag_name);
						hashtag->increment_use_count();
						delete hashtag;
					}
					else{
			  		Hashtag* hashtag = Hashtag::create(hashtag_name);
						delete hashtag;
					}
				}
				delete creator;
				return tweet;
			}
			else{
				if(tweets_file.is_open()) tweets_file.close();
				delete creator;
				return nullptr;
			}
		}
		catch(ResourceNotFoundException e){
			delete tweet;
			delete creator;
			return nullptr;
		}
	}
	else{
		if(tweets_file.is_open()) tweets_file.close();
		return nullptr;
	}
}

Tweet* Tweet::post_tweet(User* creator, std::string content, std::vector<std::string> hashtags){
	// Realizar as verificações de conssitência (se o usuário existe e etc)
	// Chamar o construtor de Tweet apropiado
	// Tratar as hashtagas (criar as inexistentes e incrementar o contador de uso das existentes)
	// adicionar tweet no vetor de tweets do usuário
	// Salvar o Tweet nos arquivos
	std::fstream tweets_file(Tweet::get_general_filename(), std::fstream::in);
  Json tweets_json;

	if (User::exists(creator->get_id())){
		Tweet* tweet = new Tweet(creator->get_id(), content, hashtags);

		try{
			if(tweet->save()){
				if(!tweets_file.is_open()){
		      tweets_json = Json::array();
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
		      {"id", tweet->id},
		      {"is_retweet", tweet->is_retweet()}
		    });
				tweets_file.open(Tweet::get_general_filename(), std::fstream::out);
		    tweets_file << tweets_json.dump(2);
		    tweets_file.close();

				creator->push_tweet(tweet);
				for (std::vector<std::string>::iterator it = hashtags.begin() ; it != hashtags.end(); ++it){
					std::string hashtag_name = *it;
					if(Hashtag::exists(hashtag_name)){
						Hashtag* hashtag = Hashtag::get(hashtag_name);
						hashtag->increment_use_count();
						delete hashtag;
					}
					else{
			  		Hashtag* hashtag = Hashtag::create(hashtag_name);
						delete hashtag;
					}
				}
				return tweet;
			}
			else{
				if(tweets_file.is_open()) tweets_file.close();
				return nullptr;
			}
		}
		catch(ResourceNotFoundException e){
			delete tweet;
			return nullptr;
		}
	}
	else{
		if(tweets_file.is_open()) tweets_file.close();
		return nullptr;
	}
}

Tweet* Tweet::get(int id){
	Tweet* tweet;
	std::ifstream tweet_file;
	Json tweet_json;

	tweet_file.open(Tweet::get_filename(id));
	if(!tweet_file.is_open()) {
	  throw UnexistentTweetException(id);
	}
	else if(tweet_file.peek() == std::ifstream::traits_type::eof()){
	  tweet_file.close();
	  throw UnexistentTweetException(id);
	}
	tweet_file >> tweet_json;
	if(tweet_json["is_retweet"]){
		tweet = new Retweet();
	}
	else{
		tweet = new Tweet();
	}
	tweet->deserialize(tweet_json);
	tweet_file.close();
	return tweet;
}

std::vector<Tweet*> Tweet::get(std::vector<int> ids){
  std::vector<Tweet*> tweets;
  for (std::vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    try{
      Tweet* tweet = Tweet::get(*it);
      if(tweet != nullptr){
        tweets.push_back(tweet);
      }
    }
    catch(UnexistentTweetException e){
      continue;
    }
  }
  return tweets;
}

std::string Tweet::get_filename(int id){
	return "data/tweet_"+std::to_string(id)+".json";
}

std::string Tweet::get_general_filename(){
  return "data/tweets.json";
}

Json Tweet::serialize(){
	Json tweet_json = {
	  {"id", this->id},
		{"creator_id", this->creator_id},
	  {"content", this->content},
	  {"created_at", this->created_at},
	  {"hashtags", this->hashtags},
	  {"like_count", this->like_count},
	  {"user_likes", this->user_likes},
		{"replies", this->replies},
    {"is_retweet", this->is_retweet()}
	};
	return tweet_json;
}

void Tweet::deserialize(Json tweet_json){
	this->id = tweet_json["id"];
	this->creator_id = tweet_json["creator_id"];
	this->content = tweet_json["content"];
	this->created_at = tweet_json["created_at"];
	this->like_count = tweet_json["like_count"];
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

bool Tweet::compare_time(Tweet* a, Tweet* b){
	return (a->created_at < b->created_at);
}
