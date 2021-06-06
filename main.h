
// FONCTIONS PRINCIPALES //

void sortie_programme_erreur(const char *message)
{
    SDL_Log("Erreur : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
void afficher_image(char *nom_image, SDL_Surface *image, SDL_Window *window, SDL_Renderer *renderer, uint x, uint y, uint l, uint h){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    
    if(image == NULL){
        FILE* fichier;
        fichier = fopen(nom_image, "r");
        if (fichier == NULL){
            fclose(fichier);
            printf("L'image %s n'existe pas\n", nom_image);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        fclose(fichier);
        image = IMG_Load(nom_image);
        if (image == NULL){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            sortie_programme_erreur("Impossible de charger l'image");
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image); // Creation de la texture de la surface
    if(nom_image != NULL){
        SDL_FreeSurface(image);// Liberation de la surface
    }

    if (texture == NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de creer la texture");
    }
    

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    if(h!=0 && l!=0){
        rectangle.h = h;
        rectangle.w = l;
    }

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
    {   
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible d'afficher la texture");
    }
    SDL_DestroyTexture(texture);
}

void afficher_puzzle(puzzle_info* puzzle, tab* tab_complet, SDL_Window *window, SDL_Renderer *renderer){
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    uint x,y;
    switch (tab_complet->forme){
    case CARRE:
        x=COIN_X_C; y=COIN_Y_C;
        break;
    case RECTANGLE_V :
        x=COIN_X_V; y=COIN_Y_V;
        break;
    case RECTANGLE_H :
        x=COIN_X_H; y=COIN_Y_H;
        break;
    }
    SDL_Texture* piece[tab_complet->l_tab * tab_complet->h_tab];
    SDL_Rect rectangle;
    for(uint i=0; i<tab_complet->l_tab * tab_complet->h_tab; i++){
        piece[i] = SDL_CreateTextureFromSurface(renderer, tab_complet->image[i]);
        SDL_SetTextureBlendMode(piece[i], SDL_BLENDMODE_BLEND);
        if (SDL_QueryTexture(piece[i], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            sortie_programme_erreur("Impossible de charger la texture");
        }
    }
    for(uint a=1; a<=40; a++){
        for(uint i=0; i<tab_complet->h_tab; i++){
            for(uint j=0; j<tab_complet->l_tab; j++){
                rectangle.w = tab_complet->l_puzzle / tab_complet->l_tab;
                rectangle.h = tab_complet->h_puzzle / tab_complet->h_tab;
                rectangle.x = x + j*rectangle.w + (tab_complet->l_puzzle%tab_complet->l_tab)/2;
                rectangle.y = y + i*rectangle.h + (tab_complet->h_puzzle%tab_complet->h_tab)/2;
                SDL_SetTextureAlphaMod(piece[tab_complet->tableau[i][j]-1], a);
                SDL_RenderCopy(renderer, piece[tab_complet->tableau[i][j]-1], NULL, &rectangle);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    for(uint i=0; i<tab_complet->l_tab * tab_complet->h_tab; i++){
        SDL_DestroyTexture(piece[i]);
    }
    
    apparition_image(puzzle->s_boutton, window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
}

void afficher_changement_piece(tab* tab_complet, pos_piece pos, SDL_Window *window, SDL_Renderer *renderer){
    uint x,y;
    switch (tab_complet->forme){
    case CARRE:
        x=COIN_X_C; y=COIN_Y_C;
        break;
    case RECTANGLE_V :
        x=COIN_X_V; y=COIN_Y_V;
        break;
    case RECTANGLE_H :
        x=COIN_X_H; y=COIN_Y_H;
        break;
    default:
        break;
    }
    if(tab_complet->position.x >= tab_complet->l_tab || tab_complet->position.y >= tab_complet->h_tab || tab_complet->position.x<0 ||tab_complet->position.y <0){
        liberer_tableau_complet(tab_complet);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("position invalide");
    }
    afficher_image(NULL, tab_complet->image[(tab_complet->l_tab * tab_complet->h_tab)-1], window, renderer, x+(tab_complet->l_puzzle/tab_complet->l_tab)*(tab_complet->position.x) + (tab_complet->l_puzzle%tab_complet->l_tab)/2, y+(tab_complet->h_puzzle/tab_complet->h_tab)*(tab_complet->position.y) + (tab_complet->h_puzzle%tab_complet->h_tab)/2, (tab_complet->l_puzzle/tab_complet->l_tab), (tab_complet->h_puzzle/tab_complet->h_tab));
    afficher_image(NULL, tab_complet->image[(tab_complet->tableau[pos.y][pos.x])-1], window, renderer, x+(tab_complet->l_puzzle/tab_complet->l_tab)*(pos.x) + (tab_complet->l_puzzle%tab_complet->l_tab)/2, y+(tab_complet->h_puzzle/tab_complet->h_tab)*(pos.y) + (tab_complet->h_puzzle%tab_complet->h_tab)/2, (tab_complet->l_puzzle/tab_complet->l_tab), (tab_complet->h_puzzle/tab_complet->h_tab));
    SDL_RenderPresent(renderer);
}

tab* creer_tableau_complet(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer){

    // Allocation structure tab{tableau_double, largeur, hauteur, tableau_surface} //

    tab* tab_complet = (tab*) malloc(sizeof(tab));
    if(tab_complet == NULL){
        printf("Impossible de creer la structure du tableau double");
        exit(EXIT_FAILURE);
    }

    // Completer largeur-hauteur tableau + largeur-hauteur puzzle //

    tab_complet->l_tab = puzzle->decoupage.largeur;
    tab_complet->h_tab = puzzle->decoupage.hauteur;
    
    tab_complet->position.x = (tab_complet->l_tab)-1;
    tab_complet->position.y = (tab_complet->h_tab)-1;

    tab_complet->l_puzzle = puzzle->l_puzzle;
    tab_complet->h_puzzle = puzzle->h_puzzle;

    tab_complet->forme = puzzle->forme;

    // Alloctaion tab.tableau_double //

    tab_complet->tableau = (uint**) malloc(tab_complet->h_tab*sizeof(uint));
    if(tab_complet->tableau == NULL){
        liberer_tableau_complet(tab_complet);
        printf("Impossible de creer le tableau");
        exit(EXIT_FAILURE);
    }
    for(uint i=0; i<tab_complet->h_tab; i++){
        tab_complet->tableau[i] = (uint*) malloc(tab_complet->l_tab*sizeof(uint));
        if(tab_complet->tableau == NULL){
            liberer_tableau_complet(tab_complet);
            printf("Impossible de creer le tableau");
            exit(EXIT_FAILURE);
        }
    }

    // Complete tab.tableau_double //
    
    uint nombre = 1;
    for(uint x=0; x<tab_complet->h_tab; x++){
        for(uint y=0; y<tab_complet->l_tab; y++){
            tab_complet->tableau[x][y] = nombre;
            nombre++;
        }
    }

    // Allocation tab.tableau_image //
    
    tab_complet->image = (SDL_Surface **) malloc(sizeof(SDL_Surface*)*tab_complet->l_tab*tab_complet->h_tab);
    if(tab_complet->image == NULL){
        liberer_tableau_complet(tab_complet);
        printf("Impossible de creer le tableau de surface\n");
        exit(EXIT_FAILURE);
    }
    
    // Complete tab.tableau_image //
    

    SDL_Surface* image = IMG_Load("image.png");
    if(image == NULL){
        liberer_tableau_complet(tab_complet);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger l'image");
    }

    SDL_Color c;
    uint cmp=0;
    for(uint i=0; i<tab_complet->h_tab; i++){
        for(uint j=0; j<tab_complet->l_tab; j++){
            tab_complet->image[cmp] = SDL_CreateRGBSurface(0, image->w/tab_complet->l_tab, image->h/tab_complet->h_tab, 32, 0, 0, 0, 255);
            if(tab_complet->image[cmp] == NULL){
                liberer_tableau_complet(tab_complet);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                sortie_programme_erreur("Impossible de charger l'image");
            }
            if(cmp!=(tab_complet->l_tab*tab_complet->h_tab)-1){
                for(uint x=(3*image->w)/tab_complet->l_puzzle; x<image->w/tab_complet->l_tab-(3*image->w)/tab_complet->l_puzzle; x++){
                    for(uint y=(3*image->h)/tab_complet->h_puzzle; y<image->h/tab_complet->h_tab-(3*image->h)/tab_complet->h_puzzle; y++){
                        lire_couleur(image, j*(image->w/tab_complet->l_tab)+x, i*(image->h/tab_complet->h_tab)+y , &c.r, &c.g, &c.b, &c.a);
                        colorier_pixel(tab_complet->image[cmp], x, y, &c);
                    }
                }
            }
            else{
                for(uint x=(3*image->w)/tab_complet->l_puzzle; x<image->w/tab_complet->l_tab-(3*image->w)/tab_complet->l_puzzle; x++){
                    for(uint y=(3*image->h)/tab_complet->h_puzzle; y<image->h/tab_complet->h_tab-(3*image->h)/tab_complet->h_puzzle; y++){
                        lire_couleur(image, j*(image->w/tab_complet->l_tab)+x, i*(image->h/tab_complet->h_tab)+y , &c.r, &c.g, &c.b, &c.a);
                        c.r /= 10;
                        c.g /= 10;
                        c.b /= 10;
                        colorier_pixel(tab_complet->image[cmp], x, y, &c);
                    }
                }
            }
            cmp++;
        }
    }
    SDL_FreeSurface(image);    
    return tab_complet;
}

void liberer_tableau_complet(tab* tab_complet){
    for(uint i=0; i<tab_complet->l_tab; i++){
        free(tab_complet->tableau[i]);
    }
    free(tab_complet->tableau);
    for(uint i=0; i<tab_complet->l_tab*tab_complet->h_tab; i++){
        SDL_FreeSurface(tab_complet->image[i]);
    }
    free(tab_complet->image);
    free(tab_complet);
}

uint clic(uint lieu, SDL_Window *window, SDL_Renderer *renderer, SDL_bool *programme_lance, puzzle_info* puzzle){
    int x,y;
    SDL_GetMouseState(&x, &y);
    switch(lieu){
        case MENU :
            if(y>340 && y<440){
                lieu = CHOIX_IMAGE;
                if(x>53 && x<185){
                    puzzle->s_decoupage_forme = "rectangle3x2";
                    puzzle->decoupage.largeur = 3;
                    puzzle->decoupage.hauteur = 2;
                }
                else if(x>190 && x<300){
                    puzzle->s_decoupage_forme = "rectangle2x3";
                    puzzle->decoupage.largeur = 2;
                    puzzle->decoupage.hauteur = 3;
                }
                else if(x>305 && x<415){
                    puzzle->s_decoupage_forme = "carre3x3";
                    puzzle->decoupage.largeur = 3;
                    puzzle->decoupage.hauteur = 3;
                }
                else if(x>420 && x<553){
                    puzzle->s_decoupage_forme = "rectangle4x3";
                    puzzle->decoupage.largeur = 4;
                    puzzle->decoupage.hauteur = 3;
                }
                else if(x>559 && x<669){
                    puzzle->s_decoupage_forme = "rectangle3x4";
                    puzzle->decoupage.largeur = 3;
                    puzzle->decoupage.hauteur = 4;
                }
                else if(x>674 && x<784){
                    puzzle->s_decoupage_forme = "carre4x4";
                    puzzle->decoupage.largeur = 4;
                    puzzle->decoupage.hauteur = 4;
                }
                else if(x>789 && x<916){
                    puzzle->s_decoupage_forme = "rectangle5x4";
                    puzzle->decoupage.largeur = 5;
                    puzzle->decoupage.hauteur = 4;
                }
                else if(x>920 && x<1030){
                    puzzle->s_decoupage_forme = "rectangle4x5";
                    puzzle->decoupage.largeur = 4;
                    puzzle->decoupage.hauteur = 5;
                }
                else if(x>1035 && x<1146){
                    puzzle->s_decoupage_forme = "carre5x5";
                    puzzle->decoupage.largeur = 5;
                    puzzle->decoupage.hauteur = 5;
                }
                else{
                    lieu = MENU;
                }
            }
            else if(x>(LARGEUR_FENETRE_M-35) && y<35){
                *programme_lance=SDL_FALSE;
            }
            return lieu;
            break;
        case CHOIX_IMAGE :
                if(x>puzzle->l_f - 35 && y<35){
                    *programme_lance = SDL_FALSE;
                }
                else if(x<35 && y<35){
                    deplacement_dossier("..");
                    deplacement_dossier("menu");
                    affichage_menu(window, renderer);
                    return MENU;
                    break;
                }
                uint x0, y0, x1, y1, l, h;
                switch(puzzle->forme){
                case CARRE:
                    x0=COIN_X_C+1; y0=COIN_Y_C+1;
                    x1 = 180; y1 = 180;
                    l = 144; h = 144;
                    break;
                case RECTANGLE_H :
                    x0=COIN_X_H+1; y0=COIN_Y_H+1;
                    x1 = 248; y1 = 184;
                    l = 197; h = 144;
                    break;
                case RECTANGLE_V :
                    x0=COIN_X_V+1; y0=COIN_Y_V+1;
                    x1 = 143; y1 = 188;
                    l = 114; h = 150;
                    break;
                }
                if(x>x0 && x<x0+l && y>y0 && y<y0+h){
                    puzzle->s_image = "image1";
                }
                else if(x>x0+x1 && x<x0+x1+l && y>y0 && y<y0+h){
                    puzzle->s_image = "image2";
                }
                else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0 && y<y0+h){
                    puzzle->s_image = "image3";
                }
                else if(x>x0 && x<x0+l && y>y0+y1 && y<y0+y1+h){
                    puzzle->s_image = "image4";
                }
                else if(x>x0+x1 && x<x0+x1+l && y>y0+y1 && y<y0+y1+h){
                    puzzle->s_image = "image5";
                }
                else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0+y1 && y<y0+y1+h){
                    puzzle->s_image = "image6";
                }
                else if(x>x0 && x<x0+l && y>y0+2*y1 && y<y0+2*y1+h){
                    puzzle->s_image = "image7";
                }
                else if(x>x0+x1 && x<x0+x1+l && y>y0+2*y1 && y<y0+2*y1+h){
                    puzzle->s_image = "image8";
                }
                else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0+2*y1 && y<y0+2*y1+h){
                    puzzle->s_image = "image9";
                }
                else{
                    return CHOIX_IMAGE;
                }
                affichage_puzzle(puzzle, window, renderer);
                return PUZZLE;
            break;
        case PUZZLE :
            if(x>puzzle->l_f - 35 && y<35){
                *programme_lance = SDL_FALSE;
            }
            else if(x<35 && y<35){
                return CHOIX_IMAGE;
            }
            else if(x>puzzle->l_f/2-145 && x<puzzle->l_f/2+145 && y>55  && y<97){
                return CLIC_PUZZLE;
            }
            break;
    }
    return 0;
}

void souris_lieu(puzzle_info* puzzle, uint lieu, uint* affichage_en_cours, SDL_Window *window, SDL_Renderer *renderer){
    uint l,h;
    int x,y;
    SDL_GetMouseState(&x, &y);
    switch(lieu){
        case MENU :
            l = LARGEUR_FENETRE_M;
            h = HAUTEUR_FENETRE_M;
            if(y>340 && y<440){
                if(x>53 && x<185 && *affichage_en_cours != IMG1){
                    afficher_image("menu1.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG1;
                }
                else if(x>190 && x<300 && *affichage_en_cours != IMG2){
                    afficher_image("menu2.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG2;
                }
                else if(x>305 && x<415 && *affichage_en_cours != IMG3){
                    afficher_image("menu3.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG3;
                }
                else if(x>420 && x<553 && *affichage_en_cours != IMG4){
                    afficher_image("menu4.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG4;
                }
                else if(x>559 && x<669 && *affichage_en_cours != IMG5){
                    afficher_image("menu5.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG5;
                }
                else if(x>674 && x<784 && *affichage_en_cours != IMG6){
                    afficher_image("menu6.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG6;
                }
                else if(x>789 && x<916 && *affichage_en_cours != IMG7){
                    afficher_image("menu7.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG7;
                }
                else if(x>920 && x<1030 &&*affichage_en_cours != IMG8){
                    afficher_image("menu8.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG8;
                }
                else if(x>1035 && x<1146 && *affichage_en_cours != IMG9){
                    afficher_image("menu9.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = IMG9;
                }
                else if((x<53 || x>1146) && *affichage_en_cours != MENU){
                    afficher_image("menu.png", NULL, window, renderer, 0, 0, l ,h);
                    *affichage_en_cours = MENU;
                }
            }
            else if(x>(LARGEUR_FENETRE_M-35) && y<35){
                if(*affichage_en_cours != CROIX){
                afficher_image("menu_croix.png", NULL, window, renderer, 0, 0, l ,h);
                *affichage_en_cours = CROIX;
                }
            }
            else if(*affichage_en_cours != MENU){
                afficher_image("menu.png", NULL, window, renderer, 0, 0, l ,h);
                *affichage_en_cours = MENU;
            }
            SDL_RenderPresent(renderer);
            break;
        case CHOIX_IMAGE :;
            uint x0, y0, x1, y1, l, h;
            switch(puzzle->forme){
                case CARRE:
                    x0=COIN_X_C+1; y0=COIN_Y_C+1;
                    x1 = 180; y1 = 180;
                    l = 144; h = 144;
                    break;
                case RECTANGLE_H :
                    x0=COIN_X_H+1; y0=COIN_Y_H+1;
                    x1 = 248; y1 = 184;
                    l = 197; h = 144;
                    break;
                case RECTANGLE_V :
                    x0=COIN_X_V+1; y0=COIN_Y_V+1;
                    x1 = 143; y1 = 188;
                    l = 114; h = 150;
                    break;
                default :
                    break;
            }
            if(x<36 && y<39){
                if (*affichage_en_cours!=RETOUR){
                    deplacement_dossier(puzzle->s_decoupage_forme);
                    afficher_image("bande_haut_retour.png", NULL, window, renderer, 0, 0, 0, 0);
                    deplacement_dossier("..");
                    *affichage_en_cours = RETOUR;
                }
            }
            else if(x>puzzle->l_f - 36 && y<39){
                if (*affichage_en_cours!=CROIX){
                    deplacement_dossier(puzzle->s_decoupage_forme);
                    afficher_image("bande_haut_croix.png", NULL, window, renderer, 0, 0, 0, 0);
                    deplacement_dossier("..");
                    *affichage_en_cours = CROIX;
                }
            }
            else if(x>x0 && x<x0+l && y>y0 && y<y0+h){
                switch(*affichage_en_cours){
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG1){
                    effet_enfoncer_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                    *affichage_en_cours = IMG1;
                }
            }
            else if(x>x0+x1 && x<x0+x1+l && y>y0 && y<y0+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG2){
                    effet_enfoncer_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                    *affichage_en_cours = IMG2;
                }
            }
            else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0 && y<y0+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG3){
                    effet_enfoncer_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                    *affichage_en_cours = IMG3;
                }
            }
            else if(x>x0 && x<x0+l && y>y0+y1 && y<y0+y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG4){
                    effet_enfoncer_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                    *affichage_en_cours = IMG4;
                }
            }
            else if(x>x0+x1 && x<x0+x1+l && y>y0+y1 && y<y0+y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG5){
                    effet_enfoncer_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                    *affichage_en_cours = IMG5;
                }
            }
            else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0+y1 && y<y0+y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG6){
                    effet_enfoncer_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                    *affichage_en_cours = IMG6;
                }
            }
            else if(x>x0 && x<x0+l && y>y0+2*y1 && y<y0+2*y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG7){
                    effet_enfoncer_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                    *affichage_en_cours = IMG7;
                }
            }
            else if(x>x0+x1 && x<x0+x1+l && y>y0+2*y1 && y<y0+2*y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG9:
                        effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG8){
                    effet_enfoncer_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                    *affichage_en_cours = IMG8;
                }
            }
            else if(x>x0+2*x1 && x<x0+2*x1+l && y>y0+2*y1 && y<y0+2*y1+h){
                switch(*affichage_en_cours){
                    case IMG1 :
                        effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                        break;
                    case IMG2 :
                        effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                        break;
                    case IMG3 :
                        effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                        break;
                    case IMG4 :
                        effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                        break;
                    case IMG5 :
                        effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                        break;
                    case IMG6 :
                        effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                        break;
                    case IMG7 :
                        effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                        break;
                    case IMG8 :
                        effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                        break;
                }
                if(*affichage_en_cours!=IMG9){
                    effet_enfoncer_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                    *affichage_en_cours = IMG9;
                }
            }
            else{
                if(*affichage_en_cours == CROIX || *affichage_en_cours == RETOUR){
                    deplacement_dossier(puzzle->s_decoupage_forme);
                    afficher_image("bande_haut.png", NULL, window, renderer, 0, 0, 0, 0);
                    deplacement_dossier("..");
                }
                else if(*affichage_en_cours == IMG1){
                    effet_relever_miniature(puzzle, "image1", window, renderer, x0, y0, l, h);
                }
                else if(*affichage_en_cours==IMG2){
                    effet_relever_miniature(puzzle, "image2", window, renderer, x0+x1, y0, l, h);
                }
                else if(*affichage_en_cours==IMG3){
                    effet_relever_miniature(puzzle, "image3", window, renderer, x0+2*x1, y0, l, h);
                }
                else if(*affichage_en_cours==IMG4){
                    effet_relever_miniature(puzzle, "image4", window, renderer, x0, y0+y1, l, h);
                }
                else if(*affichage_en_cours==IMG5){
                    effet_relever_miniature(puzzle, "image5", window, renderer, x0+x1, y0+y1, l, h);
                }
                else if(*affichage_en_cours==IMG6){
                    effet_relever_miniature(puzzle, "image6", window, renderer, x0+2*x1, y0+y1, l, h);
                }
                else if(*affichage_en_cours==IMG7){
                    effet_relever_miniature(puzzle, "image7", window, renderer, x0, y0+2*y1, l, h);
                }
                else if(*affichage_en_cours==IMG8){
                    effet_relever_miniature(puzzle, "image8", window, renderer, x0+x1, y0+2*y1, l, h);
                }
                else if(*affichage_en_cours==IMG9){
                    effet_relever_miniature(puzzle, "image9", window, renderer, x0+2*x1, y0+2*y1, l, h);
                }
                *affichage_en_cours = CHOIX_IMAGE;
            }
            SDL_RenderPresent(renderer);
            break;
        case PUZZLE :
            if(x<36 && y<39){
                if (*affichage_en_cours!=RETOUR){
                    deplacement_dossier(puzzle->s_decoupage_forme);
                    afficher_image("bande_haut_retour.png", NULL, window, renderer, 0, 0, 0, 0);
                    deplacement_dossier("..");
                    *affichage_en_cours = RETOUR;
                }
            }
            else if(x>puzzle->l_f - 36 && y<39){
                if (*affichage_en_cours!=CROIX){
                    deplacement_dossier(puzzle->s_decoupage_forme);
                    afficher_image("bande_haut_croix.png", NULL, window, renderer, 0, 0, 0, 0);
                    deplacement_dossier("..");
                    *affichage_en_cours = CROIX;
                }
            }
            else if (x>puzzle->l_f/2-145 && x<puzzle->l_f/2+145 && y>55  && y<97){
                if(*affichage_en_cours != CLIC_PUZZLE){
                deplacement_dossier("..");
                deplacement_dossier("boutton");
                apparition_image(puzzle->s_boutton_appuyer, window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
                deplacement_dossier("..");
                deplacement_dossier(puzzle->s_forme);
                *affichage_en_cours = CLIC_PUZZLE;
                }
            }
            else if (*affichage_en_cours != PUZZLE){
                deplacement_dossier(puzzle->s_decoupage_forme);
                afficher_image("bande_haut.png", NULL, window, renderer, 0, 0, 0, 0);
                deplacement_dossier("..");
                deplacement_dossier("..");
                deplacement_dossier("boutton");
                apparition_image(puzzle->s_boutton, window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
                deplacement_dossier("..");
                deplacement_dossier(puzzle->s_forme);
                *affichage_en_cours = PUZZLE;
            }
        SDL_RenderPresent(renderer);
        break;
    }
}

SDL_bool haut(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer){
    if(tab_complet->position.y>0){
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x]) = (tab_complet->tableau[tab_complet->position.y-1][tab_complet->position.x]);
        (tab_complet->tableau[tab_complet->position.y-1][tab_complet->position.x]) = (tab_complet->l_tab * tab_complet->h_tab);
        tab_complet->position.y --;
        pos_piece pos = {tab_complet->position.x, tab_complet->position.y+1};
        afficher_changement_piece(tab_complet, pos, window, renderer);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}
SDL_bool bas(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer){
    if(tab_complet->position.y < tab_complet->h_tab-1){
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x]) = (tab_complet->tableau[tab_complet->position.y+1][tab_complet->position.x]);
        (tab_complet->tableau[tab_complet->position.y+1][tab_complet->position.x]) = (tab_complet->l_tab * tab_complet->h_tab);
        tab_complet->position.y ++;
        pos_piece pos = {tab_complet->position.x, tab_complet->position.y-1};
        afficher_changement_piece(tab_complet, pos, window, renderer);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}
SDL_bool gauche(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer){
    if(tab_complet->position.x>0){
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x]) = (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x-1]);
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x-1]) = (tab_complet->l_tab * tab_complet->h_tab);
        tab_complet->position.x --;
        pos_piece pos = {tab_complet->position.x+1, tab_complet->position.y};
        afficher_changement_piece(tab_complet, pos, window, renderer);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}
SDL_bool droite(tab* tab_complet, SDL_Window* window, SDL_Renderer* renderer){
    if(tab_complet->position.x<tab_complet->l_tab-1){
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x]) = (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x+1]);
        (tab_complet->tableau[tab_complet->position.y][tab_complet->position.x+1]) = (tab_complet->l_tab * tab_complet->h_tab);
        tab_complet->position.x ++;
        pos_piece pos = {tab_complet->position.x-1, tab_complet->position.y};
        afficher_changement_piece(tab_complet, pos, window, renderer);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

void deplacement_dossier(const char* dossier){
    if(chdir(dossier)!= 0){
        printf("Impossible d'aller dans %s\n", dossier);
        exit(EXIT_FAILURE);
    }
}

SDL_Texture** remplir_miniature(SDL_Window* window, SDL_Renderer* renderer){
    SDL_Surface* tmp = NULL;
    SDL_Rect rectangle;
    SDL_Texture** tableau_miniature = (SDL_Texture **) malloc(sizeof(SDL_Texture*)*9);
    if(tableau_miniature == NULL){
        printf("Impossible de creer le tableau de texture\n");
        exit(EXIT_FAILURE);
    }
    deplacement_dossier("image1");
    tmp = IMG_Load("image.png");
    tableau_miniature[0] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[0], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image2");
    tmp = IMG_Load("image.png");
    tableau_miniature[1] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[1], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image3");
    tmp = IMG_Load("image.png");
    tableau_miniature[2] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[2], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image4");
    tmp = IMG_Load("image.png");
    tableau_miniature[3] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[3], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image5");
    tmp = IMG_Load("image.png");
    tableau_miniature[4] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[4], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image6");
    tmp = IMG_Load("image.png");
    tableau_miniature[5] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[5], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image7");
    tmp = IMG_Load("image.png");
    tableau_miniature[6] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[6], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image8");
    tmp = IMG_Load("image.png");
    tableau_miniature[7] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[7], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    deplacement_dossier("image9");
    tmp = IMG_Load("image.png");
    tableau_miniature[8] = SDL_CreateTextureFromSurface(renderer, tmp);
    if (SDL_QueryTexture(tableau_miniature[8], NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    deplacement_dossier("..");
    return tableau_miniature;
}

void liberer_miniature(SDL_Texture** miniature){
    for(uint i=0; i<9; i++){
        SDL_DestroyTexture(miniature[i]);
    }
    free(miniature);
}

void effet_enfoncer_miniature(puzzle_info* puzzle, char* image, SDL_Window* window, SDL_Renderer* renderer, uint x, uint y, uint l, uint h){
    deplacement_dossier(image);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, l, h, 32, 0, 0, 0, 255);
    SDL_Texture* texture_noire = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture_noire, 0, 0, NULL, NULL);
    surface = IMG_Load("image.png");
    SDL_Texture* texture_miniature = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture_miniature, 0, 0, 0, 0);
    SDL_FreeSurface(surface);
    SDL_Rect rectangle1 = {x-2, y-2, l+2, h+2};
    SDL_Rect rectangle2 = {x-2, y-2, l+2, h+2};
    for(float i = 1; i>=0.9; i-=0.02){
        rectangle2.w = rectangle1.w*i; 
        rectangle2.h = rectangle1.h*i;
        rectangle2.x = x -2 + ((rectangle1.w-rectangle2.w)/2);
        rectangle2.y = y -2 + ((rectangle1.h-rectangle2.h)/2);
        SDL_RenderCopy(renderer, texture_noire, NULL, &rectangle1);
        SDL_RenderCopy(renderer, texture_miniature, NULL, &rectangle2);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(texture_noire);
    SDL_DestroyTexture(texture_miniature);
    deplacement_dossier("..");
}

void effet_relever_miniature(puzzle_info* puzzle, char* image, SDL_Window* window, SDL_Renderer* renderer, uint x, uint y, uint l, uint h){
    deplacement_dossier(image);
    SDL_Surface* surface = IMG_Load("image.png");
    SDL_Texture* texture_miniature = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture_miniature, 0, 0, 0, 0);
    SDL_FreeSurface(surface);
    SDL_Rect rectangle1 = {x-2, y-2, l+3, h+3};
    SDL_Rect rectangle2 = {x-2, y-2, l+2, h+2};
    for(float i = 0.9; i<=1; i+=0.02){
        rectangle2.w = rectangle1.w*i; 
        rectangle2.h = rectangle1.h*i;
        rectangle2.x = x -2 + ((rectangle1.w-rectangle2.w)/2);
        rectangle2.y = y -2 + ((rectangle1.h-rectangle2.h)/2);
        SDL_RenderCopy(renderer, texture_miniature, NULL, &rectangle2);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(texture_miniature);
    deplacement_dossier("..");
}

void affichage_choix(SDL_Window* window, SDL_Renderer* renderer, uint lieu, puzzle_info* puzzle, SDL_Texture** miniature){
    float i = 1;
    uint x0, y0, x1, y1, l, h;
    SDL_Rect rectangle;
    switch(puzzle->forme){
        case CARRE:
            x0=COIN_X_C+1; y0=COIN_Y_C+1;
            x1 = 180; y1 = 180;
            l = 144; h = 144;
            break;
        case RECTANGLE_H :
            x0=COIN_X_H+1; y0=COIN_Y_H+1;
            x1 = 248; y1 = 184;
            l = 197; h = 144;
            break;
        case RECTANGLE_V :
            x0=COIN_X_V+1; y0=COIN_Y_V+1;
            x1 = 143; y1 = 188;
            l = 114; h = 150;
            break;
    }
    if(lieu == MENU){
    while(i>=0){
        i-=0.01;
        SDL_SetWindowOpacity(window, i);
        SDL_Delay(5);
    }
    SDL_SetWindowSize(window, puzzle->l_f, puzzle->h_f);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    deplacement_dossier(puzzle->s_decoupage_forme);
    afficher_image("bande_haut.png", NULL, window, renderer, 0, 0, 0, 0);
    deplacement_dossier("..");
    afficher_image("choix_image.png", NULL, window, renderer, 0, 40, 0, 0);
    }
    else if(lieu == PUZZLE){
    SDL_Rect rectangle1={.x = 0, .y = 40};
    SDL_Surface* image = IMG_Load("choix_image.png");
    //deplacement_dossier("..");
    if(image == NULL){
        sortie_programme_erreur("impossible de charger l'image choix_image.png");
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle1.w, &rectangle1.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    for(uint i=1; i<=40; i++){
        SDL_SetTextureAlphaMod(texture, i);
        SDL_RenderCopy(renderer, texture, NULL, &rectangle1);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(texture);
    }
    i=0;
    while(i<=1){
        i+=0.02;
        if(lieu == MENU){
            SDL_SetWindowOpacity(window, i);
        }
        uint cmp=0;
        for(uint a=0; a<3; a++){
            for(uint b=0; b<3; b++){
                rectangle.w = i*l;
                rectangle.h = i*h;
                rectangle.x = x0+(1-i)*(l)/2 + b*x1;
                rectangle.y = y0+(1-i)*(h)/2 + a*y1;
                SDL_RenderCopy(renderer, miniature[cmp], NULL, &rectangle);
                cmp++;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(8);
    }
    deplacement_dossier(puzzle->s_decoupage_forme);
    afficher_image("bande_haut.png", NULL, window, renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
    deplacement_dossier("..");
}

void affichage_menu(SDL_Window* window, SDL_Renderer* renderer){
    float i=1;
    while(i>=0){
        i-=0.01;
        SDL_SetWindowOpacity(window, i);
        SDL_Delay(5);
    }
    SDL_SetWindowSize(window, LARGEUR_FENETRE_M, HAUTEUR_FENETRE_M);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    afficher_image("menu.png", NULL, window, renderer, 0, 0, LARGEUR_FENETRE_M, HAUTEUR_FENETRE_M);
    SDL_RenderPresent(renderer);
    while(i<=1){
        i+=0.01;
        SDL_SetWindowOpacity(window, i);
        SDL_Delay(5);
    }
}

void affichage_puzzle(puzzle_info* puzzle, SDL_Window* window, SDL_Renderer* renderer){
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
    SDL_Rect rectangle = {.x = 0, .y = 40};
    SDL_Surface* image = IMG_Load("puzzle.png");
    if(image == NULL){
        sortie_programme_erreur("impossible de charger l'image puzzle.png");
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    for(uint i=0; i<=40; i++){
        SDL_SetTextureAlphaMod(texture, i);
        SDL_RenderCopy(renderer, texture, NULL, &rectangle);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(texture);

}

void melanger_puzzle(puzzle_info* puzzle, tab* tab_complet, SDL_Window *window, SDL_Renderer *renderer){
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    srand(time(NULL));
    for (uint i=0; i<tab_complet->l_tab*tab_complet->h_tab*15; i++){
        switch(rand()%4){
            case 0:
                if(bas(tab_complet, window, renderer)){
                    i++;
                }
                i--;
                break;
            case 1:
                if(haut(tab_complet, window, renderer)){
                    i++;
                }
                i--;
                break;
            case 2:
                if(gauche(tab_complet, window, renderer)){
                    i++;
                }
                i--;
                break;
            case 3:
                if(droite(tab_complet, window, renderer)){
                    i++;
                }
                i--;
                break;
        }
        SDL_Delay(3000/(tab_complet->l_tab*tab_complet->h_tab*15));
    }
    apparition_image("tricher.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
}

void tricher(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer){
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    uint x,y;
    switch (puzzle->forme){
    case CARRE:
        x=COIN_X_C; y=COIN_Y_C;
        break;
    case RECTANGLE_V :
        x=COIN_X_V; y=COIN_Y_V;
        break;
    case RECTANGLE_H :
        x=COIN_X_H; y=COIN_Y_H;
        break;
    default:
        break;
    }
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
    deplacement_dossier(puzzle->s_image);
    apparition_image("image.png", window, renderer, x, y, puzzle->l_puzzle, puzzle->h_puzzle, 480);
    deplacement_dossier("..");
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("puzzler.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
}

void gagner(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer){
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    SDL_Rect rectangle1, rectangle2, rectangle3, rectangle4, rectangle5;
    switch (puzzle->forme){
    case CARRE:
        rectangle1.x=COIN_X_C;     rectangle1.y=COIN_Y_C;      rectangle1.w=puzzle->l_puzzle;      rectangle1.h=puzzle->h_puzzle;
        rectangle2.x=COIN_X_C;     rectangle2.y=COIN_Y_C;      rectangle2.w=puzzle->l_puzzle/2;      rectangle2.h=puzzle->h_puzzle/2;
        rectangle3.x=COIN_X_C;     rectangle3.y=COIN_Y_C+puzzle->h_puzzle/2;       rectangle3.w=puzzle->l_puzzle/2;       rectangle3.h=puzzle->h_puzzle/2;
        rectangle4.x=COIN_X_C+puzzle->l_puzzle/2;     rectangle4.y=COIN_Y_C;     rectangle4.w=puzzle->l_puzzle/2;       rectangle4.h=puzzle->h_puzzle/2;
        rectangle5.x=COIN_X_C+puzzle->l_puzzle/2;     rectangle5.y=COIN_Y_C+puzzle->h_puzzle/2;    rectangle5.w=puzzle->l_puzzle/2;   rectangle5.h=puzzle->h_puzzle/2;
        break;
    case RECTANGLE_V :
        rectangle1.x=COIN_X_V;     rectangle1.y=COIN_Y_V;      rectangle1.w=puzzle->l_puzzle;      rectangle1.h=puzzle->h_puzzle;
        rectangle2.x=COIN_X_V;     rectangle2.y=COIN_Y_V+1;      rectangle2.w=puzzle->l_puzzle;      rectangle2.h=puzzle->h_puzzle/2;
        rectangle3.x=COIN_X_V;     rectangle3.y=COIN_Y_V+puzzle->h_puzzle/2;       rectangle3.w=puzzle->l_puzzle;       rectangle3.h=puzzle->h_puzzle/2;
        break;
    case RECTANGLE_H :
        rectangle1.x=COIN_X_H;     rectangle1.y=COIN_Y_H;      rectangle1.w=puzzle->l_puzzle;      rectangle1.h=puzzle->h_puzzle;
        rectangle2.x=COIN_X_H;     rectangle2.y=COIN_Y_H;      rectangle2.w=puzzle->l_puzzle/2;      rectangle2.h=puzzle->h_puzzle/2;
        rectangle3.x=COIN_X_H;     rectangle3.y=COIN_Y_H+puzzle->h_puzzle/2;       rectangle3.w=puzzle->l_puzzle/2;       rectangle3.h=puzzle->h_puzzle/2;
        rectangle4.x=COIN_X_H+puzzle->l_puzzle/2;     rectangle4.y=COIN_Y_H;     rectangle4.w=puzzle->l_puzzle/2;       rectangle4.h=puzzle->h_puzzle/2;
        rectangle5.x=COIN_X_H+puzzle->l_puzzle/2;     rectangle5.y=COIN_Y_H+puzzle->h_puzzle/2;    rectangle5.w=puzzle->l_puzzle/2;   rectangle5.h=puzzle->h_puzzle/2;
        break;
    default:
        break;
    }
    SDL_Texture** feu = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*130);
    if(feu == NULL){
        sortie_programme_erreur("impossiblde d'allouer unn tableau de surface pour le feu");
    }
    SDL_Surface* tmp;
    uint cmp = 0;
    for(uint i=0; i<13; i++){
        deplacement_dossier("feu");
        tmp = IMG_Load("feu_1.gif");
        feu[0+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_2.gif");
        feu[1+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_3.gif");
        feu[2+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_4.gif");
        feu[3+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_5.gif");
        feu[4+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_6.gif");
        feu[5+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_7.gif");
        feu[6+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_8.gif");
        feu[7+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_9.gif");
        feu[8+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        tmp = IMG_Load("feu_10.gif");
        feu[9+cmp] = SDL_CreateTextureFromSurface(renderer, tmp);
        cmp+=10;
    }
    for(uint i=0; i<126; i++){
        SDL_QueryTexture(feu[i], NULL, NULL, NULL, NULL);
    }
    SDL_DestroyTexture(feu[126]);
    SDL_DestroyTexture(feu[127]);
    SDL_DestroyTexture(feu[128]);
    SDL_DestroyTexture(feu[129]);
    for(uint i=0; i<13; i++){
        deplacement_dossier("..");
    }
    deplacement_dossier(puzzle->s_forme);
    deplacement_dossier(puzzle->s_image);
    apparition_image("image.png", window, renderer, rectangle1.x, rectangle1.y, rectangle1.w, rectangle1.h, 240);
    tmp = IMG_Load("image.png");
    SDL_Texture* tmp2 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    // Apparition du feu
    for(uint i=0; i<=200; i+=2){
        SDL_SetTextureBlendMode(feu[0], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(feu[0], i);
        SDL_RenderCopy(renderer, tmp2, NULL, &rectangle1);
        SDL_RenderCopyEx(renderer, feu[0], NULL, &rectangle2, 180, NULL, 0);
        SDL_RenderCopy(renderer, feu[0], NULL, &rectangle3);
        if(puzzle->forme != RECTANGLE_V){
            SDL_RenderCopyEx(renderer, feu[0], NULL, &rectangle4, 180, NULL, 0);
            SDL_RenderCopy(renderer, feu[0], NULL, &rectangle5);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(feu[0]);
    SDL_QueryTexture(tmp2, NULL, NULL, NULL, NULL);
    // Depart feu
    for(uint a=1; a<125; a++){
        SDL_SetTextureBlendMode(feu[a], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(feu[a], 200);
        SDL_RenderCopy(renderer, tmp2, NULL, &rectangle1);
        SDL_RenderCopyEx(renderer, feu[a], NULL, &rectangle2, 180, NULL, 0);
        SDL_RenderCopy(renderer, feu[a], NULL, &rectangle3);
        if(puzzle->forme != RECTANGLE_V){
            SDL_RenderCopyEx(renderer, feu[a], NULL, &rectangle4, 180, NULL, 0);
            SDL_RenderCopy(renderer, feu[a], NULL, &rectangle5);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
        SDL_DestroyTexture(feu[a]);
    }
    // Fin feu et ReApparirition puzzle complet
    for(uint i=200; i>0; i-=2){
        SDL_SetTextureBlendMode(feu[125], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(feu[125], i);
        SDL_RenderCopy(renderer, tmp2, NULL, &rectangle1);
        SDL_RenderCopyEx(renderer, feu[125], NULL, &rectangle2, 180, NULL, 0);
        SDL_RenderCopy(renderer, feu[125], NULL, &rectangle3);
        if(puzzle->forme != RECTANGLE_V){
            SDL_RenderCopyEx(renderer, feu[125], NULL, &rectangle4, 180, NULL, 0);
            SDL_RenderCopy(renderer, feu[125], NULL, &rectangle5);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
    SDL_DestroyTexture(feu[125]);
    SDL_DestroyTexture(tmp2);
    free(feu);
    deplacement_dossier("..");
    deplacement_dossier("..");
    deplacement_dossier("boutton");
    apparition_image(puzzle->s_boutton, window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
    deplacement_dossier("..");
    deplacement_dossier(puzzle->s_forme);
}

void apparition_image(char *nom_image, SDL_Window *window, SDL_Renderer *renderer, uint x, uint y, uint l, uint h, uint time){
    SDL_Rect rectangle = {.x = x, .y = y};
    SDL_Surface* image = IMG_Load(nom_image);
    if(image == NULL){
        sortie_programme_erreur("impossible de charger l'image boutton");
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        sortie_programme_erreur("Impossible de charger la texture");
    }
    if(l!=0 && h!=0){
        rectangle.w = l;
        rectangle.h = h;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    for(uint i=0; i<=60; i++){
        SDL_SetTextureAlphaMod(texture, i);
        SDL_RenderCopy(renderer, texture, NULL, &rectangle);
        SDL_RenderPresent(renderer);
        SDL_Delay(time/60);
    }
    SDL_DestroyTexture(texture);
    afficher_image(nom_image, NULL, window, renderer, x, y, l, h);
    SDL_RenderPresent(renderer);
}

void puzzler(puzzle_info* puzzle, SDL_Window *window, SDL_Renderer *renderer){
    tab* tab_complet;
    SDL_bool puzzle_lance = SDL_TRUE;
    SDL_Event event;
    uint valeur_clic;
    int nombre_clic=0;
    uint affichage_en_cours = PUZZLE;
    puzzle->s_boutton = "melanger.png";
    puzzle->s_boutton_appuyer = "melanger_appuyer.png";
    deplacement_dossier(puzzle->s_image);
    tab_complet = creer_tableau_complet(puzzle, window, renderer);
    deplacement_dossier("..");
    afficher_puzzle(puzzle, tab_complet, window, renderer);
    while(puzzle_lance){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT :
                    liberer_tableau_complet(tab_complet);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    exit(EXIT_SUCCESS);
                    break;
                case SDL_MOUSEMOTION : 
                    souris_lieu(puzzle, PUZZLE, &affichage_en_cours, window, renderer);
                    break;
                case SDL_MOUSEBUTTONUP :
                    valeur_clic = clic(PUZZLE, window, renderer, &puzzle_lance, puzzle);
                    if(puzzle_lance == SDL_FALSE){
                        liberer_tableau_complet(tab_complet);
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        exit(EXIT_SUCCESS);
                    }
                    if(valeur_clic == CHOIX_IMAGE){
                        liberer_tableau_complet(tab_complet);
                        puzzle_lance = SDL_FALSE;
                    }
                    else if(valeur_clic == CLIC_PUZZLE){
                        if(nombre_clic == -1){
                            puzzle->s_boutton = "melanger.png";
                            puzzle->s_boutton_appuyer = "melanger_appuyer.png";
                            deplacement_dossier("..");
                            deplacement_dossier("boutton");
                            apparition_image("blanc.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
                            deplacement_dossier("..");
                            deplacement_dossier(puzzle->s_forme);
                            afficher_puzzle(puzzle, tab_complet, window, renderer);
                            deplacement_dossier("..");
                            deplacement_dossier("boutton");
                            apparition_image("melanger.png", window, renderer, puzzle->l_f/2-145, 58, 0, 0, 120);
                            deplacement_dossier("..");
                            deplacement_dossier(puzzle->s_forme);
                        }
                        else if(nombre_clic == 0){
                            puzzle->s_boutton = "tricher.png";
                            puzzle->s_boutton_appuyer = "tricher_appuyer.png";
                            melanger_puzzle(puzzle,tab_complet, window, renderer);
                        }
                        else if(nombre_clic % 2 == 1){
                            puzzle->s_boutton = "puzzler.png";
                            puzzle->s_boutton_appuyer = "puzzler_appuyer.png";
                            tricher(puzzle, window, renderer);
                        }
                        else{
                            puzzle->s_boutton = "tricher.png";
                            puzzle->s_boutton_appuyer = "tricher_appuyer.png";
                            afficher_puzzle(puzzle, tab_complet, window, renderer);
                        }
                        nombre_clic++;
                        affichage_en_cours = PUZZLE;
                    }
                    break;
                case SDL_KEYDOWN :
                    if(nombre_clic>0 && nombre_clic % 2 ==1){
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT :
                            gauche(tab_complet, window, renderer);
                            break;
                        case SDLK_RIGHT :
                            droite(tab_complet, window, renderer);
                            break;
                        case SDLK_UP :
                            haut(tab_complet, window, renderer);
                            break;
                        case SDLK_DOWN :
                            bas(tab_complet, window, renderer);
                            break;
                        default :
                            break;
                    }
                    }
                    break;
                default :
                    break;
            }
            if(nombre_clic > 0 && puzzle_lance){
                SDL_bool gagne = SDL_TRUE;
                uint cmp=1;
                for(uint i=0; i<puzzle->decoupage.hauteur; i++){
                    for(uint j=0; j<puzzle->decoupage.largeur; j++){
                        if(tab_complet->tableau[i][j] != cmp){
                            gagne = SDL_FALSE;
                        }
                        cmp++;
                    }
                }
                if(gagne){
                    puzzle->s_boutton = "rejouer.png";
                    puzzle->s_boutton_appuyer = "rejouer_appuyer.png";
                    gagner(puzzle, window, renderer);
                    nombre_clic = -1;
                }
            }
        }
    }
}


void colorier_pixel(SDL_Surface *image, uint x, uint y, SDL_Color* c){
    Uint32* pixels=(Uint32*)image->pixels;
    Uint32* pixel=pixels+y*image->pitch/4+x;
    *pixel = SDL_MapRGBA(image->format, c->r, c->g, c->b, c->a);
}

static Uint32 lire_couleur_aux(const SDL_Surface* surface, int x, int y){

    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
 
    switch(bpp){
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
  }
}

static void lire_couleur(const SDL_Surface * surface, int x, int y, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a){
    SDL_GetRGBA(lire_couleur_aux(surface, x, y), surface->format, (Uint8*)   r, (Uint8*) g, (Uint8*) b, (Uint8*) a);
}

