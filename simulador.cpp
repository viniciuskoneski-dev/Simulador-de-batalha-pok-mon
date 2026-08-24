#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Estrutura que representa uma unidade de pokémon

struct Pokemon{
    char nome[25];
    float ataque;
    float defesa;
    float vida;
    char tipo[10];
};

/*
Essa é uma função que checa se o tipo de um pokemon tem alguma vantagem, ou 
desvantagem em relação ao outro, aumentanto ou diminuindo o multiplicador de 
acordo com a relação entre os tipos.
*/

float calcular_vantagem(char* tipo_atacante, char* tipo_defensor){
    float multiplicador=1;
    if(strcmp(tipo_atacante, "eletrico")==0){
        if (strcmp(tipo_defensor, "agua")==0) multiplicador = 1.2;
        else if (strcmp(tipo_defensor, "pedra")==0) multiplicador = 0.8;
    }
    else if(strcmp(tipo_atacante, "agua")==0){
        if (strcmp(tipo_defensor, "fogo")==0) multiplicador = 1.2;
        else if (strcmp(tipo_defensor, "eletrico")==0) multiplicador = 0.8;
    }
    else if(strcmp(tipo_atacante, "fogo")==0){
        if (strcmp(tipo_defensor, "gelo")==0) multiplicador = 1.2;
        else if (strcmp(tipo_defensor, "agua")==0) multiplicador = 0.8;
    }
    else if(strcmp(tipo_atacante, "gelo")==0){
        if (strcmp(tipo_defensor, "pedra")==0) multiplicador = 1.2;
        else if (strcmp(tipo_defensor, "fogo")==0) multiplicador = 0.8;
    }
    else if(strcmp(tipo_atacante, "pedra")==0){
        if (strcmp(tipo_defensor, "eletrico")==0) multiplicador = 1.2;
        else if (strcmp(tipo_defensor, "gelo")==0) multiplicador = 0.8;
    }
    return multiplicador;
}

/*
Essa é a função que simula um ataque de um pokémon a outro. Para isso, a função
calcula o dano causado pelo ataque, e verifica se o pokemon adversário foi 
nocauteado. Caso nocauteado, a batalha entre esse 2 pokemons termina e a função
retorna 1, caso contrário 0.
*/

int batalha(Pokemon* atacante, Pokemon* defensor){
    float multiplicador = calcular_vantagem(atacante->tipo, defensor->tipo);
    float dano;
    //calculando dano
    if(atacante->ataque * multiplicador > defensor->defesa){
        dano=atacante->ataque * multiplicador - defensor->defesa;
    }
    else dano=1;
    defensor->vida = defensor->vida - dano;
    //verificando continuidade da batalha
    if (defensor->vida<=0) return 1;
    else return 0;
}

/*
Essa é uma função que controla o fluxo da batalha. Se um pokemon sobrevive a um
ataque, ele vira o atacante da rodada, e assim sucessivamente, até algum deles 
ser nocauteado.
*/

int rodada(Pokemon* jogador1, Pokemon* jogador2, int contador_turno){
    int resultado_batalha = batalha(jogador1, jogador2);
    if (resultado_batalha == 1) return contador_turno;
    else return rodada(jogador2, jogador1, contador_turno + 1);
}

/*
Essa é a função principal do programa, responsável por determinar qual pokemon venceu
cada um dos combastes, e calcular o jogador vencedor.
*/

void jogo(Pokemon* jogador1, Pokemon* jogador2, int npokemonJ1, int npokemonJ2){
    //inicializando contadores de mortes dos pokemons.
    //esses contadores são responsáveis por controlar a continuidade da batalha.
    int contador_morteJ1=0;
    int contador_morteJ2=0;
    int vencedor_rodada=0;
    //executa a primeira batalha com o jogador1 como atacante.
    vencedor_rodada=rodada(&jogador1[contador_morteJ1], &jogador2[contador_morteJ2], 1)%2;
    //enquanto os 2 jogadores estiverem com algum pokemon vivo a batalha continua.
    while(1){
        //O jogador 1 sempre vence nos turno ímpares e o 2 nos pares.
        //Verifica o jogador vencedor com base do resto da divisão por 2.
        //Se o vencedor da rodada for ímpar, ou seja, jogador 1.
        if (vencedor_rodada==1){
            printf("%s venceu %s\n", jogador1[contador_morteJ1].nome, jogador2[contador_morteJ2].nome);
            contador_morteJ2=contador_morteJ2 + 1;
            if(contador_morteJ2==npokemonJ2) break;
            //se o jogador 1 ganhou, o próximo ataque é do jogador 2, iniciando o contador em 0
            vencedor_rodada=rodada(&jogador2[contador_morteJ2], &jogador1[contador_morteJ1], 0)%2;
        }
        //Se o vencedor da rodada for par, ou seja, jogador 2.
        else{
            printf("%s venceu %s\n", jogador2[contador_morteJ2].nome, jogador1[contador_morteJ1].nome);
            contador_morteJ1=contador_morteJ1 + 1;
            if(contador_morteJ1==npokemonJ1) break;
            //se o jogador 2 ganhou, o próximo ataque é do jogador 1, iniciando o contador em 1
            vencedor_rodada=rodada(&jogador1[contador_morteJ1], &jogador2[contador_morteJ2], 1)%2;
        }
    }
    //Verificando o vencedor da última rodada
    if (vencedor_rodada==1){
        printf("Jogador 1 venceu\n");
        printf("Pokemon sobreviventes:\n");
        for(int i=0; i<npokemonJ1; i++){
            if (jogador1[i].vida>0) printf("%s\n", jogador1[i].nome);
        }
    }
    else{
        printf("Jogador 2 venceu\n");
        printf("Pokemon sobreviventes:\n");
        for(int i=0; i<npokemonJ2; i++){
            if (jogador2[i].vida>0) printf("%s\n", jogador2[i].nome);
        }
    }
    printf("Pokemon derrotados:\n");
    for(int i=0; i<npokemonJ1; i++){
        if (jogador1[i].vida<=0) printf("%s\n", jogador1[i].nome);
    }
    for(int i=0; i<npokemonJ2; i++){
        if (jogador2[i].vida<=0) printf("%s\n", jogador2[i].nome);
    }
        
    
}
    
int main() {
    // Abertura do arquivo
    FILE* f = fopen("entrada.txt", "r+t");
    if (f == NULL) {
        printf("Nao foi possivel abrir o arquivo de entrada.\n");
        return 1;
    }
    
    // Parte de input do código
    int npokemonJ1;
    int npokemonJ2;
    
    fscanf(f, "%d %d\n", &npokemonJ1, &npokemonJ2);
    
    Pokemon pokemonsJ1[npokemonJ1];
    Pokemon pokemonsJ2[npokemonJ2];
    
    for (int i = 0; i < npokemonJ1; i++) {
        fscanf(f, "%s %f%f%f %s\n", pokemonsJ1[i].nome, &pokemonsJ1[i].ataque, &pokemonsJ1[i].defesa, &pokemonsJ1[i].vida, pokemonsJ1[i].tipo);
    }

    for (int i = 0; i < npokemonJ2; i++) {
        fscanf(f, "%s %f%f%f %s\n", pokemonsJ2[i].nome, &pokemonsJ2[i].ataque, &pokemonsJ2[i].defesa, &pokemonsJ2[i].vida, pokemonsJ2[i].tipo);
    }

    // Parte de output do código
    jogo(pokemonsJ1, pokemonsJ2, npokemonJ1, npokemonJ2);

    // Fechamento do arquivo
    fclose(f);

    return 0;
}
