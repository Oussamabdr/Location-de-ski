
/* ****************************************************************** */
/* Programme gestion de location de ski                               */                                    
/* Ce programme  permet la gestion des locations de ski               */
/* Réalisé par Oussama Bedraoui et Idriss Ziadi                       */
/* Avril 2023                                                         */
/* Programme pouvant s'exécuter sur Windows/Linux                     */
/* Compilation en ligne de commande :                                 */
/* gcc -o locski locski.c                                             */
/* ****************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>


/* ------------------------------------------------------------------ */
/*                  Macro pour l'affichage                            */
/* ------------------------------------------------------------------ */
#define clrscr() printf("\033[2J\033[H")
#define couleur(param) printf("\033[%sm",param)
#define gotoxy(row, col) printf("\033[%d;%dH", row, col)
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*                          Les paramètres                            */
/* ------------------------------------------------------------------ */
#define NRESMAX 100        // Le nombre de reservation max
#define NSK 200            // Le nombre maximum de skieurs
#define NBPAIRESKIMAX 100  // Le nombre maximum de reservations/Client
#define NAME_SIZE 26       // La longueur maximum d'un nom de client
#define ADDR_SIZE 49       // La longueur maximum d'une adresse
/* -----------------------------------------------------------------  */


/* ------------------------------------------------------------------ */
/*                 Défintion du type locataire                        */
/* ------------------------------------------------------------------ */
typedef struct {
  char nom[NAME_SIZE];
  char adresse[ADDR_SIZE];
  unsigned int psk;
} locataire_t;
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*      Tableau global des locataires et le nombre des locataires     */
/* ------------------------------------------------------------------ */
locataire_t skieur[NSK];
int nbskieur = 0;
/* ------------------------------------------------------------------ */



/* ------------------------------------------------------------------ */
/*                 Les constantes pour le menu                        */
/* ------------------------------------------------------------------ */
enum menu {CREER_SKIS=1,CREER_SKIS_SAISI, AJOUTER_SKI, SUPPRIMER_SKI,
  SUPPRIMER_TOUS_SKIS, AFFICHER_SKIS, AFFICHER_NBJ_LOC,
  CREER_RES_ALEA, CREER_RES_SAISI, AJOUTER_RES, AFFICHER_RES_NOM,
  AFFICHER_RES_TAILLE, AFFICHER_TOUTES_RES,
  ANNULER_TOUTES_RES_CLIENT, ANNULER_TOUTES_RES,AFFICHER_TAB_SKIEUR,
  QUITTER};



typedef struct {
  unsigned short deb;
  unsigned short fin;
  unsigned int taille;
} element_t;


/* ------------------------------------------------------------------ */
/*              Structure de données d'une reservation                */
/* ------------------------------------------------------------------ */
typedef struct _reservation_t {
  unsigned short deb;
  unsigned short fin;
  unsigned int ind;
}  reservation_t;
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*              Structure de données d'une paire de skis              */
/* ------------------------------------------------------------------ */
typedef struct {
  unsigned short taille;
  reservation_t *loc;
}  paireski_t;
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*              Définition du type pointeur vers réservation          */
/* ------------------------------------------------------------------ */
typedef reservation_t* ptrreservation_t;
/* ------------------------------------------------------------------ */



/* ------------------------------------------------------------------ */
/*              Définition d'un type de données générique             */
/*          qui peut être une paire de ski ou bien une réservation    */
/* ------------------------------------------------------------------ */
typedef union {
  paireski_t paireski;
	reservation_t reservation;
} data_t;
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*              Définition du type liste simplement chainée           */
/* ------------------------------------------------------------------ */
typedef struct _liste_ {
  data_t data;
  struct _liste_ *suiv;
} liste_t;
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*              Définition du type pointeur vers lsite_t              */
/* ------------------------------------------------------------------ */
typedef liste_t* ptrliste_t;
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*               Déclaration des fonctions                            */
/* ------------------------------------------------------------------ */
char *getRandomName();
char *getRandomAddress();
unsigned short getRandomSize();
void generateReservation(unsigned short *deb, unsigned short *fin);
void quitErreur(const char* msg);
void lireChaine(char *nom, const char *msg);
unsigned short lireTaille(void);


void liberer(ptrliste_t p);
void allouer(ptrliste_t *p);
void affecterValeur(ptrliste_t p, data_t d);
ptrliste_t suivant(ptrliste_t p);
data_t valeur(ptrliste_t p);
bool estVide(ptrliste_t p);
void affecterAdr(ptrliste_t p, ptrliste_t suiv);
void viderListe(ptrliste_t *L);

short int valeurTaille(ptrliste_t p);
ptrliste_t valeurLoc(ptrliste_t p);
short int valeurDebut(ptrliste_t p);
short int valeurFin(ptrliste_t p);
short int valeurInd(ptrliste_t p);

void ajouterPaireSki(ptrliste_t *L, paireski_t ps);
void supprimerPaireSki(ptrliste_t *L, short int taille);
void creerListePairesSki(ptrliste_t *L, int alea);
paireski_t creerUnePaire(int alea);
void creerListePairesSki(ptrliste_t *L, int alea);
void afficherPairesSki(ptrliste_t L);
ptrliste_t accessTaille(ptrliste_t L, unsigned short taille);

