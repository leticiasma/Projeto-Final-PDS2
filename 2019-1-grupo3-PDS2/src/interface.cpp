#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "../lib/interface.hpp"
#include "../lib/user.hpp"
#include "../lib/tweet.hpp"
#include "../lib/retweet.hpp"

// Exceptions
#include "../lib/exceptions/unexistent_user_exception.hpp"
#include "../lib/exceptions/unexistent_tweet_exception.hpp"
#include "../lib/exceptions/invalid_action_exception.hpp"
#include "../lib/exceptions/forbidden_action_exception.hpp"
#include "../lib/exceptions/resource_not_found_exception.hpp"

using Interface = Twitter::Interface;
using Tweet = Twitter::Tweet;
using Retweet = Twitter::Retweet;
using User = Twitter::User;

Interface::Interface(){

}

void Interface::splash(){
  std::cout << std::endl << std::endl;
  std::cout << "      ___           ___                       ___           ___           ___           ___      " << std::endl;
  std::cout << "     /\\  \\         /\\__\\          ___        /\\  \\         /\\  \\         /\\  \\         /\\  \\     " << std::endl;
  std::cout << "     \\:\\  \\       /:/ _/_        /\\  \\       \\:\\  \\        \\:\\  \\       /::\\  \\       /::\\  \\    " << std::endl;
  std::cout << "      \\:\\  \\     /:/ /\\__\\       \\:\\  \\       \\:\\  \\        \\:\\  \\     /:/\\:\\  \\     /:/\\:\\  \\   " << std::endl;
  std::cout << "      /::\\  \\   /:/ /:/ _/_      /::\\__\\      /::\\  \\       /::\\  \\   /::\\~\\:\\  \\   /::\\~\\:\\  \\  " << std::endl;
  std::cout << "     /:/\\:\\__\\ /:/_/:/ /\\__\\  __/:/\\/__/     /:/\\:\\__\\     /:/\\:\\__\\ /:/\\:\\ \\:\\__\\ /:/\\:\\ \\:\\__\\" << std::endl;
  std::cout << "    /:/  \\/__/ \\:\\/:/ /:/  / /\\/:/  /       /:/  \\/__/    /:/  \\/__/ \\:\\~\\:\\ \\/__/ \\/_|::\\/:/  / " << std::endl;
  std::cout << "   /:/  /       \\::/_/:/  /  \\::/__/       /:/  /        /:/  /       \\:\\ \\:\\__\\      |:|::/  /  " << std::endl;
  std::cout << "   \\/__/         \\:\\/:/  /    \\:\\__\\       \\/__/         \\/__/         \\:\\ \\/__/      |:|\\/__/   " << std::endl;
  std::cout << "                  \\::/  /      \\/__/                                    \\:\\__\\        |:|  |     " << std::endl;
  std::cout << "                   \\/__/                                                 \\/__/         \\|__|     " << std::endl;

  std::cout << std::endl << std::endl << "Gerado por: http://patorjk.com/software/taag" << std::endl << std::endl;
}

int Interface::initialize(){
  int op = -1;
  while (op != 0 && op != 1 && op != 2) {
    std::cout << "0 - Sair" << std::endl;
    std::cout << "1 - Login" << std::endl;
    std::cout << "2 - Registrar" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;
  }
  switch (op) {
    case 0:
      return 0;
    case 1:
      try{
        Twitter::Interface::login();
        Twitter::Interface::main_menu();
      }
      catch(InvalidActionException e){
        std::cout << std::endl << std::endl << e.what() << std::endl << std::endl;
      }
      catch(...){
        std::cout << std::endl << std::endl << "Um erro inesperado aconteceu" << std::endl << std::endl;
      }
      break;
    case 2:
      try{
        Twitter::Interface::signup();
        Twitter::Interface::main_menu();
      }
      catch(InvalidActionException e){
        std::cout << std::endl << std::endl << e.what() << std::endl << std::endl;
      }
      catch(...){
        std::cout << std::endl << std::endl << "Um erro inesperado aconteceu" << std::endl << std::endl;
      }
      break;
    default:
      return 0;
  }
  return op;
}

