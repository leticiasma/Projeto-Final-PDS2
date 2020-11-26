#ifndef TWITTER_BASE_H
#define TWITTER_BASE_H

#include <string>
#include "nlohmann/json.hpp"

using Json = nlohmann::json;


namespace Twitter{

  class TwitterBase {

    public:

      //@brief Retorna o id
      //@return O número inteiro identificador
      virtual int get_id() = 0;

      //@brief Retorna a string do JSON da classe
      //@param indent O número inteiro de espaços da indentação da string retornada
      //@return A string que representa a estrutura do objeto da classe
      virtual std::string dump(int indent) = 0;

      //@brief Salva nos dados
      //@return true se foi salvo
      //@return false se não foi salvo
      virtual bool save() = 0;

    protected:
      int id;  ///< Número inteiro identificador

    private:

      //@brief Transforma os atributos do objeto de uma classe em um JSON
      //@return Um objeto da classe Json para o objeto
      virtual Json serialize() = 0;

      //@brief Carrega os dados do JSON para o objeto da classe
      //@param user_json O objeto da classe Json que representa o objeto
      virtual void deserialize(Json user_json) = 0;
  };
}

#endif
