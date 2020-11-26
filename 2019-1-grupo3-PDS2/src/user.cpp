#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include "../lib/nlohmann/json.hpp"
#include "../lib/user.hpp"

// Exceptions
#include "../lib/exceptions/unexistent_user_exception.hpp"
#include "../lib/exceptions/unexistent_tweet_exception.hpp"
#include "../lib/exceptions/unexistent_reply_exception.hpp"
#include "../lib/exceptions/invalid_action_exception.hpp"
#include "../lib/exceptions/forbidden_action_exception.hpp"
#include "../lib/exceptions/resource_not_found_exception.hpp"

using Json = nlohmann::json;
using Tweet = Twitter::Tweet;
using User = Twitter::User;

//--------------------------- Métodos de instância ---------------------------//

User::User(){}

User::User(int id){
  std::ifstream user_file;
  Json user_json;

  user_file.open(User::get_filename(id));
  if(!user_file.is_open()) {
    throw ResourceNotFoundException("file:/"+User::get_filename(id));
  }
  else {
    user_file >> user_json;
    this->deserialize(user_json);
    user_file.close();
  }
}

User::User(std::string username, std::string password){
  this->id = User::get_available_id();
  this->username = username;
  this->password = password;
}

int User::get_id(){
  return this->id;
}

std::string User::get_username(){
  return this->username;
}

std::vector<int> User::get_followers(){
  return this->followers;
}

std::vector<int> User::get_followees(){
  return this->followees;
}

std::vector<int> User::get_tweets(){
  return this->tweets;
}

void User::set_id(int id){
  this->id = id;
}

void User::set_username(std::string username){
  this->username = username;
}

void User::set_followers(std::vector<int> followers){
  this->followers = followers;
}

void User::set_followees(std::vector<int> followees){
  this->followees = followees;
}

void User::set_tweets(std::vector<int> tweets){
  this->tweets = tweets;
}

std::string User::dump(int indent){
  Json user_json;
  user_json["id"] = this->id;
  user_json["username"] = this->username;
  user_json["followers"] = this->followers;
  user_json["followees"] = this->followees;
  user_json["tweets"] = this->tweets;
  return user_json.dump(indent);
}

void User::operator<<(Tweet* tweet){
  if(tweet != nullptr && Tweet::exists(tweet->get_id()) && tweet->get_creator_id() == this->id){
    this->tweets.push_back(tweet->get_id());
  }
  try{
    this->save();
  }
  catch(ResourceNotFoundException e){
  }
}

bool User::push_tweet(Tweet* tweet){
  if(tweet != nullptr && Tweet::exists(tweet->get_id()) && tweet->get_creator_id() == this->id){
    for (std::vector<int>::iterator it = this->tweets.begin(); it != this->tweets.end(); ++it) {
      if(*it == tweet->get_id()) return false;
    }
    this->tweets.push_back(tweet->get_id());
    try{
      this->save();
      return true;
    }
    catch(ResourceNotFoundException e){
      return false;
    }
  }
  else{
    return false;
  }
}

bool User::remove_tweet(Tweet* tweet){
  std::vector<int> new_tweets;
	for(std::vector<int>::iterator it = this->tweets.begin(); it != this->tweets.end(); ++it){
		if(*it != tweet->get_id()){
			new_tweets.push_back(*it);
		}
	}
  this->set_tweets(new_tweets);
  return this->save();
}

bool User::save(){
  std::ofstream user_file;
  Json user_json = this->serialize();

  // Salvando no arquivo do usuário
  user_file.open(User::get_filename(this->id));
  if(!user_file.is_open()){
    throw ResourceNotFoundException("file:/"+User::get_filename(this->id));
  }
  user_file << user_json.dump(2);
  user_file.close();
  return true;
}