void Interface::login(){
  User* user = nullptr;
  std::string login;
  std::string password;
  Twitter::logged_user = nullptr;

  std::cout << "Digite seu login e senha..." << std::endl;
  std::cout << "Lgin: ";
  std::cin >> login;
  std::cout << "Senha: ";
  std::cin >> password;

  user = User::login(login, password);
  if(user == nullptr){
    throw InvalidActionException("User::login", "Login ou senha incorretos");
  }
  else{
    Twitter::logged_user = user;
  }
}

void Interface::signup(){
  User* user;
  std::string login;
  std::string password;
  std::string password_confirm;
  Twitter::logged_user = nullptr;

  std::cout << "Novo login: ";
  std::cin >> login;
  if(User::is_username_available(login) == false){
    throw InvalidActionException("User::signup", "O login inserido já está em uso");
  }

  std::cout << "Nova senha: ";
  std::cin >> password;
  std::cout << "Repetir senha: ";
  std::cin >> password_confirm;
  if(password != password_confirm){
    std::cout << std::endl << "As senhas não coincidem..." << std::endl << std::endl;
    throw InvalidActionException("User::signup", "As senhas não coincidem");
  }

  try{
    user = User::create(login, password);
    Twitter::logged_user = user;
  }
  catch(InvalidActionException e){
    throw e;
  }
  catch(ResourceNotFoundException e){
    throw e;
  }
}

void Interface::main_menu(){
  int op = -1;

  std::cout << '\n' << "\t\t\tBem vindo, " << Twitter::logged_user->get_username() << '!' << std::endl;
  while(op != 0){
    std::cout << std::endl << "\t\t\tMENU PRINCIPAL" << std::endl << std::endl;
    std::cout << "0 - Sair" << std::endl;
    std::cout << "1 - Ver perfil" << std::endl;
    std::cout << "2 - Novo tweet" << std::endl;
    std::cout << "3 - Ver feed" << std::endl;
    std::cout << "4 - Buscar usuário" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    switch (op) {
      case 1:
        if(Interface::user_profile(Twitter::logged_user) == -1){
          op = 0;
        }
        break;
      case 4:
        Interface::search_users();
        break;
      case 3:
        Interface::feed();
        break;
      case 2:
        Interface::create_tweet();
        break;
      default:
        op = 0;
        break;
    }
  }
}

void Interface::show_data(User* user){
  std::cout << '+' << std::setw(35) << std::setfill('-') << '+' << std::endl;
  std::cout << std::setw(15) << std::setfill(' ') << std::left << "|Nome" << '|' << std::setw(19) << std::right << user->get_username() << '|' << std::endl;
  std::cout << std::setw(15) << std::setfill(' ') << std::left << "|Seguidores" << '|' << std::setw(19) << std::right << user->get_followers().size() << '|' << std::endl;
  std::cout << std::setw(15) << std::setfill(' ') << std::left << "|Seguindo" << '|' << std::setw(19) << std::right << user->get_followees().size() << '|' << std::endl;
  std::cout << std::setw(15) << std::setfill(' ') << std::left << "|Tweets" << '|' << std::setw(19) << std::right << user->get_tweets().size() << '|' << std::endl;
  std::cout << '+' << std::setw(35) << std::setfill('-') << '+' << std::endl << std::endl;
}

void Interface::show_data(Tweet* tweet){
  User* creator = User::get(tweet->get_creator_id());
  std::vector<std::string> hashtags = tweet->get_hashtags();
  const int line_size = 69;
  std::vector<std::string> content_lines = Interface::split_lines(line_size, tweet->get_content());

  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  std::cout << "|Por " << std::setw(65) << std::setfill(' ') << std::left << creator->get_username() << '|' << std::endl;
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  for (std::vector<std::string>::iterator it = content_lines.begin(); it != content_lines.end(); ++it) {
    std::cout << '|' << std::setw(69) << std::setfill(' ') << std::left << *it << '|' << std::endl;
  }
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  for (std::vector<std::string>::iterator it = hashtags.begin(); it != hashtags.end(); ++it) {
    std::cout << "#" << *it << ' ';
  }
  if(hashtags.size() > 0){
    std::cout << std::endl;
  }

  delete creator;
}