int cherchskieur(char *nom);

void afficherReservationClient(ptrliste_t pp, unsigned short ind);
void afficheToutesReservationsClient(ptrliste_t L, char *nom);
void afficheReservationParTaille(ptrliste_t L, unsigned short taille);
void afficherReservation(ptrliste_t pp);
void afficheToutesReservations(ptrliste_t L);
bool estIntersectionVide(ptrliste_t pp, unsigned short d,
    unsigned short f);


void creerListeReservation(ptrliste_t L, int alea);
void ajouterUneReservation(ptrliste_t L, int alea);
void supprimerToutesReservation(ptrliste_t L);
void annuleres(ptrliste_t plist, char *nom);
int nbjloc(ptrliste_t L, short int taille);
void AfficherTableSkieur();
int afficheMenu(void);
/* ------------------------------------------------------------------ */



/* ------------------------------------------------------------------ */
/*                   FONCTION PRINCIPALE                              */
/* ------------------------------------------------------------------ */
int main(){
  ptrliste_t  L=NULL;
  char nom[NAME_SIZE];
  int choix;
  short int taille;
  paireski_t ps;

  /* Initialisation du générateur pseudo aléatoire */
  srand((unsigned) time(NULL));
  while (1) {
    choix=afficheMenu();
    clrscr();
    switch(choix){
    case CREER_SKIS:
          couleur("33");
          gotoxy(3,8);
          creerListePairesSki(&L, 1);
          gotoxy(5,8);
          couleur("32");printf("[+] La liste a ete creee\n");
          couleur("0");
          break;
    case CREER_SKIS_SAISI:
          couleur("33");
          gotoxy(3,8);
          creerListePairesSki(&L, 0);
          gotoxy(5,8);
          couleur("32");printf("[+] La liste a ete creee\n");
          couleur("0");
          break;
    case  AJOUTER_SKI:
          couleur("33");
          ps = creerUnePaire(0);
          ajouterPaireSki(&L, ps);
          couleur("32");printf("[+] La paire a ete ajoutee\n");
          couleur("0");
          break;

    case  SUPPRIMER_SKI:
          couleur("33");
          taille = lireTaille();
          supprimerPaireSki(&L, taille);
          couleur("32");printf("[+] La paire a ete supprimee\n");
          couleur("0");
          break;

    case  SUPPRIMER_TOUS_SKIS:
          viderListe(&L);
          couleur("32");printf("[+] Toutes la paires ont ete supprimees\n");
          couleur("0");
          break;

    case  AFFICHER_SKIS:
          couleur("33");
          afficherPairesSki(L);
          couleur("0");
          break;

    case  CREER_RES_ALEA:
          couleur("33");
          creerListeReservation(L, 1);
          couleur("0");
          break;
    case CREER_RES_SAISI:
          couleur("33");
          creerListeReservation(L, 0);
          couleur("0");
          break;

    case AJOUTER_RES:
          couleur("33");
          ajouterUneReservation(L, 0);
          couleur("0");
          break;

    case AFFICHER_RES_NOM:
          lireChaine(nom,"Donnez le nom du client : ");
          couleur("33");
          afficheToutesReservationsClient(L, nom);
          couleur("0");
          break;

    case AFFICHER_RES_TAILLE:
          couleur("33");
          afficheReservationParTaille(L, lireTaille());
          couleur("0");
          break;

    case AFFICHER_TOUTES_RES:
          couleur("33");
          afficheToutesReservations(L);
          couleur("0");
          break;

    case ANNULER_TOUTES_RES_CLIENT:
          lireChaine(nom,"Donnez le nom du client : ");
          annuleres(L, nom);
          couleur("33");printf("[+] Toutes les reservations de %s ont ete annulees\n",nom);couleur("0");
          break;

    case ANNULER_TOUTES_RES:
          couleur("33");
          supprimerToutesReservation(L);
          printf("[+] Toutes les reservations ont ete annulees\n");
          couleur("0");
          break;

    case AFFICHER_NBJ_LOC:
          couleur("33");
          taille = lireTaille();
          printf("Le nombre de jours reserves pour les paires de taille %d est %d\n", taille, nbjloc(L,taille));
          couleur("0");
          break;
    case AFFICHER_TAB_SKIEUR:
          couleur("33");
          AfficherTableSkieur();
          couleur("0");
          break;
    case  QUITTER:
          return EXIT_SUCCESS;


    default:
          printf("choix incorrect \n");
    }

    printf("\n\n\n\nAppuyez sur une touche pour continuer ");
    getchar();

  }

  return EXIT_SUCCESS;
}
/* ------------------------------------------------------------------ */




