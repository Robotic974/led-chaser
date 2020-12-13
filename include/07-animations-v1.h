/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Séquenceur d'animations périodiques (première version)
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
 *       "frame" pour reprendre un anglicisme consacré.
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
 * @brief Définition de l'indice du motif de départ de chaque animation.
 * 
 * @note Cet indice permet de repérer le motif de départ de chaque
 *       animation dans le tableau `ANIMATION_FRAME`.
 */
const uint8_t ANIMATION_START[] = {
     0, // animation #0
    14, // animation #1
    20, // animation #2
    30, // animation #3
    38, // animation #4
    40, // animation #5
    44, // animation #6
    52  // animation #7
};

/**
 * @brief Définition du nombre de motifs que comporte chaque animation.
 */
const uint8_t ANIMATION_FRAMES[] = {
    14, // animation #0
     6, // animation #1
    10, // animation #2
     8, // animation #3
     2, // animation #4
     4, // animation #5
     8, // animation #6
    37  // animation #7
};

/**
 * @brief Définition de la durée d'affichage de chaque motif pour une animation.
 * 
 * @note Ces durées sont exprimées en millisecondes.
 */
const uint8_t ANIMATION_DELAY_MS[] = {
     40, // animation #0
     50, // animation #1
     50, // animation #2
     50, // animation #3
    120, // animation #4
     80, // animation #5
     60, // animation #6
     40  // animation #7
};

/**
 * @brief Définition du nombre de répétitions à appliquer à chaque animation.
 */
const uint8_t ANIMATION_REPEAT[] = {
     4, // animation #0
     8, // animation #1
     5, // animation #2
     6, // animation #3
    10, // animation #4
     8, // animation #5
     7, // animation #6
     1  // animation #7
};

/**
 * @brief Définition de l'indice de l'animation en cours.
 * 
 * @note 0 ≤ animation_id < NUM_ANIMATIONS
 */
uint8_t animation_id;

/**
 * @brief Définition de l'indice du motif de départ de l'animation en cours.
 */
uint8_t animation_start;

/**
 * @brief Définition du nombre de motifs que comporte l'animation en cours.
 */
uint8_t animation_frames;

/**
 * @brief Définition de la durée d'affichage de chaque motif pour l'animation en cours.
 */
uint8_t animation_delay_ms;

/**
 * @brief Définition du nombre de répétitions à appliquer à l'animation en cours.
 */
uint8_t animation_repeat;

/**
 * @brief Indice du motif à afficher sur la rampe de LED relatif à l'animation en cours.
 */
uint8_t player_frame;

/**
 * @brief Nombre de répétitions appliquées à l'animation en cours.
 */
uint8_t player_repeat;

/**
 * @brief Date du dernier affichage opéré sur la rampe de LEDs.
 */
uint32_t player_last_ms;

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
 * @note On effectue l'initialisation des paramètres de prise en charge de
 *       la nouvelle animation qui va démarrer.
 */
void startAnimation(const uint8_t index) {

    animation_id       = index;
    animation_start    = ANIMATION_START[index];
    animation_frames   = ANIMATION_FRAMES[index];
    animation_delay_ms = ANIMATION_DELAY_MS[index];
    animation_repeat   = ANIMATION_REPEAT[index];

    player_frame  = 0;
    player_repeat = 0;

}

/**
 * @brief Lecture incrémentale de l'animation courante.
 */
void playAnimation() {

    // Lecture du motif binaire en cours à afficher sur la rampe de LEDs :
    const uint8_t frame = ANIMATION_FRAME[animation_start + player_frame];

    // Affichage du motif en cours sur la rampe de LEDs :
    ledWrite(frame);

    // Détermination du prochain motif à afficher.
    // Si l'animation courante n'est pas terminée...
    if (player_frame + 1 < animation_frames) {

        // Alors on passe simplement au motif suivant :
        player_frame++;

    // Sinon, c'est qu'on est arrivé au terme de l'animation courante.
    } else {

        // Auquel cas, on vérifie si on doit la répéter à nouveau...
        if (player_repeat + 1 < animation_repeat) {

            // Il faut alors se replacer sur le premier motif de l'animation :
            player_frame = 0;
            // Et prendre en compte la nouvelle répétition :
            player_repeat++;

        // Si l'animation ne devait pas être répétée une nouvelle fois,
        // c'est que nous devons passer à la prochaine animation...
        } else {

            // L'indice de la nouvelle animation est donc incrémenté,
            // et on vérifie également s'il ne faut pas redémarrer
            // tout le processus à la première animation définie
            // dans le tableau `ANIMATION_FRAME`, une fois qu'on a
            // fait le tour de toutes les animations définies.
            ++animation_id %= NUM_ANIMATIONS;

            // Il ne reste plus qu'à lancer la nouvelle animation :
            startAnimation(animation_id);

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
    player_last_ms = millis();
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Toujours sans utiliser la fonction delay() !
 */
void loop() {

    const uint32_t now = millis();

    if (now - player_last_ms > animation_delay_ms) {

        playAnimation();

        player_last_ms = now;

    }

}