#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include "../lib/hashtag.hpp"
#include "../lib/nlohmann/json.hpp"

using Json = nlohmann::json;
using Hashtag = Twitter::Hashtag;

Hashtag::Hashtag(){}

Hashtag::Hashtag(int id){
  std::ifstream hashtag_file;
  Json hashtag_json;

  hashtag_file.open(Hashtag::get_filename(id));
  if(!hashtag_file.is_open()) {
    std::cout << "Unable to open hashtag file" << std::endl;
  }
  else if(hashtag_file.peek() == std::ifstream::traits_type::eof()){
    std::cout << "Unable to get hashtag info" << std::endl;
    hashtag_file.close();
  }
  else {
    hashtag_file >> hashtag_json;
    hashtag_file.close();
    this->deserialize(hashtag_json);
  }
}

Hashtag::Hashtag(std::string name){
  this->id = Hashtag::get_available_id();
  this->name = name;
  this->use_count = 1;
}

std::string Hashtag::get_filename(int id){
  return "data/hashtag_"+std::to_string(id)+".json";
}

std::string Hashtag::get_general_filename(){
  return "data/hashtags.json";
}

Json Hashtag::serialize(){
  Json hashtag_json = {
    {"id", this->id},
    {"use_count", this->use_count},
    {"name", this->name}
  };
  return hashtag_json;
}

void Hashtag::deserialize(Json hashtag_json){
  this->id = hashtag_json["id"];
  this->use_count = hashtag_json["use_count"];
  this->name = hashtag_json["name"];
}

int Hashtag::get_id(){
  return this->id;
}

int Hashtag::get_use_count(){
  return this->use_count;
}

std::string Hashtag::get_name(){
  return this->name;
}

std::vector<int> Hashtag::get_tweets(){
  return this->tweets;
}

void Hashtag::set_id(int id){
  this->id = id;
}

void Hashtag::set_use_count(int use_count){
  this->use_count = use_count;
}

void Hashtag::increment_use_count(){
  this->use_count++;
  this->save();
}

void Hashtag::set_name(std::string name){
  this->name = name;
}

void Hashtag::set_tweets(std::vector<int> tweets){
  this->tweets = tweets;
}

std::string Hashtag::dump(int indent){
  return this->serialize().dump(indent);
}

Hashtag* Hashtag::create(std::string name){
  Hashtag* hashtag = new Hashtag(name);
  std::fstream hashtags_file(Hashtag::get_general_filename(), std::fstream::in);
  Json hashtags_json;

  if(Hashtag::exists(hashtag->name)){
    std::cout << "Hashtag already exists" << std::endl;
    return nullptr;
  }
  bool success = hashtag->save();
  if(success){
    if(!hashtags_file.is_open()){
      hashtags_json = Json::array();
      std::cout << "Creating new hashtags file" << std::endl;
    }
    else if(hashtags_file.peek() == std::fstream::traits_type::eof()){
      hashtags_json = Json::array();
      hashtags_file.close();
    }
    else{
      hashtags_file >> hashtags_json;
      hashtags_file.close();
    }
    hashtags_json.push_back({
      {"id", hashtag->id},
      {"name", hashtag->name}
    });
    // Salvando no arquivo de hashtags
    hashtags_file.open(Hashtag::get_general_filename(), std::fstream::out);
    hashtags_file << hashtags_json.dump(2);
    hashtags_file.close();
    return hashtag;
  }
  else return nullptr;
}

bool Hashtag::save(){
  std::ofstream hashtag_file;
  Json hashtag_json = this->serialize();

  // Salvando no arquivo do hashtag
  hashtag_file.open(Hashtag::get_filename(this->id));
  if(!hashtag_file.is_open()){
    std::cout << "Cant open hashtag file #" << this->id << std::endl;
    return false;
  }
  hashtag_file << hashtag_json.dump(2);
  hashtag_file.close();
  return true;
}

Hashtag* Hashtag::get(int id){
  Hashtag* hashtag = new Hashtag();
  std::ifstream hashtag_file;
  Json hashtag_json;

  hashtag_file.open(Hashtag::get_filename(id));
  if(!hashtag_file.is_open()) {
    std::cout << "Unable to open hashtag file" << std::endl;
    return nullptr;
  }
  else if(hashtag_file.peek() == std::ifstream::traits_type::eof()){
    std::cout << "Unable to get hashtag info" << std::endl;
    hashtag_file.close();
    return nullptr;
  }
  else {
    hashtag_file >> hashtag_json;
    hashtag_file.close();
    hashtag->deserialize(hashtag_json);
  }
  return hashtag;
}

Hashtag* Hashtag::get(std::string name){
  std::ifstream hashtags_file;
  Json hashtags_json;

  hashtags_file.open(Hashtag::get_general_filename());
  if(!hashtags_file.is_open()) {
    std::cout << "Unable to open hashtags file" << std::endl;
    return nullptr;
  }
  else if(hashtags_file.peek() == std::ifstream::traits_type::eof()){
    std::cout << "Unexistent hashtag" << std::endl;
    hashtags_file.close();
    return nullptr;
  }
  else {
    hashtags_file >> hashtags_json;
    hashtags_file.close();
    for (Json::iterator it = hashtags_json.begin(); it != hashtags_json.end(); ++it) {
      Json hashtag_json = *it;
      if(hashtag_json["name"] == name){
        return Hashtag::get((int)hashtag_json["id"]);
      }
    }
  }
  return nullptr;
}

std::vector<Hashtag*> Hashtag::get(std::vector<int> ids){
  std::vector<Hashtag*> hashtags;
  for (std::vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    hashtags.push_back(Hashtag::get(*it));
  }
  return hashtags;
}

int Hashtag::get_available_id(){
  std::fstream hashtags_file(Hashtag::get_general_filename(), std::fstream::in);
  Json hashtags_json;
  int id = -1;

  if(!hashtags_file.is_open()){
    return 0;
  }
  else if(hashtags_file.peek() == std::fstream::traits_type::eof()){
    hashtags_file.close();
    return 0;
  }
  else{
    hashtags_file >> hashtags_json;
    hashtags_file.close();
  }
  for (Json::iterator it = hashtags_json.begin(); it != hashtags_json.end(); ++it) {
    Json hashtag = *it;
    if(hashtag["id"] > id){
      id = hashtag["id"];
    }
  }
  return id+1;
}

bool Hashtag::exists(int id){
  std::fstream hashtags_file(Hashtag::get_general_filename(), std::fstream::in);
  Json hashtags_json;

  if(!hashtags_file.is_open()){
    return false;
  }
  else if(hashtags_file.peek() == std::fstream::traits_type::eof()){
    hashtags_file.close();
    return false;
  }
  else{
    hashtags_file >> hashtags_json;
    hashtags_file.close();
  }
  for (Json::iterator it = hashtags_json.begin(); it != hashtags_json.end(); ++it) {
    Json hashtag = *it;
    if(hashtag["id"] == id){
      return true;
    }
  }
  return false;
}

bool Hashtag::exists(std::string name){
  std::fstream hashtags_file(Hashtag::get_general_filename(), std::fstream::in);
  Json hashtags_json;

  if(!hashtags_file.is_open()){
    return false;
  }
  else if(hashtags_file.peek() == std::fstream::traits_type::eof()){
    hashtags_file.close();
    return false;
  }
  else{
    hashtags_file >> hashtags_json;
    hashtags_file.close();
  }
  for (Json::iterator it = hashtags_json.begin(); it != hashtags_json.end(); ++it) {
    Json hashtag = *it;
    if(hashtag["name"] == name){
      return true;
    }
  }
  return false;
}
