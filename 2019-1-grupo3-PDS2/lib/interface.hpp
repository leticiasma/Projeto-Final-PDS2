#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include "user.hpp"
#include "tweet.hpp"
#include "retweet.hpp"

namespace Twitter {

  static User* logged_user; ///< Ponteiro para o usuário que está logado no sistema

  class Interface {
    public:

      //@brief Printa o logotipo do sistema
      static void splash();

      //@brief Inicializa a interface e mostra as opções iniciais
      //@return O número inteiro que representa a opção de navegação escolhida pelo usuário
      static int initialize();

      //@brief Realiza o login do usuário
      static void login();

      //@brief Realiza o registro de um novo usuário
      static void signup();

      //@brief Mostra o menu principal do perfil de um usuário e dá opções de navegação
      static void main_menu();

      //@brief Divide uma string em linhas (substrings)
      //@param line_size Um número inteiro que representa o tamanho máximo de uma linha
      //@param content Uma string com o conteúdo de uma mensagem
      //@param O vetor com as novas strings (substrings) da string inicial que foi divida
      static std::vector<std::string> split_lines(int line_size, std::string content);

    private:

      //@brief Construtor vazio de Interface
      Interface();

      //@brief Mostra os dados de um perfil (nome, seguidores, seguindo, tweets)
      //@param user Um ponteiro para o usuário de um perfil
      static void show_data(User* user);

      //@brief Mostra um tweet
      //@paramm tweet Um ponteiro para o tweet que será exibido
      static void show_data(Tweet* tweet);

      //@brief Mostra um retweet
      //@paramm retweet Um ponteiro para o retweet que será exibido
      static void show_data(Retweet* tweet);

      //@brief Mostra uma reply
      //@param reply Um ponteiro para a reply que será exibida
      static void show_data(Reply* reply);

      //@brief Permite navegar pelos users
      //@param users Um vetor contendo os usuários
      //@return O número inteiro que representa a opção de navegação escolhida pelo usuário
      static int list(std::vector<User*> users);

      //@brief Permite navegar perlos tweets de um usuário
      //@param tweets Um vetor contendo os tweets
      //@return O número inteiro que representa a opção de navegação escolhida pelo usuário
      static int list(std::vector<Tweet*> tweets);

      //@brief Permite navegar nas replies de um tweet
      //@param replies Um vetor contendo as replies
      //@return O número inteiro que representa a opção de navegação escolhida pelo usuário
      static int list(std::vector<Reply*> replies);

      //@brief Permite navegar no perfil de um user e dá opção de seguir/deixar de seguir
      //@param user Um ponteiro para o usuário pelo qual se navega
      static int user_profile(User* user);

      //@brief Permite ver os detalhes e opções de navegação de um tweet
      //@param tweet Um ponteiro para o tweet que está sendo exibido
      static int see_tweet(Tweet* tweet);

      //@brief Exibe um retweet com seus detalhes e fornece opções de navegação
      //@param retweet Um ponteiro para o retweet exibido
      static int see_retweet(Retweet* retweet);

      //@brief Permite ver os detalhes e opções de navegação para uma reply de um tweet
      //@param reply Um ponteiro para a resposta que está sendo exibida
      static int see_reply(Reply* reply);

      //@brief Permite escrever um tweet e hashtags e postá-lo
      static void create_tweet();

      //@brief Busca por um usuário
      static void search_users();

      //@brief Permite navegar nos followers de um user
      //@param user Um vetor para o usuário do qual se quer ver os seguidores
      static void followers(User* user);

      //@brief Permite navegar nos followees de um user
      //@param user Um vetor para o usuário do qual se quer ver as pessoas que este segue
      static void followees(User* user);

      //@brief Permite navegar pelo feed do usuário
      static void feed();

      //@brief Posta um retweet e o exibe
      //@param tweet Um ponteiro para o tweet que se deseja retweetar
      static void retweet(Tweet* tweet);

      //@brief Permite escrever uma reply para um tweet
      //@param tweet Um ponteiro para o tweet que se deseja responder
      static void reply(Tweet* tweet);

      //@brief Permite ver as replies de forma geral de um tweet
      //@param tweet Um ponteiro para o tweet que se deseja ver as respostas
      static void see_replies(Tweet* tweet);

      //@brief Deleta a conta do usuário logado
      static void delete_account();
  };
}

#endif