void Interface::show_data(Retweet* tweet){
  Tweet* original_tweet;
  try{
    original_tweet = Tweet::get(tweet->get_original_id());
  }
  catch(UnexistentTweetException e){
    return;
  }
  User* creator = User::get(tweet->get_creator_id());
  User* original_creator = User::get(original_tweet->get_creator_id());
  std::vector<std::string> hashtags = tweet->get_hashtags();
  const int line_size = 69;
  std::vector<std::string> content_lines = Interface::split_lines(line_size, tweet->get_content());

  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  std::cout << "|Por " << std::setw(65) << std::setfill(' ') << std::left << creator->get_username() << '|' << std::endl;
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  std::cout << "|Retweet de " << std::setw(58) << std::setfill(' ') << std::left << original_creator->get_username() << '|' << std::endl;
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  for (std::vector<std::string>::iterator it = content_lines.begin(); it != content_lines.end(); ++it) {
    std::cout << '|' << std::setw(69) << std::setfill(' ') << std::left << *it << '|' << std::endl;
  }
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  for (std::vector<std::string>::iterator it = hashtags.begin(); it != hashtags.end(); ++it) {
    std::cout << "#" << *it << ' ';
  }
  if(hashtags.size() > 0){
    std::cout << std::endl;
  }
  delete original_tweet;
  delete creator;
  delete original_creator;
}

void Interface::show_data(Reply* reply){
  Tweet* tweet;
  try{
    tweet = Tweet::get(reply->get_tweet_id());
  }
  catch(UnexistentTweetException e){
    return;
  }
  User* creator = User::get(reply->get_creator_id());
  User* tweet_creator = User::get(tweet->get_creator_id());
  const int line_size = 69;
  std::vector<std::string> content_lines = Interface::split_lines(line_size, reply->get_content());

  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  std::cout << "|Por " << std::setw(65) << std::setfill(' ') << std::left << creator->get_username()+" em resposta a "+tweet_creator->get_username() << '|' << std::endl;
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;
  for (std::vector<std::string>::iterator it = content_lines.begin(); it != content_lines.end(); ++it) {
    std::cout << '|' << std::setw(69) << std::setfill(' ') << std::left << *it << '|' << std::endl;
  }
  std::cout << '+' << std::setw(70) << std::setfill('-') << std::right << '+' << std::endl;

  delete tweet;
  delete creator;
  delete tweet_creator;
}

int Interface::list(std::vector<User*> users){
  int i, op;
  while(true){
    i = 1;
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it, i++) {
      User* user = *it;
      std::cout << std::endl << i << " ) " << user->get_username();
    }
    std::cout << std::endl << std::endl;
    std::cout << "0 - Voltar" << std::endl;
    std::cout << "# - Ver usuário" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    if(op > 0 && op < i){
      Interface::user_profile(users.at(op-1));
    }
    else{
      return op;
    }
  }
}

