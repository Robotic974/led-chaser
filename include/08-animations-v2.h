/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Séquenceur d'animations périodiques (seconde version)
 * -------------------------------------------------------------------------
 */

#include <Arduino.h>

// ----------------------------------------------------------------------------
// Définition des constantes et variables globales
// ----------------------------------------------------------------------------

/**
 * @brief Nombre de LEDs.
 */
const uint8_t NUM_LEDS = 8;

/**
 * @brief Broches de commande des LEDs.
 */
const uint8_t LED_PIN[] = { 5, 6, 7, 8, 9, 10, 11, 12 };

/**
 * @brief Nombre d'animations prédéfinies dans l'enchaînement proposé.
 */
const uint8_t NUM_ANIMATIONS = 8;

/**
 * @brief Définition des motifs constituant chaque animation.
 * 
 * @note Chaque animation est définie par une séquence ordonnée de motifs
 *       binaires (décrits par des entiers codés sur 8 bits), ainsi que par
 *       un nombre fini de motifs, qui correspond en définitive à la longueur
 *       de la séquence qui décrit l'animation.
 *       
 *       Chaque motif peut être considéré comme une image instantanée de
 *       l'animation qu'elle participe à décrire. On parlera également de
 *       "frame" pour reprendre un anglicisme usuel.
 *       
 *       On fait ici le choix de définir au sein d'un même tableau l'ensemble
 *       des animations que nous allons enchaîner les unes après les autres.
 */
const uint8_t ANIMATION_FRAME[] = {
    
    // animation #0

    0b10000000, //
    0b01000000, //
    0b00100000, //
    0b00010000, //
    0b00001000, //
    0b00000100, //
    0b00000010, // 14 frames
    0b00000001, //
    0b00000010, //
    0b00000100, //
    0b00001000, //
    0b00010000, //
    0b00100000, //
    0b01000000, //

    // animation #1

    0b10000001, //
    0b01000010, //
    0b00100100, // 6 frames
    0b00011000, //
    0b00100100, //
    0b01000010, //

    // animation #2

    0b11100000, //
    0b01110000, //
    0b00111000, //
    0b00011100, //
    0b00001110, // 10 frames
    0b00000111, //
    0b00001110, //
    0b00011100, //
    0b00111000, //
    0b01110000, //

    // animation #3

    0b00000000, //
    0b00011000, //
    0b00111100, //
    0b01111110, // 8 frames
    0b11111111, //
    0b01111110, //
    0b00111100, //
    0b00011000, //

    // animation #4

    0b01010101,// 2 frames
    0b10101010,// 

    // animation #5

    0b00010001, //
    0b00100010, // 4 frames
    0b01000100, //
    0b10001000, //

    // animation #6

    0b00000001, //
    0b00000010, //
    0b00000100, //
    0b00001000, // 8 frames
    0b00010000, //
    0b00100000, //
    0b01000000, //
    0b10000000, //

    // animation #7

    0b00000000, //
    0b00010000, //
    0b00001000, //
    0b00010000, //
    0b00100000, //
    0b00010000, //
    0b00001000, //
    0b00000100, //
    0b00001000, //
    0b00010000, //
    0b00100000, //
    0b01000000, //
    0b00100000, //
    0b00010000, //
    0b00001000, //
    0b00000100, //
    0b00000010, //
    0b00000100, // 37 frames
    0b00001000, //
    0b00010000, //
    0b00100000, //
    0b01000000, //
    0b10000000, //
    0b01000000, //
    0b00100000, //
    0b00010000, //
    0b00001000, //
    0b00000100, //
    0b00000010, //
    0b00000001, //
    0b00000010, //
    0b00000100, //
    0b00001000, //
    0b00010000, //
    0b00100000, //
    0b01000000, //
    0b10000000  //

};

/**
 * @brief Définition de la structure de données d'une animation.
 * 
 * @note Pour caractériser précisément chaque animation comme une séquence
 *       périodique de frames (définies par ailleurs dans le tableau précédent),
 *       on crée une structure de données générique pour les décrire toutes :
 */
struct Animation {
    uint8_t start;          // Indice du motif de départ dans le tableau.
    uint8_t frames;         // Nombre de motifs constituant la séquence.
    uint8_t frame_delay_ms; // Durée d'affichage de chaque motif exprimée en millisecondes.
    uint8_t repeat;         // Nombre de répétitions de la séquence.
};

/**
 * @brief Définition des animations périodiques que l'on souhaite enchaîner.
 * 
 * @note Maintenant que nous avons défini la structure générique commune à toutes
 *       les animations, il ne nous reste plus qu'à définir concrètement chacune
 *       d'entre elles :
 */
const Animation animation[] = {
//
//     +---------------- start
//     |   +------------ frames
//     |   |    +------- frame_delay_ms
//     |   |    |   +--- repeat
//     |   |    |   |
//     v   v    v   v
    {  0, 14,  40,  4 }, // animation #0
    { 14,  6,  50,  8 }, // animation #1
    { 20, 10,  50,  5 }, // animation #2
    { 30,  8,  50,  6 }, // animation #3
    { 38,  2, 120, 10 }, // animation #4
    { 40,  4,  80,  8 }, // animation #5
    { 44,  8,  60,  7 }, // animation #6
    { 52, 37,  40,  1 }  // animation #7
};