/* ------------------------------------------------------------------ */
/*                    Définitions de fonction                         */
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*              retourne un nom choisi aléatoirement à partir         */
/*                    d'un tableau prédéfini de noms                  */
/* ------------------------------------------------------------------ */
char *getRandomName(){
  char *nom[] ={ "YASMINE", "MOHAMED", "AMINA", "AMINE", "SARAH",
    "KARIM", "IMANE", "YACINE", "INES", "ALI", "MERIEM", "AHMED",
    "LINA", "MEHDI", "MANEL", "LYES", "SOFIA", "NAZIM", "AMEL",
    "WALID", "SABRINA", "IDRISS", "DJAMEL", "AMIRA", "HAKIM", "MINA",
    "OMAR", "AYA", "KAMEL", "MARIA", "SAMIR", "LYNDA", "NASSIM",
    "CAMELIA", "ABDOU", "LYDIA", "BRAHIM", "NOUR", "RACHID", "ANAIS",
    "HAMZA", "FERIEL", "ZAKI", "KATIA", "SALIM", "SOPHIA", "YOUCEF",
    "RYMA", "HOCINE", "LINDA", "FARES", "NESRINE", "RAYAN", "MIMI",
    "ISLAM", "LILIA", "RABAH", "CELIA", "HICHEM", "LISA", "FATEH",
    "RANIA", "SALAH", "MELISSA", "AMED", "MIRA", "MOURAD", "IKRAM",
    "YOUNES", "HIBA", "NADIR", "CHANEZ", "ANIS", "SELMA", "OUAIL",
    "LILI", "ABDELKADER", "KHADIDJA", "WASSIM", "NINA", "MOUNIR",
    "ASMAA", "SAMI", "LILY", "OUSSAMA", "ADAM", "FARAH", "FAYﺍAL",
    "SAFIA", "ISMAIL", "NADIA", "ZINOU", "FELLA", "HAMID", "YASMINA",
    "MAHDI", "ASMA", "MOULOUD", "SARA", "TAHA", "NAWEL", "MASSINISSA",
    "SAM", "MALEK", "NASSIMA", "AZIZ", "NACﺓRA", "AYMEN", "LILA",
    "NABIL", "BOUCHRA", "MUSTAPHA", "CHIRAZ", "ANISS", "KENZA",
    "SOFIANE", "HANA", "KHALED", "JULIE", "ZAHIR", "FIDY",
    "ZAKARIA", "SONIA", "FAROUK", "ANISSA", "FARID", "CHAHINEZ",
    "MALIK", "TINA", "AKRAM", "HIND", "SYPHAX", "FATIMA", "HALIM",
    "DOUNIA", "HOUSSEM", "ILHEM", "RIAD", "DAHLIA", "HOUCINE", "SOPHIE",
    "HAMED", "MERYEM", "OUSSAMA", "IMANE", "BAHA EDDINE", "RIM",
    "ILYES", "YASSINE", "SAMIRA", "KOCEILA", "LAMIA", "ELYES", "HADJER",
    "MESCHAC", "SORAYA", "FAOUZI", "MONA", "KHALIL", "MOUNIA", "LAMINE",
    "CYLIA", "BOKA", "SOUAD", "ABBES", "BATOUL", "IMED", "GHADA",
    "AISSA", "MYA", "FREEDY", "DJAMILA", "LOTFI", "HAYAT", "MADOU",
    "KAHINA", "GHILAS", "JOJO", "MOHAMMED", "WAFA", "GHANI", "ZAKIA",
    "DJAMAL", "LIDIA", "LOUNIS", "SABRINE", "AMIR", "RYM", "ACHOUR",
    "NARIMANE", "ABDELHAK", "HAYET", "MOHAND", "FATIHA", "KADI",
    "YOUSRA", "KADER", "LYLIA", "YOUSEF", "LEILA", "MOUMEN"};

  int r = rand() % 190;
  return nom[r];
}


/* ------------------------------------------------------------------ */
/*              retourne un ville choisie aléatoirement à partir      */
/*                    d'un tableau prédéfini de noms wilayas          */
/* ------------------------------------------------------------------ */
char *getRandomAddress(){
  char *wilaya[]={"ADRAR","CHELEF","LAGHOUAT","OUM EL BOUAGUI","BATNA",
    "BEDJAYA","BISKRA","BECHAR","BLIDA","BOUIRA","TAMANRASSET",
    "TEBESSA","TLEMCEN","TIARET", "TIZI OUZOU","ALGER","DJELFA","JIJEL",
    "SETIF","SAIDA","SKIKDA","SIDI BEL ABBES","ANNABA","GUELMA",
    "CONSTANTINE","MEDEA","MOSTAGHANEM","M'SILA", "MASCARA","OUARGLA",
    "ORAN","EL BAYADH","ILLIZI","BORDJ BOU ARRERIDJ","BOUMERDES",
    "EL TAREF","TINDOUF","TISSEMSILT","EL OUED","KHENCHELA",
    "SOUK AHRAS", "TIPAZA","MILA","AIN DEFLA","NAAMA","AIN TEMOUCHENT",
    "GHARDAIA","GHELIZANE","TIMIMOUN","BORDJ BADJI MOKHTAR",
    "OULED DJELLAL","BENI ABBES","IN SALAH","IN GUEZZAM", "TOUGGOURT",
    "DJANET","El M'GHAIR","EL MENIAA"};

  int r = rand() % 58;
  return wilaya[r];
}


