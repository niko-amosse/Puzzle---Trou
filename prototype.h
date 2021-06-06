#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <direct.h>

// Ici le prototpe de toutes les fonctions defenies dans le main.c et le main.h et les constantes

// puzzle CARRE
#define LARGEUR_FENETRE_C 605
#define HAUTEUR_FENETRE_C 680
#define LARGEUR_PUZZLE_C 505
#define HAUTEUR_PUZZLE_C 505
#define COIN_X_C 50
#define COIN_Y_C 126
// puzzle RECTANGLE HORIZONTAL
#define LARGEUR_FENETRE_RH 760
#define HAUTEUR_FENETRE_RH 680
#define LARGEUR_PUZZLE_RH 695
#define HAUTEUR_PUZZLE_RH 512
#define COIN_X_H 37
#define COIN_Y_H 131
// puzzle RECTANGLE VERTICAL
#define LARGEUR_FENETRE_RV 488
#define HAUTEUR_FENETRE_RV 680
#define LARGEUR_PUZZLE_RV 400
#define HAUTEUR_PUZZLE_RV 526
#define COIN_X_V 44
#define COIN_Y_V 121
// menu
#define LARGEUR_FENETRE_M 1210
#define HAUTEUR_FENETRE_M 590
// image
#define LARGEUR_FENETRE_I 680 
#define HAUTEUR_FENETRE_I 680


typedef unsigned int uint;
enum{
//forme
    CARRE,  RECTANGLE_V,    RECTANGLE_H,
//decoupage_carre
    _3x3,    _4x4,    _5x5,
//decoupage_rectangle_V
    _3x4,    _4x5,
//decoupage_rectangle_H
    _4x3,    _5x4,
//image
    IMG1, IMG2, IMG3, IMG4, IMG5, IMG6, IMG7, IMG8, IMG9,
//souris lieu && clic
    MENU,   CHOIX_IMAGE,    PUZZLE,    CLIC_PUZZLE,
    RETOUR,    CROIX,    MELANGER,    TRICHER,    PUZZLER,    REJOUER,
};

typedef struct position_derniere_piece {
    uint x;
    uint y;
}pos_piece;

typedef struct info_tableau{
    uint **tableau;
    uint l_tab;
    uint h_tab;
    SDL_Surface **image;
    pos_piece position;
    uint l_puzzle;
    uint h_puzzle;
    uint forme;
}tab;

struct LxH{
    uint largeur;
    uint hauteur;
};

typedef struct forme_decoupage_image{
    uint forme;
    char* s_forme;
    char* s_decoupage_forme;
    struct LxH decoupage;
    char* s_image;
    uint l_puzzle;
    uint h_puzzle;
    uint l_f;
    uint h_f;
    char* s_boutton_appuyer;
    char* s_boutton;
} puzzle_info;


void sortie_programme_erreur(const char *message);
void afficher_image(char *name_File, SDL_Surface *image, SDL_Window *window, SDL_Renderer *renderer, uint x, uint y, uint l, uint h);
void afficher_puzzle(puzzle_info* puzzle, tab* tab_complet, SDL_Window *window, SDL_Renderer *renderer);
tab* creer_tableau_complet(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer);
void liberer_tableau_complet(tab* tab_complet);
void puzzler(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer);
//uint clic(puzzle_info* puzzle);
SDL_bool haut(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer);
SDL_bool bas(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer);
SDL_bool gauche(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer);
SDL_bool droite(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer);
void deplacement_dossier(const char* dossier);

void affichage_choix(SDL_Window* window, SDL_Renderer* renderer, uint lieu, puzzle_info* puzzle, SDL_Texture** miniature);
void affichage_menu(SDL_Window* window, SDL_Renderer* renderer);
void affichage_puzzle(puzzle_info* puzzle, SDL_Window* window, SDL_Renderer* renderer);
void apparition_image(char *nom_image, SDL_Window *window, SDL_Renderer *renderer, uint x, uint y, uint l, uint h, uint time);
void effet_enfoncer_miniature(puzzle_info* puzzle, char* image, SDL_Window* window, SDL_Renderer* renderer, uint x, uint y, uint l, uint h);
void effet_relever_miniature(puzzle_info* puzzle, char* image, SDL_Window* window, SDL_Renderer* renderer, uint x, uint y, uint l, uint h);

static Uint32 lire_couleur_aux(const SDL_Surface* surface, int x, int y);
static void lire_couleur(const SDL_Surface * surface, int x, int y, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a);
void colorier_pixel(SDL_Surface *image, uint x, uint y, SDL_Color* c);
void changer_fenetre(SDL_Window* window, SDL_Renderer* renderer, uint nouvelle_largeur, uint nouvelle_hauteur, char* image_fond);
