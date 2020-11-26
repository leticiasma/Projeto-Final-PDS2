#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../lib/reply.hpp"

// Exceptions
#include "../lib/exceptions/unexistent_reply_exception.hpp"
#include "../lib/exceptions/unexistent_user_exception.hpp"
#include "../lib/exceptions/unexistent_tweet_exception.hpp"
#include "../lib/exceptions/invalid_action_exception.hpp"
#include "../lib/exceptions/forbidden_action_exception.hpp"
#include "../lib/exceptions/resource_not_found_exception.hpp"

using Json = nlohmann::json;
using Reply = Twitter::Reply;
using User = Twitter::User;
using Tweet = Twitter::Tweet;

Reply::Reply(){}

Reply::Reply(int id){
  std::ifstream reply_file;
  Json reply_json;

  reply_file.open(Reply::get_filename(id));
  if(!reply_file.is_open()) {
    throw ResourceNotFoundException("file:/"+Reply::get_filename(id));
  }
  else {
    reply_file >> reply_json;
    this->deserialize(reply_json);
    reply_file.close();
  }
}

Reply::Reply(int creator_id, int tweet_id, std::string content){
  this->id = Reply::get_available_id();
  this->creator_id = creator_id;
  this->tweet_id = tweet_id;
  this->content = content;
}

int Reply::get_id(){
  return this->id;
}

int Reply::get_creator_id(){
  return this->creator_id;
}

int Reply::get_tweet_id(){
  return this->tweet_id;
}

time_t Reply::get_created_at(){
  return this->created_at;
}

std::string Reply::get_content(){
  return this->content;
}

std::vector<int> Reply::get_user_likes(){
  return this->user_likes;
}

int Reply::get_like_count(){
  return this->user_likes.size();
}

void Reply::set_id(int id){
  this->id = id;
}

void Reply::set_creator_id(int creator_id){
  this->creator_id = creator_id;
}

void Reply::set_user_likes(std::vector<int> user_likes){
  this->user_likes = user_likes;
}

void Reply::set_content(std::string content){
  this->content = content;
}

std::string Reply::dump(int indent){
  return this->serialize().dump(indent);
}

bool Reply::like(int user_id){
  if (User::exists(user_id)){
		for (std::vector<int>::iterator it = this->user_likes.begin(); it != this->user_likes.end(); ++it){
			if (*it == user_id){
				throw InvalidActionException("Reply::like", "User already liked this reply");
			}
		}
		this->user_likes.push_back(user_id);
		return this->save();
	}
	else{
		throw UnexistentUserException(user_id);
	}
}

bool Reply::save(){
  std::ofstream reply_file;
  Json reply_json = this->serialize();

  // Salvando no arquivo da resposta
  reply_file.open(Reply::get_filename(this->id));
  if(!reply_file.is_open()){
    throw ResourceNotFoundException("file:/"+Reply::get_filename(this->id));
  }
  reply_file << reply_json.dump(2);
  reply_file.close();
  return true;
}

bool Reply::delete_reply(User* user){
  std::fstream replies_file(Reply::get_general_filename(), std::fstream::in);
	Tweet* tweet;
  Json replies_json;
  Json new_replies_json = Json::array();

	if(user->get_id() != this->creator_id){
		throw ForbiddenActionException("Reply::delete_reply", "User is not reply's creator");
	}
	remove(Reply::get_filename(this->id).c_str());
	if(!replies_file.is_open()){
    throw ResourceNotFoundException("file:/"+Reply::get_general_filename());
  }
  else if(replies_file.peek() == std::fstream::traits_type::eof()){
    replies_file.close();
		throw UnexistentReplyException(this->id);
  }
  else{
    replies_file >> replies_json;
    replies_file.close();
  }
  for (Json::iterator it = replies_json.begin(); it != replies_json.end(); ++it) {
    Json reply = *it;
    if(reply["id"] != this->id){
      new_replies_json.push_back(reply);
    }
  }
  replies_file.open(Reply::get_general_filename(), std::fstream::out);
  replies_file << new_replies_json.dump(2);
  replies_file.close();
  try{
    tweet = Tweet::get(this->tweet_id);
  	tweet->remove_reply(this);
  }
  catch(...){
  }
  delete tweet;
  return true;
}

Json Reply::serialize(){
  Json reply_json = {
    {"id", this->id},
    {"creator_id", this->creator_id},
    {"tweet_id", this->tweet_id},
    {"created_at", this->created_at},
    {"content", this->content},
    {"user_likes", this->user_likes}
  };
  return reply_json;
}

void Reply::deserialize(Json reply_json){
  this->id = reply_json["id"];
  this->creator_id = reply_json["creator_id"];
  this->tweet_id = reply_json["tweet_id"];
  this->created_at = reply_json["created_at"];
  this->content = reply_json["content"];
  for (Json::iterator it = reply_json["user_likes"].begin(); it != reply_json["user_likes"].end(); ++it) {
    this->user_likes.push_back(*it);
  }
}

