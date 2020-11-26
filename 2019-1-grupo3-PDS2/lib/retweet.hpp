#ifndef RETWEET_H
#define RETWEET_H

#include <string>
#include <vector>
#include "tweet.hpp"
#include "user.hpp"

namespace Twitter {

  class Retweet: public Tweet{

    friend class Tweet;

    public:

      //@brief Retorna o id do retweet
      //@return O número inteiro que identifica o retweet
      int get_original_id();

      //@brief Converte um retweet em um tweet nos dados
      //@return um booleano que indica se a ação foi concluída com sucesso
      bool make_tweet();

      //@brief Indica que é um retweet (retornando true)
      bool is_retweet() override { return true; }

      //@brief Posta um retweet criando-o, e salvando o retweet nos dados
      //@return O ponteiro para o retweet se ele for postado
      //@return nullptr se o retweet não pôde ser postado
      static Retweet* post_tweet(User* creator, Tweet* original_tweet);

      //@brief Retorna o vetor de retweets relacionados a um tweet
      //@param tweet_id Um número inteiro que identifica o tweet
      //@return O vetor de ponteiros para os retweets correspondentes
      static std::vector<Retweet*> get_retweets(int tweet_id);

    private:
      int original_id; ///< Número inteiro que identifica o retweet

      //@brief Construtor vazio de Retweet
      Retweet() {}

      //@brief Cria um objeto de Retweet sem salvá-lo nos dados
      //@param creator_id Um número inteiro que identifica o criador do tweet que será retweetado
      //@param original_tweet Um ponteiro para o tweet original que será retweetado
      Retweet(int creator_id, Tweet* original_tweet);

      //@brief Altera o id do retweet
      //@param original_id Um número inteiro que identifica o retweet
      void set_original_id(int original_id);

      //@brief Transforma os atributos do Retweet em um JSON
      //@return Um objeto da classe Json para o retweet
      Json serialize() override;

      //@brief Carrega os dados do JSON para o objeto de Retweet
      //@param tweet_json O objeto da classe Json que representa o retweet
      void deserialize(Json tweet_json) override;
  };
}

#endif