/* ------------------------------------------------------------------ */
/*              retourne une taille générée aléatoirement             */
/*                    entre 80 et 200 par pas de 5                    */
/* ------------------------------------------------------------------ */
unsigned short getRandomSize(){
unsigned short r;
  r =((rand() % 13+8)*10+(rand() % 2)*5);
  if (r==205) r=200;

  return r;
}


/* ------------------------------------------------------------------ */
/*              Génère une plage [a, b] entre 1 et 151                */
/*                    avec a<b                                        */
/* ------------------------------------------------------------------ */
void generateReservation(unsigned short *deb, unsigned short *fin){
  unsigned short a = 1 + rand()%151;
  unsigned short b = 1 + rand()%151;
  if (a<b) {
    *deb = a;
    *fin = b;
  } else {
    *deb = b;
    *fin = a;
  }
}


/* ------------------------------------------------------------------ */
/*             Affcihe le message passé en paramètre sur              */
/*             la sortie standard d'erreur (sterr) en rouge (31)      */
/* ------------------------------------------------------------------ */
void quitErreur(const char* msg){
  couleur("31");
  fprintf(stderr,"[-] %s\n", msg);
  couleur("0");
  exit(EXIT_FAILURE);
}


/* ------------------------------------------------------------------ */
/*              Fonction pour lire un chaine de caractères            */
/* ------------------------------------------------------------------ */
void lireChaine(char *nom, const char *msg){
  printf("%s", msg);
  if (gets(nom) == NULL){
    quitErreur("Probleme de saisie de chaine");
  }
}


/* ------------------------------------------------------------------ */
/*              Fonction pour lire une taile de skis                  */
/* ------------------------------------------------------------------ */
unsigned short lireTaille(void){
  unsigned short taille;
  do {
        printf("La taille de la paire de skis : ");
        if (scanf("%hu", &taille) != 1){
            quitErreur("Erreur de saisie");
        }
      } while (taille<80 || taille>205 || taille % 5 !=0);
  fflush(stdin); // vidage du buffer de lecture
  return taille;
}


/* ------------------------------------------------------------------ */
/*                 Machine abstraite sur les listes                   */
/* ------------------------------------------------------------------ */
void liberer(ptrliste_t p){
  assert(p != NULL);
  free(p);
}

void allouer(ptrliste_t *p){
  ptrliste_t q;

  q = (ptrliste_t)malloc(sizeof(liste_t));
  if (q == NULL){
    quitErreur("Reservation impossible");
  }
  *p = q;
}

void affecterValeur(ptrliste_t p, data_t d){
  assert(p != NULL);
  p->data = d;
}

ptrliste_t suivant(ptrliste_t p){
  assert(p != NULL);
  return p->suiv;
}

data_t valeur(ptrliste_t p){
  assert(p != NULL);
  return p->data;
}

bool estVide(ptrliste_t p){
  return (p == NULL);
}

void affecterAdr(ptrliste_t p, ptrliste_t suiv){
  assert(p != NULL);
  p->suiv = suiv;
}

void viderListe(ptrliste_t *L){
  ptrliste_t p, q;

  p=*L;
  while (!estVide(p)){
    q = p;
    p = suivant(p);
    liberer(q);
  }
  *L=NULL;
}
/* ------------------------------------------------------------------ */



/* ------------------------------------------------------------------ */
/*     Accès aux différents champs (taille, loc, deb, fin, ind)       */
/* ------------------------------------------------------------------ */
short int valeurTaille(ptrliste_t p){
  assert(p != NULL);
  return valeur(p).paireski.taille;
}

ptrliste_t valeurLoc(ptrliste_t p){
  assert(p != NULL);
  return (ptrliste_t)valeur(p).paireski.loc;
}

short int valeurDebut(ptrliste_t p){
  assert(p != NULL);
  return valeur(p).reservation.deb;
}

short int valeurFin(ptrliste_t p){
  assert(p != NULL);
  return valeur(p).reservation.fin;
}

short int valeurInd(ptrliste_t p){
  assert(p != NULL);
  return valeur(p).reservation.ind;
}
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/*            Ajoute une paire de skis (ps) à la liste L              */
/* ------------------------------------------------------------------ */
void ajouterPaireSki(ptrliste_t *L, paireski_t ps){
  ptrliste_t p, A = *L;
  data_t d;

  d.paireski = ps;
  allouer(&p);
  affecterValeur(p,d);
  affecterAdr(p, NULL);

  if (estVide(A)){
    affecterAdr(p, NULL);
    A = p;
  } else {
    if (valeurTaille(p) < valeurTaille(A)) {
      affecterAdr(p, A);
      A = p;
    } else {
      ptrliste_t cur, prec = A;
      cur = suivant(A);


      while (!estVide(cur) && valeurTaille(p)>valeurTaille(cur)){
        prec = cur;
        cur = suivant(cur);
      }

      affecterAdr(p, cur);
      affecterAdr(prec, p);
    }
  }
  *L = A;
}