//------------------------------- Métodos estáticos --------------------------//

bool Reply::exists(int id){
  std::fstream replies_file(Reply::get_general_filename(), std::fstream::in);
  std::ifstream reply_file(Reply::get_filename(id));
  Json replies_json;

  if(!replies_file.is_open() || !reply_file.is_open()){
		if(replies_file.is_open()) replies_file.close();
		if(reply_file.is_open()) reply_file.close();
    return false;
  }
  else if(replies_file.peek() == std::fstream::traits_type::eof()){
    replies_file.close();
		reply_file.close();
    return false;
  }
  else{
    replies_file >> replies_json;
    replies_file.close();
		reply_file.close();
  }
	// std::cout << tweets_json.dump(2) << '\n';
  for (Json::iterator it = replies_json.begin(); it != replies_json.end(); ++it) {
    Json reply = *it;
    if(reply["id"] == id){
      return true;
    }
  }
  return false;
}

int Reply::get_available_id(){
  std::fstream replies_file(Reply::get_general_filename(), std::fstream::in);
  Json replies_json;
  int id = -1;

  if(!replies_file.is_open()){
    return 0;
  }
  else if(replies_file.peek() == std::fstream::traits_type::eof()){
    replies_file.close();
    return 0;
  }
  else{
    replies_file >> replies_json;
    replies_file.close();
  }
  for (Json::iterator it = replies_json.begin(); it != replies_json.end(); ++it) {
    Json reply = *it;
    if(reply["id"] > id){
      id = reply["id"];
    }
  }
  return id+1;
}

Reply* Reply::get(int id){
  Reply* reply = new Reply();
  std::ifstream reply_file;
  Json reply_json;

  reply_file.open(Reply::get_filename(id));
  if(!reply_file.is_open()) {
    delete reply;
    throw UnexistentReplyException(id);
  }
  else if(reply_file.peek() == std::ifstream::traits_type::eof()){
    reply_file.close();
    delete reply;
    throw UnexistentReplyException(id);
  }
  reply_file >> reply_json;
  reply->deserialize(reply_json);
  reply_file.close();
  return reply;
}

std::vector<Reply*> Reply::get(std::vector<int> ids){
  std::vector<Reply*> replies;
  for (std::vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    try{
      Reply* reply = Reply::get(*it);
      replies.push_back(reply);
    }
    catch(UnexistentReplyException e){
      continue;
    }
  }
  return replies;
}

std::vector<Reply*> Reply::get_from_user(User* user){
  std::vector<Reply*> replies;
  std::ifstream replies_file(Reply::get_general_filename());
  Json replies_json;

  if(!replies_file.is_open()){
    return replies;
  }
  else if(replies_file.peek() == std::fstream::traits_type::eof()){
    replies_file.close();
    return replies;
  }
  replies_file >> replies_json;
  for (Json::iterator it = replies_json.begin(); it != replies_json.end(); ++it) {
    Json reply = *it;
    if(reply["creator_id"] == user->get_id()){
      try{
        replies.push_back(Reply::get((int)reply["id"]));
      }
      catch(...){
        continue;
      }
    }
  }
  return replies;
}

Reply* Reply::post_reply(User* creator, Tweet* tweet, std::string content){
  Reply* reply = new Reply(creator->get_id(), tweet->get_id(), content);
  std::fstream replies_file(Reply::get_general_filename(), std::fstream::in);
  Json replies_json;

  if(!User::exists(creator->get_id())){
    delete reply;
    throw UnexistentUserException(creator->get_id());
  }
  if(!Tweet::exists(tweet->get_id())){
    delete reply;
    throw UnexistentTweetException(tweet->get_id());
  }
  try{
    reply->save();
    if(!replies_file.is_open()){
      replies_json = Json::array();
    }
    else if(replies_file.peek() == std::fstream::traits_type::eof()){
      replies_json = Json::array();
      replies_file.close();
    }
    else{
      replies_file >> replies_json;
      replies_file.close();
    }
    replies_json.push_back({
      {"id", reply->id},
      {"tweet_id", reply->tweet_id},
      {"creator_id", reply->creator_id}
    });
    replies_file.open(Reply::get_general_filename(), std::fstream::out);
    replies_file << replies_json.dump(2);
    replies_file.close();
    tweet->add_reply(reply);
    return reply;
  }
  catch(ResourceNotFoundException e){
    delete reply;
    throw e;
  }
}

std::string Reply::get_filename(int id){
  return "data/reply_"+std::to_string(id)+".json";
}

std::string Reply::get_general_filename(){
  return "data/replies.json";
}
