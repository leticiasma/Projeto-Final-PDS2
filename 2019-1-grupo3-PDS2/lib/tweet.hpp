#ifndef TWEET_H
#define TWEET_H

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "twitter_base.hpp"
#include "reply.hpp"
#include "user.hpp"

using Json = nlohmann::json;

namespace Twitter {

  class User;
  class Reply;
  class Retweet;
  class Tweet : public TwitterBase{

    friend class Retweet;

    public:

      //@brief Recupera um tweet por seu id
      //@param id Um número inteiro que identifica o tweet
      Tweet(int id);

      //@brief Retorna o id do tweet
      //@return O número inteiro que identifica o tweet
      int get_id() override;

      //@brief Retorna o id do criador do tweet
      //@return O número inteiro que identifica o criador do tweet
      int get_creator_id();

      //@brief Retorna o like_count do tweet
      //@return O número inteiro que indica quantos usuários curtiram o tweet
      int get_like_count();

      //@brief Retorna o content do tweet
      //@return A string com o conteúdo da mensagem do tweet
      std::string get_content();

      //@brief Retorna as hashtags do tweet
      //@return O vetor de strings com as hashtags do tweet
      std::vector<std::string> get_hashtags();

      //@brief Retorna os user_likes do tweet
      /*@return O vetor com os usuários que curtiram o tweet
      (representados por seus números identificadores)*/
      std::vector<int> get_user_likes();

      //@brief Retorna as replies do tweet
      /*@return O vetor com as respostas do tweet
      (representadas por seus números identificadores)*/
      std::vector<int> get_replies();

      //@brief Retorna o created_at do tweet
      //@return O horário de criação do tweet
      time_t get_created_at();

      //@brief Retorna a string do JSON do tweet
      //@param indent O número inteiro de espaços da indentação da string retornada
      //@return A string que representa a estrutura do tweet
      std::string dump(int indent) override;

      //@brief Adiciona uma resposta ao vetor de respostas do tweet
      //@param Um ponteiro para o tipo Reply
      void add_reply(Reply* reply);

      /*@brief Remove um reply do vetor de replies do tweet
      e retorna true: se foi possível salvar o tweet ou
      false: se a remoção não foi possível*/
      //@param reply Um ponteiro para o tipo Reply
      bool remove_reply(Reply* reply);

      //@brief Salva o tweet nos dados
      //@return true: se o tweet foi salvo nos dados
      //@return false: se o tweet não pôde ser salvo nos dados
      bool save();

      //@brief Adiciona um usuário ao vetor de user_likes do tweet
      //@param user_id Um número inteiro que identifica o usuário que irá curtir o tweet
      //@return true: se a ação foi realizada com sucesso
      //@return false: se não foi possível realizar a ação
      bool like_tweet(int user_id);

      //@brief Remove um usuário do vetor de user_likes do tweet
      //@param user_id Um número inteiro que identifica o usuário que irá descurtir o tweet
      //@return true: se a ação foi realizada com sucesso
      //@return false: se não foi possível realizar a ação
      bool dislike_tweet(int user_id);

      //@brief Deleta um tweet dos dados e retorna
      //@param user Um ponteiro para um user
      //@return true: se o tweet foi deletado dos dados
      //@return false: se o tweet não pôde ser deletado dos dados
      bool delete_tweet(User* user);

      //@brief Indica que não é um retweet (retornando false)
      bool virtual is_retweet() { return false; }

      //@brief Verifica se um tweet existe através de seu id
      //@param id Um número inteiro que identifica o tweet
      //@return true: se o tweet existe nos dados
      //@return false: se o tweet não existe nos dados
      static bool exists(int id);

      //@brief Retorna um id disponível
      //@return O número inteiro que é o identificador disponível para um tweet
      static int get_available_id();

      //@brief Posta um tweet criando-o, adicionando-o ao vetor de tweets do usuário e salvando o tweet nos dados
      //@param creator_id Um número inteiro que identifica o criador do tweet
      //@param content Uma string com o conteúdo da mensagem do tweet
      //@param hashtags Um vetor de strings com as hashtags do tweet
      //@return O ponteiro para o tipo Tweet se o tweet foi postado
      //@return nullptr se o tweet não pôde ser postado
      static Tweet* post_tweet(int creator_id, std::string content, std::vector<std::string> hashtags);