/* ------------------------------------------------------------------ */
/*    Supprime de la liste L toute paire de skis taille (taille)      */
/* ------------------------------------------------------------------ */
void supprimerPaireSki(ptrliste_t *L, short int taille){
  ptrliste_t prec, cur, A=*L;

  cur = A;
  prec = NULL;

  while (!estVide(cur) && valeurTaille(cur) <= taille){
    if (valeurTaille(cur) == taille){
      if (cur == A) {
        A = suivant(cur);
        liberer(cur);
      } else {
        affecterAdr(prec, suivant(cur));
        liberer(cur);
        cur = suivant(prec);
      }
    } else {
      prec = cur;
      cur = suivant(cur);
    }
  }
  *L = A;
}


/* ------------------------------------------------------------------ */
/*    Crée une paire de skis d'une alaéatoire si alea=1               */
/*    ou la taille saisie par l'utilisateur si alea != 1              */
/* ------------------------------------------------------------------ */
paireski_t creerUnePaire(int alea){
  paireski_t ps;
  int t;

  ps.loc = NULL;
  if (alea == 1) {
    ps.taille = getRandomSize();
  } else {
    t = lireTaille();
    ps.taille = t;
  }

  return ps;
}


/* ------------------------------------------------------------------ */
/*    Crée une liste de paires de skis d'une alaéatoire si alea=1     */
/*    ou les tailles saisies par l'utilisateur si alea != 1           */
/* ------------------------------------------------------------------ */
void creerListePairesSki(ptrliste_t *L, int alea){
  int nb;

  viderListe(L);

  do {
    printf("Donnez le nombre de paires de ski a creer : ");
    if(scanf("%d", &nb)!=1){
      quitErreur("Erreur de saisie");
    }
  }
  while (nb<0 || nb>NBPAIRESKIMAX);
  fflush(stdin);

  for(int i=0; i<nb; ++i){
    paireski_t ps;
    ps = creerUnePaire(alea);
    ajouterPaireSki(L, ps);
  }
}


/* ------------------------------------------------------------------ */
/*              Affiche la liste de paires de skis L                  */
/* ------------------------------------------------------------------ */
void afficherPairesSki(ptrliste_t L){
  ptrliste_t p = L;
  printf("plist = [ ");
  while (!estVide(p)){
    printf("%d ", valeurTaille(p));
    p = suivant(p);
  }
  printf("]\n");
}


/* ------------------------------------------------------------------ */
/*        Retourne un pointeur vers la premiere paires de             */
/* taille (taille) partant de L ou NULL (si taille n'existe pas       */
/* ------------------------------------------------------------------ */
ptrliste_t accessTaille(ptrliste_t L, unsigned short taille){
  ptrliste_t p = L;
  while (!estVide(p)){
    if (valeurTaille(p) == taille) return p;
    p = suivant(p);
  }

  return NULL;
}


/* ------------------------------------------------------------------ */
/*        Retourne l'indice d'un skieur dans la table skieur          */
/*               ou -1 si le skieur n'existe pas                      */
/* ------------------------------------------------------------------ */
int cherchskieur(char *nom){
  int i=0;

  while (i<nbskieur){
    if (strcmp(skieur[i].nom, nom) == 0) return i;
    i++;
  }

  return -1;
}


/* ------------------------------------------------------------------ */
/*                  Affiche toutes les réservation d'un client        */
/*               identifié par son indice (ind) pour une paire        */
/*               de ski pointée par pp                                */
/* ------------------------------------------------------------------ */
void afficherReservationClient(ptrliste_t pp, unsigned short ind){
  ptrliste_t p = valeurLoc(pp);
  printf("R(%3d) = [ ",valeurTaille(pp));
  while (!estVide(p)){
    if (valeurInd(p) == ind) {
      printf("(%d, %d) ", valeurDebut(p), valeurFin(p));
    }
    p = suivant(p);
  }
  printf("]\n");
}


/* ------------------------------------------------------------------ */
/*                  Affiche toutes les réservation d'un client        */
/*                          identifié par son nom                     */
/* ------------------------------------------------------------------ */
void afficheToutesReservationsClient(ptrliste_t L, char *nom){
  ptrliste_t p;

  int ind = cherchskieur(nom);
  if (ind == -1) {
    couleur("31");printf("[-] %s n'a pas de reservations !!\n", nom);
    couleur("33");
    return;
  }

  p=L;
  while (!estVide(p)){
    afficherReservationClient(p, ind);
    p = suivant(p);
  }
}


/* ------------------------------------------------------------------ */
/*                  Affiche toutes les réservation                    */
/*               des paires de ski de taille (taille)                 */
/* ------------------------------------------------------------------ */
void afficheReservationParTaille(ptrliste_t L, unsigned short taille){
  ptrliste_t p;

  p=L;
  while (p != NULL){
    if (valeurTaille(p) == taille) afficherReservation(p);
    else if (valeurTaille(p) > taille) return;

    p = suivant(p);
  }
}


/* ------------------------------------------------------------------ */
/*                  Affiche toutes les réservation                    */
/*               d'une paire de ski pointée par pp                    */
/* ------------------------------------------------------------------ */
void afficherReservation(ptrliste_t pp){
  ptrliste_t p = valeurLoc(pp);
  printf("R(%3d) = [ ",valeurTaille(pp));
  while (!estVide(p)){
    printf("(%d, %d, %d) ", valeurDebut(p), valeurFin(p), valeurInd(p));
    p = suivant(p);
  }
  printf("]\n");
}


