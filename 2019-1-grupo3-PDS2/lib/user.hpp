#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "twitter_base.hpp"
#include "tweet.hpp"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace Twitter {

  class Tweet;
  class User : public TwitterBase {

    public:

      //@brief Recupera um usuário dos dados
      //@param id Um número inteiro que identifica o usuário
      User(int id);

      //@brief Cria um objeto de User sem salvá-lo nos dados
      //@param username Uma string com o nome de usuário do usuário
      //@param password Uma string com a senha do usuário
      User(std::string username, std::string password);

      //@brief Retorna o id do usuário
      //@return O número inteiro que identifica o usuário
      int get_id() override;

      //@brief Retorna o username do usuário
      //@return A string com o nome de usuário do usuário
      std::string get_username();

      //@brief Retorna os followers do usuário
      /*@return O vetor com os seguidores do usuário
      (representados por seus números identificadores)*/
      std::vector<int> get_followers();

      //@brief Retorna os followees do usuário
      /*@return O vetor com as pessoas que o usuário segue
      (representadas por seus números identificadores)*/
      std::vector<int> get_followees();

      //@brief Retorna os tweets do usuário
      /*@return O vetor com os tweets do usuário
      (representados por seus números identificadores)*/
      std::vector<int> get_tweets();

      //@brief Retorna a string do JSON do usuário
      //@param indent O número inteiro de espaços da indentação da string retornada
      //@return A string que representa a estrutura do usuário
      std::string dump(int indent) override;

      //@brief Adiciona um tweet ao vetor de tweets do usuário
      //@param tweet Um ponteiro para o tipo tweet
      void operator<<(Tweet* tweet);

      /*@brief Adiciona um tweet ao vetor de tweets do usuário
      e retorna true: se foi possível salvar o tweet ou
      false: se a adição não foi possível*/
      //@param tweet Um ponteiro para o tipo tweet
      bool push_tweet(Tweet* tweet);

      /*@brief Remove um tweet do vetor de tweets do usuário
      e retorna true: se foi possível salvar o tweet ou
      false: se a remoção não foi possível*/
      //@param tweet Um ponteiro para o tipo tweet
      bool remove_tweet(Tweet* tweet);

      /*@brief Salva um usuário nos dados e retorna true: se foi possível
      salvar o usuário ou false: se não foi possível*/
      bool save() override;

      /*@brief Adiciona um followee ao vetor de followees do usuário
      e um follower ao vetor de followers do followee e retorna
      true: se a ação foi possível ou false: se não foi possível*/
      //@param user_id Um inteiro que identifica o usuário
      bool follow(int user_id);

      /*@brief Adiciona um followee ao vetor de followees do usuário
      e um follower ao vetor de followers do followee e retorna
      true: se a ação foi possível ou false: se não foi possível*/
      //@param followee Um ponteiro para o tipo User
      bool follow(User* followee);

      /*@brief Remove um followee do vetor de followees do usuário
      e um follower do vetor de followers do followee e retorna
      true: se a ação foi possível ou false: se não foi possível*/
      //@param user_id Um inteiro que identifica o usuário
      bool unfollow(int user_id);

      /*@brief Remove um followee do vetor de followees do usuário
      e um follower do vetor de followers do followee e retorna
      true: se a ação foi possível ou false: se não foi possível*/
      //@param followee Um ponteiro para o tipo User
      bool unfollow(User* followee);

      /*@brief Deleta um usuário dos dados
      e retorna true: se a ação foi possível ou false: se não foi possível*/
      //@param password Uma string com a senha do usuário
      bool delete_account(std::string password);

      //@brief Retorna o caminho para o arquivo do usuário
      //@param id Um número inteiro que identifica o usuário
      //@return A string que representa o caminho do arquivo do usuário
      static std::string get_filename(int id);

      //@brief Retorna o caminho para o arquivo dos usuários
      //@return A string que representa o caminho do arquivo dos usuários
      static std::string get_general_filename();

      //@brief Cria o objeto usuário e salva nos dados
      //@param username Uma string com o nome de usuário do usuário
      //@param password Uma string com a senha do usuário
      //@return O ponteiro para o usuário
      static User* create(std::string username, std::string password);

      //@brief Realiza o login de um usuário
      //@param username Uma string com o nome de usuário do usuário
      //@param password Uma string com a senha do usuário
      //@return O ponteiro para o usuário
      static User* login(std::string username, std::string password);

      //@brief Retorna um usuário baseado no id
      //@param id Um número inteiro que identifica o usuário
      //@return O ponteiro para o usuário
      static User* get(int id);

      //@brief Retorna um vetor de ponteiros para usuários baseado em um vetor de ids
      //@param ids Um vetor contendo os números inteiros que identificam os usuários
      //@return O vetor de ponteiros para usuários
      static std::vector<User*> get(std::vector<int> ids);

      //@brief Pesquisa um usuário pelo username
      //@param search_term Uma string com um termo de pesquisa (nome de usuário)
      //@return O vetor de ponteiros para usuários
      static std::vector<User*> search(std::string search_term);

      //@brief Retorna um id disponível
      //@return O número inteiro que é o identificador disponível para um usuário
      static int get_available_id();

      /*@brief Verifica se o username especificado já está em uso
      e retorna true: se estiver disponível ou false: se estiver em uso*/
      //@param username Uma string com o nome de usuário do usuário
      static bool is_username_available(std::string username);

      /*@brief Verifica se um usuário existe atráves de seu id
      e retorna true: se existe ou false: se não existe*/
      //@param id Um número inteiro que identifica o usuário
      static bool exists(int id);

    private:

      //@brief Construtor vazio de User
      User();

      std::string username; ///< String com o nome de usuário do usuário
      std::string password; ///< String com a senha do usuário
      std::vector<int> followers; ///< Vetor com os seguidores do usuário (representados por seus ids)
      std::vector<int> followees; ///< Vetor com as pessoas que o usuário segue (representadas por seus ids)
      std::vector<int> tweets; ///< Vetor com os tweets do usuário (representados por seus ids)

      //@brief Altera o id do usuário
      //@param id Um número inteiro que identifica o usuário
      void set_id(int id);

      //@brief Altera o username do usuário
      //@param username Uma string com o nome de usuário do usuário
      void set_username(std::string username);

      //@brief Altera os followers do usuário
      /*@param followers Um vetor com os seguidores do usuário
      (representados por seus números identificadores)*/
      void set_followers(std::vector<int> followers);

      //@brief Altera os followees do usuário
      /*@param followees Um vetor com as pessoas que seguem o usuário
      (representadas por seus números identificadores)*/
      void set_followees(std::vector<int> followees);

      //@brief Altera os tweets do usuário
      /*@param tweets Um vetor com os tweets do usuário
      (representados por seus números identificadores)*/
      void set_tweets(std::vector<int> tweets);

      /*@brief Remove um followee do vetor de followees do usuário
      e retorna true: se a ação foi possível ou false: se não foi possível*/
      //@param user_id Um número inteiro que identifica o usuário
      bool remove_followee(int user_id);

      /*@brief Remove um follower do vetor de followers do usuário
      e retorna true: se a ação foi possível ou false: se não foi possível*/
      //@param user_id Um número inteiro que identifica o usuário
      bool remove_follower(int user_id);

      /*@brief Verifica se a password fornecida corresponde a do usuário
      e retorna true: se as senhas correspondem ou false: se não correspondem*/
      //@param password Uma string com a senha do usuário
      bool compare_password(std::string password);

      //@brief Transforma os atributos do usuário em um JSON
      //@return Um objeto da classe Json para o usuário
      Json serialize() override;

      //@brief Carrega os dados do JSON para o objeto de usuário
      //@param user_json O objeto da classe Json que representa o usuário
      void deserialize(Json user_json) override;
  };
}

#endif
