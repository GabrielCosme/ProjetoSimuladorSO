Para a implementação do projeto, foi utilizado o paradigma de Programação orientada a objetos, de forma que a solução se encontra divida em classes, que são descritas a seguir.

Memory: Representa a memória do sistema, ela possui um bitmap expandido, no qual o valor inicial 0 é reservado para espaços vazios e cada número diferente representa o id do processo que está ocupando aquele espaço na memória, isso pode ser alterado visualmente definindo a constante BITMAP_SHOW_IDS para false, de forma que ao ser exibido, o bitmap só mostrará valores binários. Seu tamanho é definido pela constante de configuração MEMORY_SIZE, que é 20 por padrão. Além disso, ela possui 3 métodos principais:
- allocate: Aloca um espaço definido na memória para um processo utilizando o algoritmo First Fit, preenchendo todos os valores em uma região do bitmap com o id do processo, e jogando uma exceção caso não seja possível alocar o espaço solicitado.
- free: Libera um espaço definido na memória, atribuindo 0 a todos valores do bitmap naquela região.
- get_first_fragmented_process: Retorna o id do primeiro processo na memória que ocupa uma posição com algum espaço vazio antes, de forma a auxiliar a defragmentação.

Process: Define cada um dos processos do sistema, armazenando cada uma de suas informações, que também são apresentadas na Tabela de Processo, como o id, o contador de programa, a quantidade de instruções, o endereço inicial da memória alocada, e o tamanho da memória alocada, essas informações são definidas ao construir o objeto, de forma que não precisam ser passadas posteriormente ao executar a operação de criar o processo. Também possui 3 métodos principais para lidar com cada uma das possível operações:
- create: Chama a função allocate da memória, passando os parâmetros necessários que foram definidos no construtor.
- run: Incrementa o contador de programa e o compara com o número de instruções, de forma que caso o processo tenha sido finalizado, a função retorna `true`, e caso contrário, retorna `false`.
- kill: Chama a função free da memória, liberando o espaço relativo ao processo. Esta função também é chamada no destrutor, de forma que sempre que o processo é apagado, seu espaço no bitmap é liberado.

Process Manager: Gerencia cada um dos processos do sistema, ela armazena o objeto da Memória, um Map que contém os processos atuais do sistema, indexados pelo seu id, e a fila de prontos, que é representada como uma List de Tasks, sendo que cada Task é definida por um Command, que pode ser do tipo CREATE, RUN ou KILL, e um id de processo. Ao construir o objeto, os processos iniciais definidos no arquivo de configuração são construídos e colocados na fila de prontos para serem executados, sendo que cada um dos métodos da classe que representam possíveis comandos são descritos a seguir:
- plan_create: Incrementa o contador de id, de forma a garantir um id único para cada processo, constrói um novo processo com os parâmetros definidos e o coloca no final da fila de prontos para ser criado.
- plan_kill: Adiciona uma Task de KILL do processo escolhido no final na fila de prontos.
- run_first_task: Executa a primeira task da fila de prontos, que pode ser de 3 tipos:
  - CREATE: Remove a task da fila de prontos, chama a função create do processo e coloca uma Task de run do processo no final da fila de prontos.
  - RUN: Chama a função de run do processo, executando uma instrução. Caso o processo tenha finalizado, remove a Task da fila de prontos e apaga o processo do Map. Caso ainda existam instruções a serem executadas, o gerenciador de processos decidirá o que fazer baseado na opção de configuração: Caso esteja no modo FIFO, nada mais será feito, visto que a Task continuará na fila de prontos até que o processo seja finalizado, e caso esteja no modo Round Robin, o quantum counter será incrementado, e assim que ele chegar no valor configurado, a task será movida para o final da fila de prontos.
  - KILL: Remove todas as tasks da fila de prontos que tem o id do processo morto, apaga o processo do Map, e caso a defragmentação automática esteja ativa, defragmenta a memória.
- defragment_memory: Utiliza a função auxiliar get_first_fragmented_process para realocar o primeiro processo fragmentado, chamando um kill seguido de um create do processo, visto que o create aloca o processo utilizando o algoritmo First-Fit, que evita a fragmentação. Isso ocorre até que não existam mais processos fragmentados, e a função utilizada da classe Memory retornará 0.