/* ------------------------------------------------------------------ */
/*                  Affiche toutes les réservation                    */
/*                   de toutes les paires de skis                     */
/* ------------------------------------------------------------------ */
void afficheToutesReservations(ptrliste_t L){
  ptrliste_t p;

  p=L;
  while (!estVide(p)){
    afficherReservation(p);
    p = suivant(p);
  }
}


/* ------------------------------------------------------------------ */
/* Prend en entrée un intervalle [d, f] et une paire de skis pointée  */
/* par pp et renvoie "True" si [d, f] ne chevauche pas avec une autre */
/*    réservation dans pp. Sinon elle retourne "False"                */
/* ------------------------------------------------------------------ */
bool estIntersectionVide(ptrliste_t pp, unsigned short d,
    unsigned short f){
  ptrliste_t p = pp;

  while (!estVide(p)){
    if (!(valeurDebut(p)> f || valeurFin(p)<d)) return false;
    p = suivant(p);
  }
  return true;
}


/* ------------------------------------------------------------------ */
/*            Insere une réservation en tête de la liste L            */
/* ------------------------------------------------------------------ */
void inserTete(ptrliste_t *L, data_t d){
  ptrliste_t p;

  ptrliste_t pp = valeurLoc(*L);


  allouer(&p);
  affecterValeur(p, d);
  affecterAdr(p, NULL);

  if (estVide(pp)){
    pp = p;
  } else {
    affecterAdr(p, pp);
    data_t dm;
    paireski_t ps;
    ps.taille = valeurTaille(*L);
    ps.loc = (ptrreservation_t)p;
    dm.paireski = ps;

    affecterValeur(*L, dm);
  }
}


/* ------------------------------------------------------------------ */
/* Fonction qui prend en paramètres le nom, l'adresse, la taille d'une*/
/* paire de ski, l'intervalle [debut, fin] de la réservation          */
/* effectue la réservation et affecte True à la variable (possible)   */
/* quand la réseravtion est possible. Sinon elle affecte              */
/* la valeur False à la variable de sortie (possibe)                  */
/* ------------------------------------------------------------------ */
void reservation(ptrliste_t L, char *nom, char *adresse,
      unsigned short taille, unsigned short debut,
      unsigned short fin, bool *possible){

    ptrliste_t p;
    ptrliste_t premloc;


    *possible = false;
    p = accessTaille(L, taille);

    while (!estVide(p) && *possible==false) {

        premloc = valeurLoc(p);
        if (estIntersectionVide(premloc, debut, fin)) {


            int ind = cherchskieur(nom);

            if (ind == -1) {
              strncpy(skieur[nbskieur].nom, nom, NAME_SIZE);
              strncpy(skieur[nbskieur].adresse, adresse, ADDR_SIZE);
              skieur[nbskieur].psk = 1;
              ind = nbskieur;
              nbskieur++;
            } else {
              skieur[ind].psk++;
            }


            ptrliste_t pp ;
            allouer(&pp);
            reservation_t re;
              re.deb = debut;
              re.fin = fin;
              re.ind = ind;
            data_t d;

            d.reservation = re;
            affecterValeur(pp, d);
            affecterAdr(pp, premloc);

            d = valeur(p);
            d.paireski.loc = (ptrreservation_t)pp;
            affecterValeur(p,d);

            afficherReservation(p);
            *possible=true;
            return;

        } else {
          p = accessTaille(suivant(p), taille);
        }
    }

    *possible = false;
}


/* ------------------------------------------------------------------ */
/*   Effectue (nr) réservation(s) (res[]) pour un client (nom)        */
/*                  de l'adresse (adresse)                            */
/* ------------------------------------------------------------------ */
void reservations(ptrliste_t plist, char *nom, char *adresse,
                   element_t res[], int nr){
  bool possible;

  printf("M./Mme %s residant(e) a %s souhaite faire %d reservation(s)\n\n", nom, adresse, nr);
  for(int i=0; i<nr; ++i){
    printf("Reservation Num. %d : ski de taille %hu du %hu au %hu \n", i+1, res[i].taille, res[i].deb, res[i].fin);
    reservation(plist, nom, adresse, res[i].taille, res[i].deb, res[i].fin, &possible);
    if (possible) {
      couleur("32");printf("[+] reservation effecrtuee\n\n");couleur("33");
    } else {
      couleur("31");printf("[-] reservation impossible\n\n");couleur("33");
    }
  }
}


/* ------------------------------------------------------------------ */
/*            supprimer les reservation d'un pairede ski pointée      */
/*                            par (pp)                                */
/* ------------------------------------------------------------------ */
void supprimerReservation(ptrliste_t pp){
  data_t d = valeur(pp);
  ptrliste_t A = valeurLoc(pp);
  ptrliste_t cur, prec;

  if (!estVide(A)){
    cur = A;
    prec=NULL;
    while (cur != NULL){
        if (prec == NULL) {
          prec = cur;
          cur = suivant(cur);
          liberer(prec);
          prec = NULL;
        } else {
          affecterAdr(prec, suivant(cur));
          liberer(cur);
          cur = suivant(prec);
        }
    }
    d.paireski.loc = NULL;

    affecterValeur(pp, d);

  }
}


