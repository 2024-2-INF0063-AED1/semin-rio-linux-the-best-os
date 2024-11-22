# Gerenciador Multiplataforma de Histórico de Clipboard  

**Autores:** Enzo Daniel Marinho, Ryan Ribeiro Rodrigues e Ian Azevedo de Carvalho

**Resumo:** Este projeto propõe a criação de um gerenciador multiplataforma de histórico de clipboard. Ele captura automaticamente o conteúdo copiado pelo usuário e o organiza em um histórico acessível e gerenciável. Utiliza sockets para comunicação cliente-servidor e implementa algoritmos otimizados para busca e armazenamento dinâmico dos itens. 

**Palavras-chave:** Clipboard, sockets, sistemas operacionais, armazenamento dinâmico, Python, C  

---

## **Seção I. Introdução**  

### **Problema**  
Usuários frequentemente enfrentam dificuldades para gerenciar o histórico de itens copiados. O problema é mais evidente em tarefas que demandam múltiplas referências ao clipboard, como pesquisas, edição de documentos e programação. Esse projeto busca solucionar essa limitação, fornecendo um sistema que captura, organiza e facilita o acesso ao histórico de textos copiados.  

### **Literatura**

1. **Vídeo:** [Sockets and Pipes Look Like Files (Unix/fdopen)](https://www.youtube.com/watch?v=il4N6KjVQ-s)  
   - Explica como sockets e pipes podem ser manipulados como arquivos em sistemas Unix, destacando o uso de `fdopen`.  

2. **Vídeo:** [Unix Domain Socket in 100 seconds](https://www.youtube.com/watch?v=1UHaR54i3ak)  
   - Uma introdução rápida e prática ao uso de Unix Domain Sockets.  

3. **Vídeo:** [[Linguagem C] Sockets no Windows e no Linux](https://www.youtube.com/watch?v=GaxjJvMnz-I)  
   - Demonstra como implementar sockets em C, abordando tanto sistemas Windows quanto Linux.  

4. **Documentação:** [socket](https://docs.python.org/3/library/socket.html#socket.SOCK_STREAM)  
   - Documentação oficial do módulo `socket` em Python, cobrindo o uso de `SOCK_STREAM` para comunicação TCP.  

5. **How-to Guide:** [Programação de Soquetes](https://docs.python.org/pt-br/3/howto/sockets.html)  
   - Guia introdutório à programação de sockets, com exemplos em Python.  

6. **Discussão:** [Can anyone explain to me how __init__ == '__main__' is used properly?](https://www.reddit.com/r/Python/comments/qsezbh/can_anyone_explain_to_me_how_init_main_is_used/)  
   - Discussão em fórum sobre o uso correto da estrutura `if __name__ == '__main__'` em Python, relevante para a execução do cliente.  

7. **Artigo:** [Introdução a Sockets em Python](https://medium.com/@urapython.community/introdu%C3%A7%C3%A3o-a-sockets-em-python-44d3d55c60d0)  
   - Um artigo introdutório sobre programação com sockets em Python, incluindo exemplos práticos.
     
8. **Artigo:** [Manipulação de arquivos em C — como abrir e fechar arquivos e escrever algo neles](https://www.freecodecamp.org/portuguese/news/manipulacao-de-arquivos-em-c-como-abrir-e-fechar-arquivos-e-escrever-algo-neles/)
   - Um artigo que trata de abrir, escrever e fechar arquivos .txt em C
     
9. **Artigo:** [How much memory does a call to ‘malloc’ allocate?](https://lemire.me/blog/2024/06/27/how-much-memory-does-a-call-to-malloc-allocates/)
   - Artigo que discute o funcionamento da função malloc com mais detalhes
10. **Artigo:** [Como armazenar uma string com tamanho qualquer (desconhecido antecipadamente) em C?](https://pt.quora.com/Como-armazenar-uma-string-com-tamanho-qualquer-desconhecido-antecipadamente-em-C)
   - Uma solução de como ler uma string de tamanho desconhecido, porém ainda possui a limitação de não suportar o caractere de quebra de linha
11. **Artigo:** [Como faço para guardar uma string com tamanho indefinido sem utilizar memória extra](https://pt.stackoverflow.com/questions/175529/como-faço-para-guardar-uma-string-com-tamanho-indefinido-numa-estrutura)
   - Artigo que trata da leitura de string de tamanho desconhecido, dando ênfase ao uso adequado de buffers e a função strlen(), ideal para o entendimento do código, que implementa na função realloc().
12. **Artigo:** [How does the EOF macro work with getchar?](https://stackoverflow.com/questions/34816138/how-does-the-eof-macro-work-with-getchar)
   - Artigo que trata da macro EOF, uma macro que é a solução implementada para a leitura indefinida de caracteres, teoricamente, para além de uma segunda solução implementada no código.

### **Dataset**  
Neste projeto, o "dataset" é composto pelos itens copiados pelo usuário em tempo real, armazenados inicialmente em uma lista ligada e, futuramente, em arquivos persistentes.  

### **Métodos**  
Os métodos principais incluem:  
1. Captura automática de eventos de `CTRL+C`.  
2. Comunicação cliente-servidor via sockets.  
3. Armazenamento dinâmico em listas ligadas.  
4. Planejamento para integração de árvores de busca para otimização.  

### **Avaliação**  
Os resultados serão medidos por:  
- Latência das operações (`COPY`, `DELETE`, `SELECT`, `SHOW_LIST`).  
- Consumo de memória durante a execução.  
- Eficiência da busca no histórico (quando implementada a árvore de busca).  

---

## **Seção II. Fundamentos Teóricos**  

### **Sockets Cliente-Servidor**  
A comunicação cliente-servidor é realizada por meio de sockets UNIX (Linux) e TCP/IP (Windows). Essa abordagem permite troca de dados rápida e confiável entre os componentes do sistema, independentemente do sistema operacional.  

### **Estruturas de Dados**  
- **Lista Ligada:** Utilizada para armazenamento temporário dos itens copiados. Sua implementação permite inserções e remoções dinâmicas.  
- **Árvore de Busca Binária (Planejada):** Visa melhorar a eficiência na busca de itens ao longo do histórico.  

### **Integração Python e C**  
O projeto combina a versatilidade do Python com a performance do C. Enquanto o cliente utiliza Python para lidar com eventos de sistema e interação com o usuário, o servidor em C é responsável por processar e armazenar os dados de forma eficiente.  

---

## **Seção III. Metodologia**  

1. **Arquitetura Inicial:**  
   - Configuração de um socket servidor em C e cliente em Python.  
   - Desenvolvimento da função `handle_signal` para capturar `CTRL+C` e enviar mensagens ao servidor.  

2. **Estruturação do Histórico:**  
   - Implementação de uma lista ligada para gerenciamento dinâmico do histórico.  
   - Definição dos comandos básicos:  
     - `COPY <texto>`: Armazena o texto copiado.  
     - `DELETE <id>`: Remove um item específico.  
     - `SELECT <id>`: Recupera um item.  
     - `SHOW_LIST`: Exibe todos os itens armazenados.  

3. **Otimização e Planejamento:**  
   - Planejamento para integração de uma árvore de busca para consultas mais rápidas.  
   - Proposta de armazenamento permanente para persistência dos dados.  

---

## **Seção IV. Resultados e Conclusões**  

### **Resultados**  
- **Funcionalidades Implementadas:**  
  - Comunicação cliente-servidor bem-sucedida.  


- **Desempenho:**  
TODO

### **Conclusões**  
- **Lições Aprendidas:**  
TODO

- **Perspectivas Futuras:**  
  - Implementar armazenamento permanente para histórico.  
  - Otimizar buscas com árvores de busca.  
  - Expandir a funcionalidade para múltiplos formatos de dados (imagens, arquivos, etc.).  

---

## **Referências**  

1. [Sockets and Pipes Look Like Files (Unix/fdopen)](https://www.youtube.com/watch?v=il4N6KjVQ-s)
2. [Unix Domain Socket in 100 seconds](https://www.youtube.com/watch?v=1UHaR54i3ak)
3. [[Linguagem C] Sockets no Windows e no Linux](https://www.youtube.com/watch?v=GaxjJvMnz-I)
4. [socket](https://docs.python.org/3/library/socket.html#socket.SOCK_STREAM)
5. [Programação de Soquetes](https://docs.python.org/pt-br/3/howto/sockets.html)
6. [Can anyone explain to me how __init__ == ‘__main__’ is used properly? I’ve watched multiple videos but it just hard for me to understand.](https://www.reddit.com/r/Python/comments/qsezbh/can_anyone_explain_to_me_how_init_main_is_used/)
7. [Introdução a Sockets em Python](https://medium.com/@urapython.community/introdu%C3%A7%C3%A3o-a-sockets-em-python-44d3d55c60d0)
8. [Manipulação de arquivos em C — como abrir e fechar arquivos e escrever algo neles](https://www.freecodecamp.org/portuguese/news/manipulacao-de-arquivos-em-c-como-abrir-e-fechar-arquivos-e-escrever-algo-neles/)
9. [How much memory does a call to ‘malloc’ allocate?](https://lemire.me/blog/2024/06/27/how-much-memory-does-a-call-to-malloc-allocates/)
10. [Como armazenar uma string com tamanho qualquer (desconhecido antecipadamente) em C?](https://pt.quora.com/Como-armazenar-uma-string-com-tamanho-qualquer-desconhecido-antecipadamente-em-C)
11. [Como faço para guardar uma string com tamanho indefinido sem utilizar memória extra](https://pt.stackoverflow.com/questions/175529/como-faço-para-guardar-uma-string-com-tamanho-indefinido-numa-estrutura)
12. [How does the EOF macro work with getchar?](https://stackoverflow.com/questions/34816138/how-does-the-eof-macro-work-with-getchar)
