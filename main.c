
#include "prototype.h"
#include "main.h"

//gcc -Wall src/main.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

int main(int argc, char** argv){
    // Initialisation variable pour la fenetre et le rendu
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        sortie_programme_erreur("Lancement SDL");
    // Creation et ouverture fenetre
    window = SDL_CreateWindow("Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_FENETRE_M, HAUTEUR_FENETRE_M, SDL_WINDOW_BORDERLESS);
    if(window == NULL)
        sortie_programme_erreur("Creation et ouverture de la fenetre");
    // Creation du rendu
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL){
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Creation du rendu");
    }

    //  *********  CEST  PARTI  **********  //
    deplacement_dossier("..");
    deplacement_dossier("src/menu");
    afficher_image("menu.png", NULL, window, renderer, 0, 0, LARGEUR_FENETRE_M, HAUTEUR_FENETRE_M);

    SDL_RenderPresent(renderer);
    uint lieu_apres = MENU;
    uint lieu_avant ;
    uint affichage_en_cours = MENU;
    SDL_Texture** miniature;
    puzzle_info puzzle = {.s_boutton = "melanger.png", .s_boutton_appuyer = "melanger_appuyer.png"};
    SDL_bool programme_lance = SDL_TRUE;
    SDL_Event event;
    while(programme_lance){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT :
                    programme_lance = SDL_FALSE;
                    break;
                case SDL_MOUSEMOTION : 
                    souris_lieu(&puzzle, lieu_apres, &affichage_en_cours, window, renderer);
                    break;
                case SDL_MOUSEBUTTONUP :
                    lieu_avant = lieu_apres;
                    lieu_apres = clic(lieu_apres, window, renderer, &programme_lance, &puzzle);
                    if(lieu_apres == PUZZLE){
                        puzzler(&puzzle, window, renderer);
                        lieu_apres = CHOIX_IMAGE;
                        affichage_choix(window, renderer, PUZZLE, &puzzle, miniature);
                    }
                    if(lieu_avant == MENU && lieu_apres == CHOIX_IMAGE){
                        deplacement_dossier("..");
                        if(puzzle.decoupage.largeur == puzzle.decoupage.hauteur){
                            puzzle.forme=CARRE;
                            puzzle.s_forme = "carre";
                            puzzle.l_f = LARGEUR_FENETRE_C;
                            puzzle.h_f = HAUTEUR_FENETRE_C;
                            puzzle.l_puzzle = LARGEUR_PUZZLE_C;
                            puzzle.h_puzzle = HAUTEUR_PUZZLE_C;
                        }
                        else if(puzzle.decoupage.largeur < puzzle.decoupage.hauteur){
                            puzzle.forme=RECTANGLE_V;
                            puzzle.s_forme = "rectangle_v";
                            puzzle.l_f = LARGEUR_FENETRE_RV;
                            puzzle.h_f = HAUTEUR_FENETRE_RV;
                            puzzle.l_puzzle = LARGEUR_PUZZLE_RV;
                            puzzle.h_puzzle = HAUTEUR_PUZZLE_RV;
                        }
                        else{
                            puzzle.forme=RECTANGLE_H;
                            puzzle.s_forme = "rectangle_h";
                            puzzle.l_f = LARGEUR_FENETRE_RH;
                            puzzle.h_f = HAUTEUR_FENETRE_RH;
                            puzzle.l_puzzle = LARGEUR_PUZZLE_RH;
                            puzzle.h_puzzle = HAUTEUR_PUZZLE_RH;
                        }
                        deplacement_dossier(puzzle.s_forme);
                        miniature = remplir_miniature(window, renderer);
                        affichage_choix(window, renderer, MENU, &puzzle, miniature);
                    }
                    if(lieu_apres == MENU){
                        liberer_miniature(miniature);
                    }
                default:
                    break;
            }
        }
    }

    // ********  CEST  FINI  ************ //

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}