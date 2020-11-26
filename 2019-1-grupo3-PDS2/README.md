# TRABALHO DE PDS 2 GRUPO 3
## Twitter

Repositório do trabalho de PDS 2 do grupo 3.

***

### Grupo

[Luiz Philippe](https://github.com/luizppa) - luizphilippe@dcc.ufmg.br

[Clarisse Scofield](https://github.com/clapscofield) - clarissescofield@dcc.ufmg.br

[Letícia Alves](https://github.com/leticiasma) - leti.ufmg@gmail.com

### Projeto

```
2019-1-grupo3
├── build
|   └── .gitignore
├── data # Diretório de armazenamento de dados de execução
|   ├── .gitignore
|   ├── hashtags.json
|   ├── logins.json
|   ├── replies.json
|   └── tweets.json
├── docs
|   ├── crc_cards.json # Cartões CRC importável para https://echeung.me/crcmaker/
|   ├── crc_cards.pdf # Cartões CRC em formato PDF
|   └── user_stories.pdf # Histórias de usuário em formato PDF
├── lib
|   ├── doctest
|   |   └── doctest.h
|   ├── exceptions
|   |   ├── forbidden_action_exception.hpp
|   |   ├── invalid_action_exception.hpp
|   |   ├── resource_not_found_exception.hpp
|   |   ├── unexistent_entity_exception.hpp
|   |   ├── unexistent_reply_exception.hpp
|   |   ├── unexistent_tweet_exception.hpp
|   |   └── unexistent_user_exception.hpp
|   ├── nlohmann # Biblioteca para manipulação de arquivos JSON
|   |   └── json.hpp
|   ├── hashtag.hpp
|   ├── interface.hpp
|   ├── reply.hpp
|   ├── retweet.hpp
|   ├── tweet.hpp
|   ├── twitter_base.hpp
|   └── user.hpp
├── src
|   ├── exceptions
|   |   ├── forbidden_action_exception.cpp
|   |   ├── invalid_action_exception.cpp
|   |   ├── resource_not_found_exception.cpp
|   |   ├── unexistent_entity_exception.cpp
|   |   ├── unexistent_reply_exception.cpp
|   |   ├── unexistent_tweet_exception.cpp
|   |   └── unexistent_user_exception.cpp
|   ├── hashtag.cpp
|   ├── interface.cpp
|   ├── main.cpp
|   ├── reply.cpp
|   ├── retweet.cpp
|   ├── tweet.cpp
|   └── user.cpp
├── test
|   ├── test_0_hashtag.cpp
|   ├── test_1_user.cpp
|   ├── test_2_tweet.cpp
|   ├── test_3_reply.cpp
|   └── test.cpp
├── .gitignore
├── makefile
└── README.md
```

### Dependências

- [Json for modern C++](https://github.com/nlohmann/json) (Já incluso no código fonte)
- [Doctest for unity tests](https://github.com/onqtam/doctest) (Já incluso no código fonte)

### Compilando e executando

- `$ make`: Compila os arquivos do código fonte
- `$ make run`: Executa a aplicação
- `$ make mem`: Executa a aplicação e exibe estatísticas de memória alocada e desalocada
- `$ make clean`: Remove os arquivos de saída da compilação
- `$ make tests`: Executa os testes de unidade

### User Stories

- Eu, como usuário, gostaria de compartilhar pensamentos e opiniões para me conectar com outras pessoas.
- Eu, como usuário, gostaria de seguir outras pessoas para me informar de assuntos de meu interesse.
- Eu, como usuário, gostaria de compartilhar tweets de outros usuários para fazer com que meus amigos tenham acesso àquele conteúdo.
- Eu, como usuário, gostaria de atualizar minhas informações para me apresentar para as outras pessoas.
- Eu, como usuário, gostaria de responder a outros tweets para interagir com outros usuários.

### Regras de estilo

- O código deve ser escrito em inglês (que é mais conveniente já que é um idioma comum que não utiliza acentos).
- A tabulação deve ser feita com um tab.
- Utilizar estruturas inline com moderação. Exemplo:
  ```
  if(...) a = b; // Ok, mas utilizar com cautela
  ```
- Abrir blocos com chaves coladas a estrutura (não na linha abaixo), blocos else e else if devem ser precedidos por uma única quebra de linha. Exemplo:
   ```
   if(...){ // Correto

   }
   else{

   }

   if(...) // Incorreto
   {

   } else {

   }
   ```
- Para beneficiar a legibilidade do código, utilizar espaços com frequência quando for conveniente. Exemplo:
  ```
  if(a == 0 && b == 1){ // Correto
    a = b + 3;
  }

  if(a==0&&b==1){ // Incorreto
    a=b+3;
  }
  ```
- Nomes de arquivos fonte no singular e em letra minúscula, no caso de nomes compostos, optar por snake case. Os nomes de arquivos que implementam classes devem ter o mesmo nome da classe. **Um arquivo não deve implementar mais de uma classe**
  - meu_arquivo.cpp (correto)
  - MeuArquivo.cpp (incorreto)
- Nomes de módulos no singular e em letra maiúscula, no caso de nomes compostos, optar por camel case.
  - MeuModulo (correto)
  - meu_modulo (incorreto)
- Nomes de classes no singular e em letra maiúscula, no caso de nomes compostos, optar por camel case.
  - MinhaClasse (correto)
  - minha_classe (incorreto)
- Nomes de métodos em letra minúscula, no caso de nomes compostos optar por snake case.
  - meu_metodo (correto)
  - MeuMetodo (incorreto)
- Nomes de variáveis em letra minúscula, no caso de nomes compostos optar por snake case.
  - minha_variavel (correto)
  - MinhaVariavel (incorreto)