      //@brief Posta um tweet criando-o, adicionando-o ao vetor de tweets do usuário e salvando o tweet nos dados
      //@param creator Um ponteiro para o User criador do tweet
      //@param content Uma string com o conteúdo da mensagem do tweet
      //@param hashtags Um vetor de strings com as hashtags do tweet
      //@return O ponteiro para o tipo Tweet se o tweet foi postado
      //@return nullptr se o tweet não pôde ser postado
      static Tweet* post_tweet(User* creator, std::string content, std::vector<std::string> hashtags);

      //@brief Busca um tweet nos dados por seu id e lança uma exceção se ele não for encontrado
      //@param id Um número inteiro que identifica o tweet
      //@return O ponteiro para o tweet
      static Tweet* get(int id);

      //@brief Retorna um vetor de tweets baseado em um vetor de ids
      //@param ids Um vetor com os números inteiros que identificam os tweets
      //@return O vetor de ponteiros para os tweets correspondentes aos identificadores
      static std::vector<Tweet*> get(std::vector<int> ids);

      //@brief Retorna o caminho para o arquivo do tweet
      //@param id Um número inteiro que identifica o tweet
      //@return A string que representa o caminho do arquivo do tweet
      static std::string get_filename(int id);

      //@brief Retorna o caminho para o arquivo dos tweets
      //@return A string que representa o caminho do arquivo dos tweets
      static std::string get_general_filename();

      /*@brief Compara o horário de criação de dois tweets e retorna
      true: se o tweet "a" foi criado antes de "b" ou false: se não*/
      //@param a Um ponteiro para um tweet
      //@param b Um ponteiro para outro tweet
      static bool compare_time(Tweet* a, Tweet* b);

    protected:
      int creator_id; ///< Número inteiro que identifica o criador do tweet
      int like_count; ///< Número inteiro que indica quantas pessoas curtiram o tweet
      std::string content; ///< String com o conteúdo da mensagem do tweet
      std::vector<std::string> hashtags; ///< Vetor de strings com as hashtags do tweet
      std::vector<int> user_likes; ///< Vetor com os usuários que curtiram o tweet (representados por seus ids)
      std::vector<int> replies; ///< Vetor com as respostas do tweet (representadas por seus ids)
      time_t created_at; ///< Variável do tipo time_t que indica o horário que o tweet foi criado

      //@brief Construtor vazio de Tweet
      Tweet();

      //@brief Cria um objeto de Tweet sem salvá-lo nos dados
      //@param creator_id Um número inteiro que identifica o criador do tweet
      //@param content Uma string com o conteúdo da mensagem do tweet
      //@param hashtags Um vetor de strings com as hashtags do tweet
      Tweet(int creator_id, std::string content, std::vector<std::string> hashtags);

      //@brief ALtera o id do tweet
      //@param id Um número inteiro que identifica o tweet
      void set_id(int id);

      //@brief Altera o id do criador do tweet
      //@param creator_id Um número inteiro que identifica o criador do tweet
      void set_creator_id(int creator_id);

      //@brief Altera o like_count do tweet
      void set_like_count();

      //@brief Altera o content do tweet
      //@param content Uma string com o conteúdo da mensagem de tweet
      void set_content(std::string content);

      //@brief Altera as hashtags do tweet
      //@param hashtags Um vetor de strings com as hashtags do tweet
      void set_hashtags(std::vector<std::string> hashtags);

      //@brief Altera os user_likes do tweet
      void set_user_likes(std::vector<int> user_likes);

      //@brief Altera as replies do tweet
      /*@param replies Um vetor as respostas do tweet
      (representadas por seus números identificadores)*/
      void set_replies(std::vector<int> replies);

      //@brief Transforma os atributos do Tweet em um JSON
      //@return Um objeto da classe Json para o tweet
      Json virtual serialize() override;

      //@brief Carrega os dados do JSON para o objeto de Tweet
      //@param tweet_json O objeto da classe Json que representa o tweet
      void virtual deserialize(Json tweet_json) override;

  };
}

#endif
