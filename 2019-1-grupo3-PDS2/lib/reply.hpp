#ifndef REPLY_H
#define REPLY_H

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include "twitter_base.hpp"
#include "nlohmann/json.hpp"
#include "user.hpp"
#include "tweet.hpp"

namespace Twitter {

  class User;
  class Tweet;
  class Reply: public TwitterBase{

    public:

      //@brief Recupera uma Reply dos dados
      //@param id Um número inteiro que identifica a reply
      Reply(int id);

      //@brief Retorna o id da reply
      //@return O número inteiro que identifica a reply
      int get_id();

      //@brief Retorna o creator_id da reply
      //@return O número inteiro que identifica o criador da reply
      int get_creator_id();

      //@brief Retorna o tweet_id do tweet ao qual a reply está vinculada
      //@return O número inteiro que identifica o tweet ao qual a reply está vinculada
      int get_tweet_id();

      //@brief Retorna o created_at da reply
      //@return O horário de criação da reply
      time_t get_created_at();

      //@brief Retorna o content da reply
      //@return A string que é o conteúdo da mensagem de reply
      std::string get_content();

      //@brief Retorna o user_likes da reply
      /*@return O vetor de usuários que curtiram a reply
      (representados por seus números identificadores)*/
      std::vector<int> get_user_likes();

      //@brief Retorna quantas curtidas teve a reply
      //@return O número inteiro que indica quantas curtidas teve a reply
      int get_like_count();

      //@brief Retorna a string do JSON da reply
      //@param indent O número inteiro de espaços da indentação da string retornada
      //@return A string que representa a estrutura da reply
      std::string dump(int indent) override;

      //@brief Adiciona um usuário ao vetor de likes da reply e lança uma exceção se a ação não for possível
      //@param user_id Um número inteiro que identifica o usuário que deseja curtir a reply
      //@return true: se a adição do usuário ao vetor foi possível
      bool like(int user_id);

      //@brief Salva uma reply nos dados
      //@return true: se a reply foi salva nos dados
      //@return false: se a reply não pôde ser salva nos dados
      bool save();

      //@brief Remove um reply dos dados
      //@param user Um ponteiro para o usuário que deletará a reply dos dados
      //@return true: se a reply foi removida dos dados
      //@return false: se a reply não pôde ser removida dos dados
      bool delete_reply(User* user);

      /*@brief Verifica se a reply existe por seu id e retorna
      true: se existir ou false: se não existir*/
      //@param id Um número inteiro que identifica a reply
      static bool exists(int id);

      //@brief Retorna um id disponível
      //@return O número inteiro que é o identificador disponível para uma reply
      static int get_available_id();

      //@brief Busca uma reply por seu id e lança uma exceção se a reply não foi encontrada nos dados
      //@param id Um número inteiro que identifica a reply
      //@return O ponteiro para a reply se ela foi encontrada
      static Reply* get(int id);

      //@brief Retorna um vetor de usuários baseado em um vetor de ids
      //@param ids Um vetor com os números inteiros que identificam as replies
      //@return O vetor de ponteiros para as replies correspondentes ao identificadores
      static std::vector<Reply*> get(std::vector<int> ids);

      //@brief Retorna um vector de replies de um determinado user
      //@param user O user criador das replies desejadas
      //@return Um vector de replies do usuário recebido
      static std::vector<Reply*> get_from_user(User* user);

      //@brief Posta uma reply criando-a e salvando-a nos dados e lança uma exceção se não foi possível postar a reply
      //@param creator Um ponteiro para o usuário criador da reply
      //@param tweet Um ponteiro para o tweet ao qual a reply está vinculada
      //@param content Uma string com o conteúdo da mensagem da reply
      //@return O ponteiro para a reply se ela foi postada
      static Reply* post_reply(User* creator, Tweet* tweet, std::string content);

      //@brief Retorna o caminho para o arquivo da reply
      //@param id Um número inteiro que identifica a reply
      //@return A string que representa o caminho do arquivo da reply
      static std::string get_filename(int id);

      //@brief Retorna o caminho para o arquivo das replies
      //@return A string que representa o caminho do arquivo das replies
      static std::string get_general_filename();

    private:
      int creator_id; ///< Número inteiro que identifica o criador da reply
      int tweet_id; ///< Número inteiro que identifica o tweet que a reply responde
      time_t created_at; ///< Variável do tipo time_t que indica o horário em que a reply foi criada
      std::string content; ///< String com o conteúdo da mensagem da reply
      std::vector<int> user_likes; ///< Vetor com os usuários que curtiram reply (representados por seus ids)

      //@brief Construtor vazio de Reply
      Reply();

      //@brief Cria um objeto de Reply sem salvá-lo nos dados
      //@param creator_id Número inteiro que identifica o criador da reply
      //@param tweet_id Número inteiro que identifica o tweet ao qual a reply está vinculada
      //@param content Uma string com o conteúdo da mensagem de reply
      Reply(int creator_id, int tweet_id, std::string content);

      //@brief Altera o id da reply
      //@param id Um número inteiro que identifica a reply
      void set_id(int id);

      //@brief Altera o creator_id da reply
      //@param creator_id Um número inteiro que identifica o criador da reply
      void set_creator_id(int creator_id);

      //@brief Altera o user_likes da reply
      /*@param user_likes Um vetor com os usuários que curtiram a reply
      (representados por seus números identificadores)*/
      void set_user_likes(std::vector<int> user_likes);

      //@brief Altera o content da reply
      //@param content Uma string com o conteúdo da mensagem de reply
      void set_content(std::string content);

      //@brief Transforma os atributos da reply em um JSON
      //@return Um objeto da classe Json para a reply
      Json serialize() override;

      //@brief Carrega os dados do JSON para o objeto de reply
      //@param reply_json O objeto da classe Json que representa a reply
      void deserialize(Json reply_json) override;
  };
}

#endif
