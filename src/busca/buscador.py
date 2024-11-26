def buscar_em_arquivo():
    # Definindo o nome do arquivo diretamente no código
    arquivo = "historico.txt"
    
    try:
        # Lendo o conteúdo do arquivo
        with open(arquivo, 'r', encoding='utf-8') as f:
            conteudo = f.readlines()
        
        print("\nDigite o texto que deseja buscar. Pressione ENTER sem digitar nada para sair.\n")
        
        while True:
            # Captura da entrada do usuário
            termo = input("Buscar: ").strip()
            
            # Se o usuário pressionar ENTER sem digitar nada, sair
            if termo == "":
                print("Encerrando a busca.")
                break
            
            # Filtrar linhas que contêm o termo buscado
            resultados = [linha.strip() for linha in conteudo if termo.lower() in linha.lower()]
            
            # Exibindo os resultados
            if resultados:
                print("\nResultados encontrados:")
                for i, linha in enumerate(resultados, 1):
                    print(f"{i}. {linha}")
            else:
                print("Nenhuma correspondência encontrada.")
            print()
    
    except FileNotFoundError:
        print(f"Erro: O arquivo '{arquivo}' não foi encontrado. Certifique-se de que ele está no mesmo diretório do script.")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")


# Executando a função
buscar_em_arquivo()