# MC504 Projeto 03 - Animação multithread ouEstudo de tópico avançado sobre sincronização

## Grupo 
* `Fernando de Saes Madeira Vallar RA: 171509`
* `João Theophilo Morais Santos da Silva RA: 237736`
* `Lucas Barroso Alves Farias RA: 220650`

## Resumo do Projeto
> Neste terceiro trabalho da disciplina, com o objetivo  de demonstrar uma aplicação multithread que utiliza locks ou variáveis de condição para sincronização e de um visualizador para o progresso desta aplicação, decidimos abordar o mesmo caso do projeto dois da disciplina dos "múltiplos barbeiros dorminhocos" (descrição do caso feita no projeto passado: [projeto 2](https://github.com/IucasF/trabalho2mc504)), com a diferença que desta vez, ao invés de utilizar semáfaros, utilizamos mutex, que é um tipo de lock que permite que múltiplas threads acessem um recurso de memória compartilhado, mas apenas um de cada vez, diferente do semáfaro utilizado anteriormente que permite à multiplas threads o acesso aos recursos de memória compartilhado ao mesmo tempo (em paralelo), e claro, assim como no projeto passado, todo o processo é mostrado em um painel animado, que reflete os passos do código do arquivo em questão: [projeto 3](https://github.com/IucasF/trabalho3mc504/blob/main/barbearia.c).