/**
 * @brief Définition du séquenceur d'animation.
 * 
 * @note Pour faciliter la lecture des animations, nous définissons un séquenceur
 *       qui va nous permettre de gérer précisément comment doit se dérouler la
 *       lecture périodique des animations et la gestion des paramètres afférents.
 */
struct Player {
    uint8_t  animation_id; // Indice de l'animation en cours.
    uint8_t  repeat;       // Nombre de répétitions effectuées.
    uint8_t  frame;        // Indice du motif binaire relatif à l'animation en cours.
    uint32_t last_ms;      // Date du dernier affichage opéré sur la rampe de LEDs.
};

/**
 * @brief Initalisation du séquenceur.
 * 
 * @note Tous les paramètres sont initialisés à zéro par défaut.
 */
Player player = {
    0, // animation_id
    0, // repeat
    0, // frame
    0  // last_ms
};

// ----------------------------------------------------------------------------
// Gestion des LEDs
// ----------------------------------------------------------------------------

/**
 * @brief Initialisation des broches de commande des LEDs.
 */
void initLeds() {

    for (uint8_t i=0; i<NUM_LEDS; i++) {
        pinMode(LED_PIN[i], OUTPUT);
    }

}

/**
 * @brief Affichage d'un motif binaire 8-bits sur le chenillard à 8 LEDs.
 * 
 * @param n Entier compris dans l'intervalle [0,255].
 */
void ledWrite(const uint8_t pattern) {

    for (uint8_t i=0; i<NUM_LEDS; i++) {
        digitalWrite(LED_PIN[i], pattern & (1 << i));
    }

}

// ----------------------------------------------------------------------------
// Gestion des animations
// ----------------------------------------------------------------------------

/**
 * @brief Lancement d'une animation.
 * 
 * @param index Indice de l'animation à lancer (0 ≤ index < NUM_ANIMATIONS)
 * 
 * @note On effectue l'initialisation des propriétés du séquenceur avec les données
 *       de prise en charge de la nouvelle animation qui va démarrer.
 */
void startAnimation(const uint8_t index) {

    player.animation_id = index;
    player.repeat       = 0;
    player.frame        = 0;

}

/**
 * @brief Lecture incrémentale de l'animation courante.
 */
void playAnimation() {

    // Définition d'un pointeur sur la structure de donnée qui décrit
    // l'animation courante, permettant d'accéder directement à ses
    // propriétés sans faire de copie locale.
    // 
    // Le mot-clef `const` utilisé 2 fois ici déclare que :
    //   1. la structure de données pointée est constante,
    //   2. le pointeur lui-même est de valeur constante,
    const Animation * const pAnimation = &animation[player.animation_id];

    // Lecture du motif binaire en cours à afficher sur la rampe de LEDs :
    const uint8_t frame = ANIMATION_FRAME[pAnimation->start + player.frame];

    // Affichage du motif en cours sur la rampe de LEDs :
    ledWrite(frame);

    // Déplacement de la tête de lecture du séquenceur.
    // Si l'animation courante n'est pas terminée...
    if (player.frame + 1 < pAnimation->frames) {

        // Alors on déplace la tête de lecture au prochain motif binaire
        // de l'animation courante :
        player.frame++;

    // Sinon, c'est qu'on est arrivé au terme de l'animation courante.
    } else {

        // Auquel cas, on vérifie si on doit la répéter à nouveau...
        if (player.repeat + 1 < pAnimation->repeat) {

            // Il faut alors replacer la tête de lecture au début de l'animation :
            player.frame = 0;
            // Et incrémenter le nombre de répétitions opérées sur l'animation :
            player.repeat++;

        // Si l'animation ne devait pas être répétée une nouvelle fois,
        // c'est que nous devons passer à la prochaine animation...
        } else {

            // L'indice de la nouvelle animation est donc incrémenté,
            // et on vérifie également s'il ne faut pas redémarrer
            // tout le processus à la première animation définie
            // dans le tableau `ANIMATION_FRAME`, une fois qu'on a
            // fait le tour de toutes les animations définies.
            ++player.animation_id %= NUM_ANIMATIONS;

            // Il ne reste plus qu'à lancer la nouvelle animation :
            startAnimation(player.animation_id);

        }

    }

}

// ----------------------------------------------------------------------------
// Squelette principal du programme
// ----------------------------------------------------------------------------

/**
 * @brief Démarrage du programme.
 */
void setup() {

    initLeds();
    startAnimation(0);
    player.last_ms = millis();

}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Toujours sans utiliser la fonction delay() !
 */
void loop() {

    const uint32_t now = millis();

    // Définition d'un pointeur sur la structure de donnée qui décrit
    // l'animation courante, permettant d'accéder directement à ses
    // propriétés sans faire de copie locale.
    // 
    // Le mot-clef `const` utilisé 2 fois ici déclare que :
    //   1. la structure de données pointée est constante,
    //   2. le pointeur lui-même est de valeur constante,
    const Animation * const pAnimation = &animation[player.animation_id];

    if (now - player.last_ms > pAnimation->frame_delay_ms) {

        playAnimation();

        player.last_ms = now;

    }

}