// Uso não recomendado
bool User::follow(int user_id){
  User* followee;

  std::cout << "WARNING:" << std::endl;
  std::cout << "The method 'bool User::follow(int user_id)' should be used with caution." << std::endl;
  std::cout << "For a safer approach, use 'bool User::follow(User* followee)' instead." << std::endl;

  if(this->id == user_id){
    throw InvalidActionException("User::follow", "User can't follow himself");
  }
  if(!User::exists(user_id)){
    throw UnexistentUserException(user_id);
  }
  for (std::vector<int>::iterator it = this->followees.begin(); it != this->followees.end(); ++it) {
    if(*it == user_id){
      throw InvalidActionException("User::follow", "User already follows followee");
    }
  }
  followee = new User(user_id);
  followee->followers.push_back(this->id);
  this->followees.push_back(followee->id);
  try{
    this->save();
    followee->save();
    delete followee;
    return true;
  }
  catch(ResourceNotFoundException e){
    delete followee;
    return false;
  }
}

bool User::follow(User* followee){
  if(this->id == followee->id){
    throw InvalidActionException("User::follow", "User can't follow himself");
  }
  if(!User::exists(followee->id)){
    throw UnexistentUserException(followee->id);
  }
  for (std::vector<int>::iterator it = this->followees.begin(); it != this->followees.end(); ++it) {
    if(*it == followee->id){
      throw InvalidActionException("User::follow", "User already follows followee");
    }
  }
  followee->followers.push_back(this->id);
  this->followees.push_back(followee->id);
  try{
    this->save();
    followee->save();
    return true;
  }
  catch(ResourceNotFoundException e){
    return false;
  }
}

bool User::unfollow(int user_id){
  User* followee;

  std::cout << "WARNING:" << std::endl;
  std::cout << "The method 'bool User::unfollow(int user_id)' should be used with caution." << std::endl;
  std::cout << "For a safer approach, use 'bool User::unfollow(User* followee)' instead." << std::endl;

  if(this->id == user_id){
    throw InvalidActionException("User::follow", "User can't unfollow himself");
  }
  if(!User::exists(user_id)){
    throw UnexistentUserException(user_id);
  }
  try{
    followee = User::get(user_id);
  }
  catch(UnexistentUserException e){
    return false;
  }
  if(followee == nullptr){
    throw UnexistentUserException(user_id);
  }
  followee->remove_follower(this->id);
  this->remove_followee(followee->id);
  try{
    this->save();
    followee->save();
    delete followee;
    return true;
  }
  catch(ResourceNotFoundException e){
    delete followee;
    return false;
  }
}

bool User::unfollow(User* followee){
  if(this->id == followee->id){
    throw InvalidActionException("User::follow", "User can't unfollow himself");
  }
  if(!User::exists(followee->id)){
    throw UnexistentUserException(followee->id);
  }
  if(!followee->remove_follower(this->id)){
    throw InvalidActionException("User::follow", "User does not follows followee");
  }
  this->remove_followee(followee->id);
  try{
    this->save();
    followee->save();
    return true;
  }
  catch(ResourceNotFoundException e){
    return false;
  }
}

bool User::remove_followee(int user_id){
  for (std::vector<int>::iterator it = this->followees.begin(); it != this->followees.end(); ++it) {
    if (*it == user_id) {
      this->followees.erase(it);
      return true;
    }
  }
  return false;
}

bool User::remove_follower(int user_id){
  for (std::vector<int>::iterator it = this->followers.begin(); it != this->followers.end(); ++it) {
    if (*it == user_id) {
      this->followers.erase(it);
      return true;
    }
  }
  return false;
}

bool User::compare_password(std::string password){
  std::ifstream user_file;
  Json user_json;

  user_file.open(User::get_filename(this->id));
  if(!user_file.is_open()) {
    throw ResourceNotFoundException("file:/"+User::get_filename(this->id));
  }
  else {
    user_file >> user_json;
    user_file.close();
    return password == user_json["password"];
  }
}

