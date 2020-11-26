#ifndef HASHTAG_H
#define HASHTAG_H

#include <string>
#include <vector>
#include "twitter_base.hpp"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace Twitter {

  class Hashtag : public TwitterBase{

    public:

      //@brief Recupera uma Hashtag dos dados
      //@param id Um número inteiro que identifica a hashtag
      Hashtag(int id);

      //@brief Cria um objeto de Hashtag sem salvá-lo nos dados
      //@param name Uma string com o nome da hashtag
      Hashtag(std::string name);

      //@brief Retorna o id da hashtag
      //@return O número inteiro que identifica a hashtag
      int get_id();

      //@brief Retorna o use_count da hashtag
      //@return O número inteiro que indica quantas vezes a hashtag foi usada
      int get_use_count();

      //@brief Retorna o name da hashtag
      //@return A string que é o nome da hashtag
      std::string get_name();

      //@brief Retorna os tweets que usam a hashtag
      /*@return O vetor com os tweets que usam a hashtag
      (representados por seus números identificadores)*/
      std::vector<int> get_tweets();

      //@brief Retorna a string do JSON da hashtag
      //@param indent O número inteiro de espaços da indentação da string retornada
      //@return A string que representa a estrutura da hashtag
      std::string dump(int indent);

      //@brief Incrementa o use_count da hashtag
      void increment_use_count();

      /*@brief Salva uma hashtag nos dados e retorna true: se a hashtag
      pôde ser salva ou false: se não foi salva pela função*/
      bool save();

      //@brief Retorna o caminho para o arquivo da hashtag
      //@param id Um número inteiro que identifica a hashtag
      //@return A string que representa o caminho do arquivo da hashtag
      static std::string get_filename(int id);

      //@brief Retorna o caminho para os arquivos das hashtags
      //@return A string que representa o caminho do arquivo das hashtags
      static std::string get_general_filename();

      /*@brief Cria uma hashtag e retorna um ponteiro para a hashtag
      (se foi criada na função) ou um nullptr (se já existia)*/
      //@param name Uma string com o nome da hashtag_
      //@return O ponteiro para o tipo Hashtag
      static Hashtag* create(std::string name);

      /*@brief Busca uma hashtag pelo seu id e retona um ponteiro para
      a hashtag se existir e um nullptr se não for encontrada*/
      //@param id Número inteiro que representa a hashtag
      //@return O ponteiro para o tipo Hashtag
      static Hashtag* get(int id);

      /*@brief Busca uma hashtag por seu name e retona um ponteiro para
      a hashtag se existir e um nullptr se não for encontrada*/
      //@param name Uma string com o nome da hashtag
      //@return O ponteiro para o tipo Hashtag
      static Hashtag* get(std::string name);

      //@brief Retorna um vetor de hashtags baseado em um vetor de ids
      //@param ids Um vetor com os números inteiros que identificam as hashtags
      //@return O vetor de ponteiros para as hashtags correspondentes aos identificadores
      static std::vector<Hashtag*> get(std::vector<int> ids);

      //@brief Retorna um id disponível
      //@return O número inteiro que é o identificador disponível para uma hashtag
      static int get_available_id();

      /*@brief Verifica se a hashtag existe por seu name e retorna
      true: se existir ou false: se não existir*/
      //@param name Uma string com o nome da hashtag
      static bool exists(std::string name);

      /*@brief Verifica se a hashtag existe por seu id e retorna
      true: se existir ou false: se não existir*/
      //@param id Um número inteiro que identifica a hashtag
      static bool exists(int id);

    private:

      //@brief Construtor vazio de Hashtag
      Hashtag();

      int use_count; ///< Número inteiro que indica quantas vezes a hashtag foi usada
      std::string name; ///< String que identifica o nome da hashtag
      std::vector<int> tweets; ///< Vetor dos tweets que usaram a hashtag (identificados por seus ids)

      //@brief Altera o id da hashtag
      //@param id Número inteiro que identifica a hashtag
      void set_id(int id);

      //@brief Altera o use_count da hashtag
      //@param use_count Número inteiro que indica quantas vezes a hashtag foi usada
      void set_use_count(int use_count);

      //@brief Altera o name da hashtag
      //@param name Uma string com o nome da hashtag
      void set_name(std::string name);

      //@brief Altera os tweets que usam a hashtag
      /*@param tweets Um vetor com os tweets que usaram a hashtag
      (representados por seus números identificadores)*/
      void set_tweets(std::vector<int> tweets);

      //@brief Carrega os dados do JSON para o objeto de hashtag
      //@param hashtag_json O objeto da classe Json que representa a hashtag
      void deserialize(Json hashtag_json);

      //@brief Transforma os atributos da hashtag em um JSON
      //@return Um objeto da classe Json para a hashtag
      Json serialize();
  };
}

#endif