int Interface::list(std::vector<Tweet*> tweets){
  int i, op;
  while(true){
    i = 1;
    for (std::vector<Tweet*>::iterator it = tweets.begin(); it != tweets.end(); ++it, i++) {
      Tweet* tweet = *it;
      std::cout << std::endl << i << " ) " << std::endl;
      if(tweet->is_retweet()){
        Interface::show_data(dynamic_cast<Retweet*>(tweet));
      }
      else{
        Interface::show_data(tweet);
      }
      std::cout << std::endl << "Curtidas: " << tweet->get_like_count() << "\t\tRespostas: " << tweet->get_replies().size() << "\t\tRetweets: 0";
      std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "0 - Voltar" << std::endl;
    std::cout << "# - Ver tweet" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    if(op > 0 && op < i){
      if(tweets.at(op-1)->is_retweet()){
        if(Interface::see_retweet(dynamic_cast<Retweet*>(tweets.at(op-1))) == -1){
          return 0;
        }
      }
      else{
        if(Interface::see_tweet(tweets.at(op-1)) == -1){
          return 0;
        }
      }
    }
    else{
      return op;
    }
  }
}

int Interface::list(std::vector<Reply*> replies){
  int i, op;
  while(true){
    i = 1;
    for (std::vector<Reply*>::iterator it = replies.begin(); it != replies.end(); ++it, i++) {
      Reply* reply = *it;
      std::cout << std::endl << i << " ) " << std::endl;
      Interface::show_data(reply);
      std::cout << std::endl << "Curtidas: " << reply->get_like_count() << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "0 - Voltar" << std::endl;
    std::cout << "# - Ver resposta" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    if(op > 0 && op < i){
      if(Interface::see_reply(replies.at(op-1)) == -1){
        return 0;
      }
    }
    else{
      return op;
    }
  }
}

int Interface::user_profile(User* user){
  int op = -1;
  bool follows = false;
  const bool foreign = (Twitter::logged_user->get_id() != user->get_id());
  std::vector<Tweet*> tweets = Tweet::get(user->get_tweets());
  std::vector<int> followers = user->get_followers();

  if(foreign){
    follows = std::find(followers.begin(), followers.end(), Twitter::logged_user->get_id()) != followers.end();
  }
  while (op != 0) {
    if(foreign) std::cout << std::endl << "\t\t\tPERFIL DE " << user->get_username() << std::endl << std::endl;
    else std::cout << std::endl << "\t\t\tSEU PERFIL" << std::endl << std::endl;
    Interface::show_data(user);

    std::cout << "0 - Voltar" << std::endl;
    std::cout << "1 - Ver seguidores" << std::endl;
    std::cout << "2 - Ver seguidos" << std::endl;
    std::cout << "3 - Ver tweets" << std::endl;
    if(foreign){
      if(follows) std::cout << "4 - Deixar de seguir" << std::endl;
      else std::cout << "4 - Seguir" << std::endl;
    }
    else{
      std::cout << "4 - Deletar conta" << std::endl;
    }
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    switch (op) {
      case 1:
        Interface::followers(user);
        break;
      case 2:
        Interface::followees(user);
        break;
      case 3:
        Interface::list(tweets);
        break;
      case 4:
        if(foreign){
          if(follows){
            try {
              Twitter::logged_user->unfollow(user);
              follows = false;
            }
            catch (UnexistentUserException e){
              return 0;
            }
            catch(InvalidActionException e){
              if(Twitter::logged_user->get_id() != user->get_id()){
                follows = false;
              }
            }
            catch(...){
              std::cout << "Um erro inesperado aconteceu" << std::endl;
            }
          }
          else{
            try{
              Twitter::logged_user->follow(user);
              follows = true;
            }
            catch (UnexistentUserException e){
              return 0;
            }
            catch(InvalidActionException e){
              if(Twitter::logged_user->get_id() != user->get_id()){
                follows = true;
              }
            }
            catch(...){
              std::cout << "Um erro inesperado aconteceu" << std::endl;
            }
          }
        }
        else{
          try{
            Interface::delete_account();
            return -1;
          }
          catch(ForbiddenActionException e){
            std::cout << "Senha incorreta" << std::endl;
          }
          catch(...){
            std::cout << "Um erro inesperado aconteceu" << std::endl;
          }
        }
        break;
      default:
        op = 0;
        break;
    }
  }
  for (std::vector<Tweet*>::iterator it = tweets.begin(); it != tweets.end(); ++it) {
    delete *it;
  }
  return 0;
}

int Interface::see_tweet(Tweet* tweet){
  int op = -1;
  User* creator = User::get(tweet->get_creator_id());
  std::vector<int> user_likes =  tweet->get_user_likes();
  bool is_owner = (creator->get_id() == Twitter::logged_user->get_id());
  bool likes = std::find(user_likes.begin(), user_likes.end(), Twitter::logged_user->get_id()) != user_likes.end();

  while (op != 0) {
    std::cout << std::endl << std::endl << "\t\t\tTWEET" << std::endl;
    Interface::show_data(tweet);
    std::cout << std::endl << "Curtidas: " << tweet->get_like_count() << "\t\tRespostas: " << tweet->get_replies().size() << "\t\tRetweets: 0";
    std::cout << std::endl << std::endl;

    std::cout << std::endl << std::endl << "0 - Voltar" << std::endl;
    if(likes){
      std::cout << "1 - Descurtir" << std::endl;
    }
    else{
      std::cout << "1 - Curtir" << std::endl;
    }
    std::cout << "2 - Responder" << std::endl;
    std::cout << "3 - Ver respostas" << std::endl;
    std::cout << "4 - Retweet" << std::endl;
    std::cout << "5 - Ver criador" << std::endl;
    if(is_owner) std::cout << "6 - Deletar" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    switch (op) {
      case 1:
        if(likes){
          try{
            likes = !tweet->dislike_tweet(Twitter::logged_user->get_id());
          }
          catch(...){}
        }
        else{
          try{
            likes = tweet->like_tweet(Twitter::logged_user->get_id());
          }
          catch(InvalidActionException e){
            likes = true;
          }
          catch(...){}
        }
        break;
      case 2:
        Interface::reply(tweet);
        break;
      case 3:
        Interface::see_replies(tweet);
        break;
      case 4:
        Interface::retweet(tweet);
        break;
      case 5:
        Interface::user_profile(creator);
        break;
      case 6:
        if(is_owner){
          try{
            tweet->delete_tweet(Twitter::logged_user);
            delete creator;
            return -1;
          }
          catch(ForbiddenActionException e){
            is_owner = false;
            std::cout << std::endl << "Permissão negada" << std::endl << std::endl;
          }
          catch(...){
            std::cout << std::endl << "Um erro inesperado aconteceu." << std::endl << std::endl;
          }
        }
        else{
          op = 0;
        }
        break;
      default:
        op = 0;
        break;
    }
  }
  delete creator;
  return 0;
}

int Interface::see_retweet(Retweet* retweet){
  int op = -1;
  Tweet* original_tweet;
  try{
    original_tweet = Tweet::get(retweet->get_original_id());
  }
  catch(UnexistentTweetException e){
    return -1;
  }
  User* creator = User::get(retweet->get_creator_id());
  bool is_owner = (creator->get_id() == Twitter::logged_user->get_id());
  std::vector<int> user_likes = retweet->get_user_likes();
  bool likes = (std::find(user_likes.begin(), user_likes.end(), Twitter::logged_user->get_id()) != user_likes.end());

  while (op != 0) {
    std::cout << std::endl << std::endl << "\t\t\tTWEET" << std::endl;
    Interface::show_data(retweet);
    std::cout << std::endl << "Curtidas: " << retweet->get_like_count() << "\t\tRespostas: " << retweet->get_replies().size() << "\t\tRetweets: 0";
    std::cout << std::endl << std::endl;

    std::cout << std::endl << std::endl << "0 - Voltar" << std::endl;
    if(likes){
      std::cout << "1 - Descurtir" << std::endl;
    }
    else{
      std::cout << "1 - Curtir" << std::endl;
    }
    std::cout << "2 - Responder" << std::endl;
    std::cout << "3 - Ver respostas" << std::endl;
    std::cout << "4 - Retweet" << std::endl;
    std::cout << "5 - Ver criador" << std::endl;
    std::cout << "6 - Ver tweet original" << std::endl;
    if(is_owner) std::cout << "7 - Deletar" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    switch (op) {
      case 1:
        if(likes){
          try{
            likes = !retweet->dislike_tweet(Twitter::logged_user->get_id());
          }
          catch(...){}
        }
        else{
          try{
            likes = retweet->like_tweet(Twitter::logged_user->get_id());
          }
          catch(InvalidActionException e){
            likes = true;
          }
          catch(...){}
        }
        break;
      case 2:
        Interface::reply(retweet);
        break;
      case 3:
        Interface::see_replies(retweet);
        break;
      case 4:
        Interface::retweet(retweet);
        break;
      case 5:
        Interface::user_profile(creator);
        break;
      case 6:
        if(original_tweet->is_retweet()){
          Interface::see_retweet(dynamic_cast<Retweet*>(original_tweet));
        }
        else{
          Interface::see_tweet(original_tweet);
        }
        break;
      case 7:
        if(is_owner){
          try{
            retweet->delete_tweet(Twitter::logged_user);
            delete creator;
            delete original_tweet;
            return -1;
          }
          catch(ForbiddenActionException e){
            is_owner = false;
            std::cout << std::endl << "Permissão negada" << std::endl << std::endl;
          }
          catch(...){
            std::cout << std::endl << "Um erro inesperado aconteceu." << std::endl << std::endl;
          }
        }
        else{
          op = 0;
        }
        break;
      default:
        op = 0;
        break;
    }
  }
  delete creator;
  delete original_tweet;
  return 0;
}

int Interface::see_reply(Reply* reply){
  int op = -1;
  Tweet* tweet;
  try{
    tweet = Tweet::get(reply->get_tweet_id());
  }
  catch(UnexistentTweetException e){
    return -1;
  }
  User* creator = User::get(reply->get_creator_id());
  bool is_owner = (creator->get_id() == Twitter::logged_user->get_id());
  std::vector<int> user_likes = reply->get_user_likes();
  bool likes = std::find(user_likes.begin(), user_likes.end(), Twitter::logged_user->get_id()) != user_likes.end();

  while(op!=0){
    std::cout << std::endl << std::endl << "EM RESPOSTA A" << std::endl << std::endl;
    Interface::show_data(tweet);

    std::cout << std::endl << creator->get_username() << " ESCREVEU:" << std::endl << std::endl;
    Interface::show_data(reply);
    std::cout << std::endl << "Curtidas: " << reply->get_like_count() << std::endl << std::endl;

    std::cout << std::endl << std::endl << "0 - Voltar" << std::endl;
    if(likes){
      std::cout << "1 - Descurtir" << std::endl;
    }
    else{
      std::cout << "1 - Curtir" << std::endl;
    }
    std::cout << "2 - Ver criador" << std::endl;
    if(is_owner) std::cout << "3 - Deletar" << std::endl;
    std::cout << std::endl << "Opção: ";
    std::cin >> op;
    std::cout << std::endl;

    switch (op) {
      case 1:
        if(likes){

        }
        else{
          likes = reply->like(Twitter::logged_user->get_id());
        }
        break;
      case 2:
        Interface::user_profile(creator);
        break;
      case 3:
        if(is_owner){
          try{
            reply->delete_reply(Twitter::logged_user);
            return -1;
          }
          catch(ForbiddenActionException e){
            is_owner = false;
          }
          catch(...){
            return 0;
          }
        }
        break;
      default:
        op = 0;
        break;
    }
  }
  delete tweet;
  return 0;
}

void Interface::create_tweet(){
  Tweet* tweet;
  std::string content;
  std::string hashtags_line;
  std::string hashtag;
  std::vector<std::string> hashtags;

  std::cout << "\t\t\tPOSTAR TWEET" << std::endl << std::endl;
  std::cout << "Texto: ";
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, content);
  std::cout << "Hashtags (separadas por espaço): ";
  std::getline(std::cin, hashtags_line);

  std::istringstream string_stream(hashtags_line);
  while (string_stream >> hashtag) {
    hashtags.push_back(hashtag);
  }

  tweet = Tweet::post_tweet(Twitter::logged_user, content, hashtags);
  Interface::see_tweet(tweet);
  delete tweet;
}

void Interface::search_users(){
  std::vector<User*> users;
  std::string search_term;
  int op = -1;

  std::cout << std::endl << "\t\t\tBUSCAR USUÁRIO" << std::endl << std::endl;
  std::cout << "Buscar: ";
  std::cin >> search_term;
  std::cout << std::endl;

  users = User::search(search_term);
  op = Interface::list(users);
  for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
    delete *it;
  }
}