/* ------------------------------------------------------------------ */
/*   supprimer toutes les reservations de toutes les paires de skis   */
/* ------------------------------------------------------------------ */
void supprimerToutesReservation(ptrliste_t L){
  ptrliste_t p = L;
  while (!estVide(p)){
    supprimerReservation(p);
    p = suivant(p);
  }
  nbskieur=0;
}


/* ------------------------------------------------------------------ */
/*             supprimer toutes les reservations de le client         */
/*             définé par l'indice (ind) dans le paire se ski L       */
/* ------------------------------------------------------------------ */

void supprimerParIndice(ptrliste_t *L, unsigned int ind){
  ptrliste_t A = *L;
  ptrliste_t prec, cur;

  if (!estVide(A)){
    cur = A;
    prec=NULL;
    while (!estVide(cur)){
      if (valeurInd(cur) == ind){
        if (prec == NULL) {
          prec = cur;
          cur = suivant(cur);
          A = cur;
          liberer(prec);
          prec = NULL;
        } else {
          affecterAdr(prec, suivant(cur));
          liberer(cur);
          cur = suivant(prec);
        }
      } else {
        prec = cur;
        cur = suivant(cur);
      }
    }
  *L = A;
  }
}


/* ------------------------------------------------------------------ */
/*             supprimer toutes les reservations de le client         */
/*         définé par son nom (nom) dans toutes les paires de skis    */
/* ------------------------------------------------------------------ */
void annuleres(ptrliste_t plist, char *nom)
{
  ptrliste_t p;

  int ind = cherchskieur(nom);
  if (ind == -1) return;

  p = plist;
  while (!estVide(p)) {
    data_t d = valeur(p);
    ptrliste_t ploc = valeurLoc(p);

    supprimerParIndice(&ploc, ind);

    d.paireski.loc = (ptrreservation_t)ploc;
    affecterValeur(p, d);

    p = suivant(p);
  }

  for (int i=ind; i<nbskieur-1; ++i){
      skieur[i] = skieur[i+1];
  }
  nbskieur--;
}


/* ------------------------------------------------------------------ */
/* Si alea =1 : ajouter une reservation de maniére aléatoire          */
/* Si alea <>1 : ajouter une reservation de maniére saisie            */
/* ------------------------------------------------------------------ */
void ajouterUneReservation(ptrliste_t L, int alea){
  int nr, taille, deb, fin;
  element_t res[NRESMAX];
  char nom[NAME_SIZE];
  char adresse[ADDR_SIZE];

  nr=1;

  fflush(stdin);

  if (alea==1){
    strncpy(nom,  getRandomName(), NAME_SIZE);
    strncpy(adresse, getRandomAddress(), ADDR_SIZE);
    generateReservation(&res[0].deb, &res[0].fin);
    res[0].taille = getRandomSize();
  } else {
    lireChaine(nom, "Donnez le nom : ");
    lireChaine(adresse, "Donnez l'adresse : ");
    taille = lireTaille();
    do {
        printf("Le debut de location : ");
        if (scanf("%d", &deb)!= 1){
          quitErreur("Erreur de saisie");
        }
    } while (deb<1 || deb>151);

    do {
        printf("La fin de location : ");
        if(scanf("%d", &fin)!= 1){
          quitErreur("Erreur de saisie");
        }
    } while (fin<1 || fin>151 || fin<deb);

      res[0].taille = taille;
      res[0].deb = deb;
      res[0].fin = fin;

  }
  reservations(L, nom, adresse, res, nr);
}


/* ------------------------------------------------------------------ */
/*              Le module : creerListeReservation                     */
/* si alea=1 alors les reservations sont crees dune manière aléatoire */
/* si alea<>1 alors les reservations sont crées par saisie            */
/* ------------------------------------------------------------------ */
void creerListeReservation(ptrliste_t L, int alea){
  int nr, taille, deb, fin;
  element_t res[NRESMAX];
  char nom[NAME_SIZE];
  char adresse[ADDR_SIZE];

  do {
      printf("Donner le nombre de reservations : ");
      if(scanf("%d", &nr) != 1){
          quitErreur("Erreur de saisie");
      }
  } while (nr<0 || nr>NRESMAX);

  fflush(stdin);

  if (alea == 1){
    strncpy(nom,  getRandomName(), NAME_SIZE);
    strncpy(adresse, getRandomAddress(), ADDR_SIZE);
    for(int i=0; i<nr; ++i){
      generateReservation(&res[i].deb, &res[i].fin);
      res[i].taille = getRandomSize();
    }
  } else {
    printf("Donnez le nom : "); gets(nom);
    printf("Donnez l'adresse : "); gets(adresse);

    for(int i=0; i<nr; ++i){
      taille = lireTaille();

      do {
        printf("Le debut de location : "); //lecture du jour debut//
        if(scanf("%d", &deb)!= 1){
          quitErreur("Erreur de saisie");
        }
      } while (deb<1 || deb>151);

      do {
        printf("La fin de location : ");//lecture du jour fin//
        if(scanf("%d", &fin)!= 1){
          quitErreur("Erreur de saisie");
        }
      } while (fin<1 || fin>151 || fin<deb);

      res[i].taille = taille;
      res[i].deb = deb;
      res[i].fin = fin;
    }
  }
  reservations(L, nom, adresse, res, nr); //effectuer la reservation//
}