bool User::delete_account(std::string password){
  std::fstream logins_file(User::get_general_filename(), std::fstream::in);
  std::vector<Tweet*> tweets;
  std::vector<Reply*> replies;
  Json logins_json;
  Json new_logins_json = Json::array();

  if(!this->compare_password(password)){
    throw ForbiddenActionException("User::delete", "Wrong password");
  }
  for (std::vector<int>::iterator it = this->followers.begin(); it != this->followers.end(); ++it) {
    User* follower = new User(*it);
    follower->remove_followee(this->id);
    try{
      follower->save();
      delete follower;
    }
    catch(ResourceNotFoundException e){
      delete follower;
      continue;
    }
  }
  for (std::vector<int>::iterator it = this->followees.begin(); it != this->followees.end(); ++it) {
    User* followee = new User(*it);
    followee->remove_follower(this->id);
    try{
      followee->save();
      delete followee;
    }
    catch(ResourceNotFoundException e){
      delete followee;
      continue;
    }
  }
  tweets = Tweet::get(this->tweets);
  for (std::vector<Tweet*>::iterator it = tweets.begin(); it != tweets.end(); ++it) {
    Tweet* tweet = *it;
    try{
      tweet->delete_tweet(this);
    }
    catch(...){
      continue;
    }
    delete *it;
  }
  replies = Reply::get_from_user(this);
  for (std::vector<Reply*>::iterator it = replies.begin(); it != replies.end(); ++it) {
    Reply* reply = *it;
    try{
      reply->delete_reply(this);
    }
    catch(...){
      continue;
    }
    delete *it;
  }
  remove(User::get_filename(this->id).c_str());
  if(!logins_file.is_open()){
    throw ResourceNotFoundException("file:/"+User::get_general_filename());
  }
  else if(logins_file.peek() == std::fstream::traits_type::eof()){
    logins_file.close();
    return false;
  }
  else{
    logins_file >> logins_json;
    logins_file.close();
  }
  for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
    Json login = *it;
    if(login["id"] != this->id){
      new_logins_json.push_back(login);
    }
  }
  logins_file.open(User::get_general_filename(), std::fstream::out);
  logins_file << new_logins_json.dump(2);
  logins_file.close();
  return true;
}

Json User::serialize(){
  Json user_json = {
    {"id", this->id},
    {"username", this->username},
    {"password", this->password},
    {"followers", this->followers},
    {"followees", this->followees},
    {"tweets", this->tweets}
  };
  return user_json;
}

void User::deserialize(Json user_json){
  this->id = user_json["id"];
  this->username = user_json["username"];
  this->password = user_json["password"];
  for (Json::iterator it = user_json["followers"].begin(); it != user_json["followers"].end(); ++it) {
    this->followers.push_back(*it);
  }
  for (Json::iterator it = user_json["followees"].begin(); it != user_json["followees"].end(); ++it) {
    this->followees.push_back(*it);
  }
  for (Json::iterator it = user_json["tweets"].begin(); it != user_json["tweets"].end(); ++it) {
    this->tweets.push_back(*it);
  }
}

//------------------------------- Métodos estáticos --------------------------//

std::string User::get_filename(int id){
  return "data/user_"+std::to_string(id)+".json";
}

std::string User::get_general_filename(){
  return "data/logins.json";
}

User* User::create(std::string username, std::string password){
  User* user = new User(username, password);
  std::fstream logins_file(User::get_general_filename(), std::fstream::in);
  Json logins_json;

  if(!User::is_username_available(user->username)){
    delete user;
    throw InvalidActionException("User::Create", "Username already in use");
  }
  try{
    user->save();
    if(!logins_file.is_open()){
      logins_json = Json::array();
      std::cout << "Creating new logins file" << std::endl;
    }
    else if(logins_file.peek() == std::fstream::traits_type::eof()){
      logins_json = Json::array();
      std::cout << "Logins file is empty" << std::endl;
      logins_file.close();
    }
    else{
      logins_file >> logins_json;
      logins_file.close();
    }
    logins_json.push_back({
      {"id", user->id},
      {"username", user->username}
    });
    // Salvando no arquivo de logins
    logins_file.open(User::get_general_filename(), std::fstream::out);
    logins_file << logins_json.dump(2);
    logins_file.close();
    return user;
  }
  catch(ResourceNotFoundException e){
    delete user;
    throw ResourceNotFoundException("User file");
  }
}