void Interface::followers(User* user){
  std::vector<User*> followers;
  int op = -1;

  followers = User::get(user->get_followers());
  while(op != 0){
    std::cout << std::endl << "\t\t\tSEGUIDORES DE " << user->get_username() << std::endl << std::endl;
    op = Interface::list(followers);
  }
  for (std::vector<User*>::iterator it = followers.begin(); it != followers.end(); ++it) {
    delete *it;
  }
}

void Interface::followees(User* user){
  std::vector<User*> followees;
  int op = -1;

  followees = User::get(user->get_followees());
  while(op != 0){
    std::cout << std::endl << "\t\t\tSEGUIDOS POR " << user->get_username() << std::endl << std::endl;
    op = Interface::list(followees);
  }
  for (std::vector<User*>::iterator it = followees.begin(); it != followees.end(); ++it) {
    delete *it;
  }
}

void Interface::feed(){
  std::vector<User*> followees = User::get(Twitter::logged_user->get_followees());
  std::vector<Tweet*> tweets;
  std::vector<int> tweets_ids = Twitter::logged_user->get_tweets();

  for (std::vector<User*>::iterator it = followees.begin(); it != followees.end(); ++it) {
    std::vector<int> user_tweets = (*it)->get_tweets();
    tweets_ids.insert(tweets_ids.end(), user_tweets.begin(), user_tweets.end());
    delete *it;
  }
  tweets = Tweet::get(tweets_ids);
  std::sort(tweets.begin(), tweets.end(), Tweet::compare_time);
  std::cout << std::endl << std::endl << "\t\t\tFEED" << std::endl;
  Interface::list(tweets);
  for (std::vector<Tweet*>::iterator it = tweets.begin(); it != tweets.end(); ++it) {
    delete *it;
  }
}

