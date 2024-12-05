# Gerenciador Multiplataforma de Histórico de Clipboard  

**Autores:**  
- Enzo Daniel Marinho  
- Ryan Ribeiro Rodrigues  
- Ian Azevedo de Carvalho  

**Resumo:**  
Este projeto implementa um gerenciador de histórico de clipboard multiplataforma usando Python. Ele captura automaticamente o conteúdo copiado pelo usuário e o organiza em uma estrutura de dados otimizada para busca e gerenciamento. A aplicação apresenta uma interface gráfica intuitiva e um daemon que monitora continuamente o clipboard.  

**Palavras-chave:** Clipboard, gerenciamento de histórico, Python, Tkinter, automação  

---

## **Seção I. Introdução**  

### **Problema**  
A gestão do histórico de clipboard é uma necessidade cada vez mais presente em tarefas que demandam múltiplas referências, como edição de textos, pesquisa e programação. Este projeto aborda essa dificuldade ao criar uma solução que organiza e facilita o acesso a itens previamente copiados, promovendo maior eficiência e produtividade.  

### **Métodos**  
Os métodos implementados incluem:  
1. **Monitoramento Contínuo:** Um daemon em Python para capturar automaticamente o conteúdo do clipboard.  
2. **Estrutura de Dados Otimizada:** Utilização de uma Árvore de Busca Binária para armazenamento dinâmico e eficiente.  
3. **Interface Gráfica (GUI):** Desenvolvida com Tkinter para interação amigável e intuitiva com o usuário.  

### **Objetivos**  
- Capturar o conteúdo copiado em tempo real.  
- Armazenar o histórico de forma estruturada e permitir buscas eficientes.  
- Oferecer uma interface gráfica simples e funcional para gerenciamento do histórico.  

---

## **Seção II. Arquitetura do Sistema**  

### **Componentes Principais**  

1. **Daemon de Monitoramento**  
   - Classe: `ClipboardDaemon`  
   - Função: Monitora continuamente o clipboard em busca de novos conteúdos e os registra no sistema.  

2. **Gerenciador de Histórico**  
   - Classe: `ClipboardManager`  
   - Função: Gerencia o histórico, utilizando uma Árvore de Busca Binária para inserções e buscas rápidas.  

3. **Interface Gráfica**  
   - Classe: `ClipboardInterface`  
   - Função: Permite ao usuário visualizar, buscar, copiar e gerenciar o histórico por meio de uma interface visual.  

4. **Estrutura de Dados**  
   - Classe: `ClipboardSearchTree`  
   - Estrutura: Árvore de Busca Binária  
   - Função: Armazena e organiza o histórico de forma eficiente, permitindo buscas otimizadas.  

---

## **Seção III. Funcionalidades Implementadas**  

1. **Captura Automática**  
   - Captura em tempo real dos itens copiados pelo usuário utilizando a biblioteca `pyperclip`.  

2. **Gerenciamento de Histórico**  
   - Adição de novos itens ao histórico.  
   - Busca eficiente por meio de uma Árvore de Busca Binária.  
   - Visualização completa do histórico.  

3. **Interface Gráfica**  
   - Busca de itens no histórico.  
   - Opção de copiar diretamente itens selecionados para o clipboard.  
   - Botão para encerrar o daemon e a aplicação.  

4. **Persistência Temporária**  
   - Os dados são armazenados na memória durante a execução da aplicação.  

---

## **Seção IV. Estruturas de Dados**  

### **Árvore de Busca Binária**  
- **Motivação:** Proporciona buscas rápidas e eficiente organização dos dados.  
- **Implementação:**  
  - Cada nó contém um texto copiado.  
  - Inserções e buscas são realizadas de forma recursiva.  
  - A travessia em ordem (in-order traversal) retorna os itens do histórico em ordem alfabética.  

---

## **Seção V. Bibliotecas Utilizadas**  

1. **`pyperclip`**  
   - Captura e manipula o conteúdo do clipboard.  

2. **`multiprocessing`**  
   - Garante a execução paralela do daemon e da interface gráfica.  

3. **`tkinter`**  
   - Criação da interface gráfica.  

4. **`os` e `signal`**  
   - Controla e finaliza processos em execução.  

5. **`time`**  
   - Introduz atrasos no monitoramento contínuo do clipboard.  

6. **`sys`**  
   - Garante o encerramento seguro do sistema ao capturar sinais de término.  

---

## **Seção VI. Resultados**  

### **Resultados Obtidos**  
- **Funcionalidades Implementadas:**  
  - Monitoramento eficiente do clipboard.  
  - Busca e gerenciamento do histórico por meio de interface gráfica.  
  - Armazenamento dinâmico com Árvore de Busca Binária.  

### **Desempenho**  
- **Latência:** O sistema apresenta um tempo médio de resposta de 1 segundo entre a captura e o registro do conteúdo copiado.  

---

## **Seção VII. Conclusões e Trabalhos Futuros**  

### **Conclusões**  
O projeto demonstra a viabilidade de implementar um sistema eficiente de gerenciamento de histórico de clipboard utilizando Python. A integração entre o monitoramento contínuo, estruturação de dados eficiente e interface gráfica resulta em uma solução robusta e amigável para o usuário.  

### **Trabalhos Futuros**  
- Implementar persistência dos dados em armazenamento local (ex.: arquivos ou banco de dados).  
- Expandir o suporte a diferentes tipos de conteúdo (ex.: imagens, arquivos, etc.).  
- Melhorar o desempenho do sistema em cenários com grandes volumes de dados.  
- Melhorar o refresh para um refresh automático

---

## **Referências**  

1. PYTHON. *pyperclip - A cross-platform clipboard module for Python*. Documentação oficial, 2024. Disponível em: [https://pypi.org/project/pyperclip/](https://pypi.org/project/pyperclip/). Acesso em: 04 dez. 2024.  

2. PYTHON. *multiprocessing — Process-based parallelism*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/multiprocessing.html](https://docs.python.org/3/library/multiprocessing.html). Acesso em: 04 dez. 2024.  

3. PYTHON. *tkinter — Python interface to Tcl/Tk*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/tkinter.html](https://docs.python.org/3/library/tkinter.html). Acesso em: 04 dez. 2024.  

4. PYTHON. *signal — Set handlers for asynchronous events*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/signal.html](https://docs.python.org/3/library/signal.html). Acesso em: 04 dez. 2024.  

5. PYTHON. *os — Miscellaneous operating system interfaces*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/os.html](https://docs.python.org/3/library/os.html). Acesso em: 04 dez. 2024.  

6. PYTHON. *time — Time access and conversions*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/time.html](https://docs.python.org/3/library/time.html). Acesso em: 04 dez. 2024.  

7. PYTHON. *sys — System-specific parameters and functions*. Documentação oficial, 2024. Disponível em: [https://docs.python.org/3/library/sys.html](https://docs.python.org/3/library/sys.html). Acesso em: 04 dez. 2024.  

---

#### **Como rodar o projeto**

O codigo principal está na pasta src