User* User::login(std::string username, std::string password){
  User *user;
  std::ifstream logins_file;
  Json logins_json;

  logins_file.open(User::get_general_filename());
  if(!logins_file.is_open()){
    return nullptr;
  }
  else if(logins_file.peek() == std::ifstream::traits_type::eof()){
    logins_file.close();
    return nullptr;
  }
  else{
    logins_file >> logins_json;
    for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
      Json login_json = *it;
      if(login_json["username"] == username){
        try{
          user = User::get((int)login_json["id"]);
        }
        catch(UnexistentUserException e){
          return nullptr;
        }
        try{
          if(user->compare_password(password)){
            return user;
          }
          else{
            delete user;
            return nullptr;
          }
        }
        catch(ResourceNotFoundException e){
          delete user;
          return nullptr;
        }
      }
    }
    return nullptr;
  }
}

User* User::get(int id){
  User* user = new User();
  std::ifstream user_file;
  Json user_json;

  user_file.open(User::get_filename(id));
  if(!user_file.is_open()) {
    delete user;
    throw UnexistentUserException(id);
  }
  else if(user_file.peek() == std::ifstream::traits_type::eof()){
    user_file.close();
    delete user;
    throw UnexistentUserException(id);
  }
  user_file >> user_json;
  user->deserialize(user_json);
  user_file.close();
  return user;
}

std::vector<User*> User::get(std::vector<int> ids){
  std::vector<User*> users;
  for (std::vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    try{
      User* user = User::get(*it);
      if(user != nullptr){
        users.push_back(user);
      }
    }
    catch(UnexistentUserException e){
      continue;
    }
  }
  return users;
}

std::vector<User*> User::search(std::string search_term){
  std::vector<User*> users;
  std::ifstream logins_file(User::get_general_filename());
  Json logins_json;
  std::transform(search_term.begin(), search_term.end(), search_term.begin(), ::tolower);

  if(!logins_file.is_open()){
    return users;
  }
  else if(logins_file.peek() == std::ifstream::traits_type::eof()){
    logins_file.close();
    return users;
  }
  else{
    logins_file >> logins_json;
    logins_file.close();
  }
  for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
    Json login = *it;
    int user_id = login["id"];
    std::string username = login["username"];
    std::transform(username.begin(), username.end(), username.begin(), ::tolower);

    if(username == search_term){
      try{
        users.insert(users.begin(), User::get(user_id));
      }
      catch(UnexistentUserException e){
        continue;
      }
    }
    else if (username.find(search_term) != std::string::npos) {
      try{
        users.push_back(User::get(user_id));
      }
      catch(UnexistentUserException e){
        continue;
      }
    }
  }
  return users;
}

bool User::is_username_available(std::string username){
  std::fstream logins_file(User::get_general_filename(), std::fstream::in);
  Json logins_json;

  if(!logins_file.is_open()){
    return true;
  }
  else if(logins_file.peek() == std::fstream::traits_type::eof()){
    logins_file.close();
    return true;
  }
  else{
    logins_file >> logins_json;
    logins_file.close();
  }
  for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
    Json login = *it;
    if(login["username"] == username){
      return false;
    }
  }
  return true;
}

bool User::exists(int id){
  std::fstream logins_file(User::get_general_filename(), std::fstream::in);
  std::ifstream user_file(User::get_filename(id));
  Json logins_json;

  if(!logins_file.is_open() || !user_file.is_open()){
    if(logins_file.is_open()) logins_file.close();
    if (user_file.is_open()) user_file.close();
    return false;
  }
  else if(logins_file.peek() == std::fstream::traits_type::eof()){
    logins_file.close();
    user_file.close();
    return false;
  }
  else{
    logins_file >> logins_json;
    logins_file.close();
    user_file.close();
  }
  for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
    Json login = *it;
    if(login["id"] == id){
      return true;
    }
  }
  return false;
}

int User::get_available_id(){
  std::fstream logins_file(User::get_general_filename(), std::fstream::in);
  Json logins_json;
  int id = -1;

  if(!logins_file.is_open()){
    return 0;
  }
  else if(logins_file.peek() == std::fstream::traits_type::eof()){
    logins_file.close();
    return 0;
  }
  else{
    logins_file >> logins_json;
    logins_file.close();
  }
  for (Json::iterator it = logins_json.begin(); it != logins_json.end(); ++it) {
    Json login = *it;
    if(login["id"] > id){
      id = login["id"];
    }
  }
  return id+1;
}