void Interface::retweet(Tweet* tweet){
  Retweet* retweet = Retweet::post_tweet(Twitter::logged_user, tweet);
  Interface::see_retweet(retweet);
  delete retweet;
}

void Interface::reply(Tweet* tweet){
  Reply* reply;
  std::string content;

  std::cout << "\t\t\tESCREVER RESPOSTA" << std::endl << std::endl;
  Interface::show_data(tweet);
  std::cout << std::endl << "Texto: ";
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, content);

  reply = Reply::post_reply(Twitter::logged_user, tweet, content);
  Interface::see_reply(reply);
  delete reply;
}

void Interface::see_replies(Tweet* tweet){
  User* creator = User::get(tweet->get_creator_id());
  std::vector<Reply*> replies = Reply::get(tweet->get_replies());

  std::cout << std::endl << "\t\t\tRESPOSTAS A " << creator->get_username() << std::endl << std::endl;
  Interface::list(replies);
  delete creator;
  for (std::vector<Reply*>::iterator it = replies.begin(); it != replies.end(); ++it) {
    delete *it;
  }
}

void Interface::delete_account(){
  std::string password;
  std::cout << std::endl << std::endl << "Digite sua senha para confirmar: ";
  std::cin >> password;
  std::cout << std::endl;

  try{
    if(Twitter::logged_user->delete_account(password)){
      delete Twitter::logged_user;
    }
    else{
      throw InvalidActionException("User::delete_account", "Unknown");
    }
  }
  catch(ForbiddenActionException e){
    throw e;
  }
  catch(...){
    throw InvalidActionException("User::delete_account", "Unknown");
  }
}

std::vector<std::string> Interface::split_lines(int line_size, std::string content){
  std::vector<std::string> content_lines;
  int read = 0, current_line;

  for(int i = 0; read+line_size < content.size(); i++) {
    current_line = line_size;
    while (content.at(read+current_line) != ' ' && current_line > 0) {
      current_line--;
    }
    if(current_line > 0){
      content_lines.push_back(content.substr(read, current_line));
      read += current_line+1;
    }
    else{
      current_line = line_size-1;
      content_lines.push_back(content.substr(read, current_line)+'-');
      read += current_line;
    }
  }
  content_lines.push_back(content.substr(read, content.size()-read));
  return content_lines;
}