/* ------------------------------------------------------------------ */
/*             La fonction compteJourLocation                         */
/* Entree :  un pointeur vers une liste de reservation d'une paire    */
/*                       de Ski                                       */
/* La valeur de retour : le nombre de jours de location               */
/* d'une paire de ski                                                 */
/* ------------------------------------------------------------------ */
int compteJourLocation(ptrliste_t pp){
  ptrliste_t p=pp;
  int nb=0;
  while (!estVide(p)){
    nb = nb + (valeurFin(p) - valeurDebut(p) + 1);
    p = suivant(p);
  }
  return nb;
}


/* ------------------------------------------------------------------ */
/*                      La fonction nbjloc                            */
/* Entrees : La liste plist , une taille d'une paire de Ski           */
/* La valeur de retour: le nombre de jours de location                */
/* d'une paire de ski                                                 */
/* ------------------------------------------------------------------ */
int nbjloc(ptrliste_t L, short int taille){
  ptrliste_t p;
  int x=0;
  p=L;
  while (!estVide(p)){
    if (valeurTaille(p) == taille) x = x + compteJourLocation(valeurLoc(p));
    else if (valeurTaille(p) > taille) return x;
    p = suivant(p);
  }
  return x;
}


/* ------------------------------------------------------------------ */
/*                      Le module  AfficherTableSkieur                */
/*             afficher toutes les clients qui ont au moins           */
/*     une reservation dansun table de taille (nbskieur) qui contient */
/*     3 champs : le nom  |  l'adresse   | le nombre de reservations  */
/* ------------------------------------------------------------------ */
void AfficherTableSkieur(){
  printf("Table des Skieurs :\n\n");
  printf("%30s%30s%20s\n","Nom","Adresse", "PSK");
  for(int i=0; i<nbskieur; ++i){
    printf("%30s%30s%20d\n",skieur[i].nom,skieur[i].adresse, skieur[i].psk);
  }
}






/* ------------------------------------------------------------------ */
/* Role : Affiche le menu                                             */
/* Entree/Sortie : Rien                                               */
/* Valeur de retour : entier correspondant au choix de l'utilisateur  */
/* ------------------------------------------------------------------ */
int afficheMenu(void){
  int c;
  clrscr();
  couleur("32");
  printf("\t\t*********************************************************\n");
  printf("\t\t+             Gestion de location de skis                +\n");
  printf("\t\t+                                                       +\n");
  printf("\t\t+                     Realise par                       +\n");
  printf("\t\t+                O. Bedraoui & I. Ziadi                 +\n");
  printf("\t\t+                                                       +\n");
  printf("\t\t*********************************************************\n");
  printf("\t\t                                                         \n");
  printf("\t\t %2d - Creer la liste des paires de skis aleatoirement\n",CREER_SKIS);
  printf("\t\t %2d - Creer la liste des paires de skis saisies\n",CREER_SKIS_SAISI);
  printf("\t\t %2d - Ajouter une paire de ski\n",AJOUTER_SKI);
  printf("\t\t %2d - Supprimer une paire de ski\n",SUPPRIMER_SKI);
  printf("\t\t %2d - Supprimer la liste des paires de skis \n",SUPPRIMER_TOUS_SKIS);
  printf("\t\t %2d - Afficher la liste des paires de skis\n",AFFICHER_SKIS);
  printf("\t\t %2d - Nombre de jours de location d'une paire de skis\n",AFFICHER_NBJ_LOC);
  printf("\t\t %2d - Creer aleatoirement de reservations  \n",CREER_RES_ALEA);
  printf("\t\t %2d - Saisir un ensemble de reservations\n",CREER_RES_SAISI);
  printf("\t\t %2d - Ajouter une reservation\n",AJOUTER_RES);
  printf("\t\t %2d - Afficher les reservations d'un client\n",AFFICHER_RES_NOM);
  printf("\t\t %2d - Afficher les reservations d'une paire de ski\n",AFFICHER_RES_TAILLE);
  printf("\t\t %2d - Afficher toutes les reservations\n",AFFICHER_TOUTES_RES);
  printf("\t\t %2d - Annuler toutes les reservations d'un client\n",ANNULER_TOUTES_RES_CLIENT);
  printf("\t\t %2d - Annuler toutes les reservations\n",ANNULER_TOUTES_RES);
  printf("\t\t %2d - Afficher Table Skieurs\n",AFFICHER_TAB_SKIEUR);
  printf("\t\t %2d - Quitter le programme \n",QUITTER);

  printf("\n\n\t\t\tDonner votre choix : ");
  couleur("0");
  if (scanf("%d", &c) != 1){
    c = -1;
  }
  fflush(stdin);
  return c;
}



