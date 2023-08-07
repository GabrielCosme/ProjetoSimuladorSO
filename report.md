Foi utilizado o paradigma de POO, com a solução sendo divida nas classes descritas a seguir.

Memory: Representa a memória do sistema, que possui um bitmap expandido no qual o valor inicial 0 é reservado para espaços vazios e cada número diferente representa o id do processo que está ocupando aquele espaço, isso pode ser alterado visualmente definindo a configuração BITMAP_SHOW_IDS como false, de forma que o bitmap só mostrará valores binários. Seu tamanho é definido pela configuração MEMORY_SIZE, que é 20 por padrão. Além disso, ela possui 3 métodos principais:
- allocate: Aloca um determinado espaço na memória para um processo utilizando o algoritmo First Fit, preenchendo todos os valores em uma região do bitmap com o id do processo, e jogando uma exceção caso não seja possível alocar o espaço solicitado.
- free: Libera um espaço definido na memória, atribuindo 0 a todos valores do bitmap naquela região.
- get_first_fragmented_process: Retorna o id do primeiro processo na memória que ocupa uma posição com algum espaço vazio antes, de forma a auxiliar a defragmentação.

Process: Define cada um dos processos do sistema, armazenando cada uma de suas informações, que também são apresentadas na Tabela de Processo, como o id, o contador de programa, a quantidade de instruções, o endereço inicial da memória alocada, e o tamanho da memória alocada, essas informações são definidas ao construir o objeto, de forma que não precisam ser passadas posteriormente ao executar a operação de criar o processo. Também possui 3 métodos principais para lidar com cada uma das possível operações:
- create: Chama a função allocate da memória, passando os parâmetros necessários que foram definidos no construtor.
- run: Incrementa o contador de programa e o compara com o número de instruções, de forma que caso o processo tenha sido finalizado, a função retorna `true`, e caso contrário, retorna `false`.
- kill: Chama a função free da memória, liberando o espaço relativo ao processo. Esta função também é chamada no destrutor, de forma que sempre que o processo é apagado, seu espaço no bitmap é liberado.

Process Manager: Gerencia os processos do sistema, para isso, armazena o objeto da Memória, um Map que contém os processos atuais indexados pelo seu id, e a fila de prontos, que é representada como uma List de Tasks, sendo que cada Task é definida por um Command, que pode ser do tipo CREATE, RUN ou KILL, e o id do processo. Ao construir o objeto, a criação dos processos iniciais definidos no arquivo de configuração é colocada na fila de prontos. Os métodos da classe são descritos a seguir:
- plan_create: Incrementa o contador de id, de forma a garantir um id único para cada processo, constrói um novo processo com os parâmetros definidos e adiciona uma Task de CREATE no final da fila de prontos.
- plan_kill: Adiciona uma Task de KILL do processo escolhido no final na fila de prontos.
- run_first_task: Executa a primeira task da fila de prontos, que pode ser de 3 tipos:
  - CREATE: Remove a task da fila de prontos, chama a função create do processo e coloca uma Task de RUN do processo no final da fila de prontos.
  - RUN: Chama a função de run do processo, executando uma instrução. Caso o processo tenha finalizado, remove a Task da fila de prontos e apaga o processo do Map. Caso ainda existam instruções a serem executadas, o gerenciador de processos decidirá o que fazer baseado na opção de configuração: Caso esteja no modo FIFO, nada mais será feito, visto que a Task continuará na fila de prontos até que o processo seja finalizado, e caso esteja no modo Round Robin, o quantum counter será incrementado, e assim que ele chegar no valor configurado, a task será movida para o final da fila de prontos.
  - KILL: Remove todas as tasks da fila de prontos que tem o id do processo morto, apaga o processo do Map, e caso a defragmentação automática esteja ativa, defragmenta a memória.
- defragment_memory: Utiliza a função auxiliar get_first_fragmented_process para realocar o primeiro processo fragmentado, chamando um kill seguido de um create do processo, visto que o create aloca o processo utilizando o algoritmo First-Fit, que evita a fragmentação. Isso ocorre até que não existam mais processos fragmentados, e a função utilizada da classe Memory retornará 0.

Interface: Responsável pelo interfaceamento entre o programa e o usuário, é dividida em 2 sub-classes, a Input e a Output.

Input: Lida com o terminal de entrada, no qual o programa é executado, realizando entradas e saídas, mas sempre no terminal principal do programa. Possui métodos para auxiliar na aquisição da entrada no usuário e imprimir mensagens de ajuda ou erro.
- update_input: Captura um comando de entrada do usuário e o armazena na variável input_command, também limpa a tela após o usuário terminar seu comando.
- get_command: Aplica os regex de cada um dos possíveis comandos que podem ser executados, retornando o tipo de comando que o usuário digitou, ou INVALID_COMMAND, caso nenhum dos regex deem match.
- get_instruction_amount: Retorna o número de instruções do processo em um comando de create.
- get_memory_size: Também Retorna o tamanho da memória que deve ser alocada para o processo em um comando de create.
- get_process_id: Retorna o id do processo que deve ser eliminado em um comando de kill.
- print_invalid_command: Imprime uma mensagem de comando inválido no terminal de entrada, sugerindo a utilização do comando help.
- print_invalid_pid: Imprime uma mensagem de ID de processo não encontrado no terminal de entrada.
- print_insufficient_memory: Imprime uma mensagem de memória insuficiente para criar o processo no terminal de entrada.

Output: Lida com o terminal de saída, que deve ser o segundo terminal aberto pelo usuário no sistema, já que o programa irá acessá-lo através do diretório `/dev/pts/1`, responsável por fornecer acesso aos pseudo-terminais presentes no sistema. Possui dois métodos para atualizar a tela e auxiliar no processo de construção da saída.
- update_output: Limpa a tela e imprime o Process Manager na tela, isso é possível através do overload do operador `<<`, que permite imprimir na tela informações privadas de uma classe, simplificando a Interface de saída e negando a necessidade de criar getters para cada atributo.
- place_vertical: Utiliza regex para substituir os `line feed` presentes em uma string por Escape Codes, de forma a permitir que o texto seja posicionado em uma coluna específica sem afetar os textos já presentes na saída.

Main: Chama os métodos definidos na classe Interface e Process Manager, começando com com a declaração de um objeto de cada uma das classes, e ao entrar no loop de execução principal, é atualizada a saída e a entrada com os métodos update_output e update_input, de forma que o retorno do get_command é avaliado, sendo realizadas diferentes ações para cada um dos possíveis comandos:
- CREATE: Obtém o valor dos argumentos com os métodos get_instruction_amount e get_memory_size, e em seguida, chama a função plan_create com os parâmetros adquiridos.
- RUN: Executa a primeira task da fila de prontos com a função run_first_task, e trata as possíveis exceções:
  - out_of_range: O programa tentou acessar um id de processo não existente, ocorre ao tentar dar kill em um processo que não existe. A função print_invalid_id é chamada.
  - length_error: O programa tentou alocar mais memória do que há disponível, ocorre ao executar um create com um parâmetro de memory_size que não é possível na configuração do momento. A função print_insufficient_memory é chamada.
- KILL: Obtém o id do processo com o método get_process_id, e chama a função plan_kill.
- DEFRAG: Chama a função defragment_memory, que defragmenta a memória instantaneamente, sem que a task precise passar pela fila de prontos.
- HELP: Chama a função print_help.
- EXIT: Finaliza a execução do programa.
- INVALID: Chama a função print_invalid_command.