Interface: Responsável pelo interfaceamento entre o programa e o usuário, é dividida em 2 sub-classes, a Input e a Output.

Input: Lida com o terminal de entrada, que é o terminal no qual o programa é executado, sendo que está classe pode realizar entradas ou saídas, mas sempre no terminal principal do programa. Possui métodos para auxiliar na aquisição da entrada no usuário e imprimir mensagens de ajuda ou erro.
- update_input: Captura um comando de entrada do usuário e o armazena na variável input_command, também limpa a tela após o usuário terminar seu comando.
- get_command: Aplica os regex de cada um dos possíveis comandos que podem ser executados, retornando o tipo de comando que o usuário digitou, ou INVALID_COMMAND, caso nenhum dos regex deem match.
- get_instruction_amount: Retorna o valor do argumento que representa o número de instruções do processo em um comando de create.
- get_memory_size: Também Retorna o valor do argumento que representa o tamanho da memória que deve ser alocada para o processo em um comando de create.
- get_process_id: Retorna o valor do argumento que representa o id do processo que deve ser eliminado em um comando de kill.
- print_invalid_command: Imprime uma mensagem de comando inválido no terminal de entrada, sugerindo a utilização do comando help.
- print_invalid_pid: Imprime uma mensagem de ID de processo não encontrado no terminal de entrada.
- print_insufficient_memory: Imprime uma mensagem de memória insuficiente para criar o processo no terminal de entrada.

Output: Lida com o terminal de saída, que deve ser o segundo terminal aberto pelo usuário no sistema, de forma que o programa irá acessá-lo através do diretório `/dev/pts/1`, responsável por fornecer acesso aos pseudo-terminais presentes no sistema. Dessa forma, devem ser abertos dois terminais inicialmente no sistema, e o programa deve ser executado no primeiro deles, o `/dev/pts/0`, que também será considerado a entrada e saída padrão no contexto do código, com a qual a classe de Input irá se comunicar. Possui dois métodos para atualizar a tela e auxiliar no processo de construção da saída.
- update_output: Limpa a tela e imprime o Process Manager na tela, isso é possível através do overload do operador `<<`, que permite imprimir na tela informações privadas de uma classe, ou seja, toda a lógica de construção da mensagem de saída está presente no overload deste operador em cada uma das classes do sistema, simplificando a Interface de saída e negando a necessidade de criar getters para cada atributo.
- place_vertical: Utiliza regex para substituir os `line feed` presentes em uma string por Escape Codes, de forma a permitir que o texto seja posicionado em uma coluna específica sem afetar os textos já presentes na saída.

Main: Com as classes bem definidas, a função de execução principal do programa fica bem simples, já que basta chamar os métodos definidos na classe de Interface e do Process Manager. Inicialmente, essas duas classes são declaradas, e ao entrar no loop de execução principal, é atualizada a saída e a entrada com os métodos update_output e update_input, de forma que o retorno do get_command é avaliado, sendo realizadas diferentes ações para cada um dos possíveis comandos:
- CREATE: Obtém o valor dos argumentos com os métodos get_instruction_amount e get_memory_size, e em seguida, chama a função plan_create com os parâmetros adquiridos.
- RUN: Executa a primeira task da fila de prontos com a função run_first_task, e trata as possíveis exceções:
  - out_of_range: O programa tentou acessar um id de processo não existente, ocorre ao tentar dar kill em um processo que não existe. A função print_invalid_id é chamada.
  - length_error: O programa tentou alocar mais memória do que há disponível, ocorre ao executar um create com um parâmetro de memory_size que não é possível na configuração do momento. A função print_insufficient_memory é chamada.
- KILL: Obtém o id do processo com o método get_process_id, e chama a função plan_kill.
- DEFRAG: Chama a função defragment_memory, que defragmenta a memória instantaneamente, sem que a task precise passar pela fila de prontos.
- HELP: Chama a função print_help.
- EXIT: Finaliza a execução do programa.
- INVALID: Chama a função print_invalid_command.
