/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Simple animation périodique
 * -------------------------------------------------------------------------
 */

#include <Arduino.h>

/**
 * @brief Nombre de LEDs.
 */
const uint8_t NUM_LEDS = 8;

/**
 * @brief Broches de commande des LEDs.
 */
const uint8_t LED_PIN[] = { 5, 6, 7, 8, 9, 10, 11, 12 };

/**
 * @brief Nombre d'étapes que comporte l'animation.
 * 
 * @note Une animation est ici considérée comme un enchaînement
 *       de motifs prédéfinis à afficher dans un ordre donné.
 *       Chaque motif correspond ainsi à une étape de l'animation.
 */
const uint8_t ANIMATION_STEPS = 10;

/**
 * @brief Séquence ordonnée de motifs prédéfinis qui constitue
 *        les différentes étapes de l'animation.
 * 
 * @note L'idée consiste ici à représenter chaque étape de l'animation
 *       par un motif binaire décrivant l'état du chenillard à l'aide
 *       d'un simple nombre entier codé sur 8 bits (pour 8 LEDs).
 * 
 *       En effet, pour chaque étape, lorsque le bit d'indice `i` est à 1
 *       alors la LED d'indice `i` est allumée. Si le bit est à 0, alors
 *       la LED est éteinte.
 * 
 *       L'animation qui est décomposée ci-dessous est un petit clin d'oeil
 *       à la célèbre voiture intelligente nommée KITT, mise en scène dans
 *       la série télévisée K2000 des années 80  😉
 * 
 *       @see https://youtu.be/iN3rvvkHo1M
 * 
 *       Pour une meilleure lisibilité, les entiers sont ici définis sous
 *       leur forme d'écriture binaire et sont donc préfixés par `0b`.
 * 
 *       On pourrait également définir ces mêmes entiers sous leurs formes
 *       d'écritures hexadécimale (préfixe `0x`) ou décimale (sans préfixe).
 *       Cela revient strictement au même pour le compilateur.
 */
const uint8_t ANIMATION[] = {
    0b11100000, // étape #0    0xe0    224
    0b01110000, // étape #1    0x70    112
    0b00111000, // étape #2    0x38     56
    0b00011100, // étape #3    0x1c     28
    0b00001110, // étape #4    0x0e     14
    0b00000111, // étape #5    0x07      7
    0b00001110, // étape #6    0x0e     14
    0b00011100, // étape #7    0x1c     28
    0b00111000, // étape #8    0x38     56
    0b01110000  // étape #9    0x70    112
};

/**
 * @brief Durée pendant laquelle on affiche le motif courant de l'animation.
 */
const uint8_t ANIMATION_STEP_DURATION_MS = 100;

/**
 * @brief Indice du motif courant de l'animation.
 */
uint8_t animation_step = 0;

/**
 * @brief Date à laquelle on a affiché le dernier motif courant de l'animation.
 */
uint32_t last_animation_step_ms = 0;

/**
 * @brief Affichage d'un motif binaire 8-bits sur le chenillard à 8 LEDs.
 * 
 * @param n entier compris dans l'intervalle [0,255].
 */
void ledWrite(uint8_t n) {

    for (uint8_t i=0; i<NUM_LEDS; i++) {
        digitalWrite(LED_PIN[i], n & (1 << i));
    }

}

/**
 * @brief Démarrage du programme.
 */
void setup() {

    // Configuration des broches de commande des LEDs.
    for (uint8_t i=0; i<NUM_LEDS; i++) {
        pinMode(LED_PIN[i], OUTPUT);
    }

    // On initialise la date de démarrage de l'afficheur.
    last_animation_step_ms = millis();
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Comme dans l'exercice précédent, on n'utilise
 *       plus la fonction delay() qui est bloquante.
 */
void loop() {

    // On détermine la date courante :
    uint32_t now = millis();

    // Si la durée écoulée depuis le dernier affichage de motif
    // a dépassé la période d'attente que l'on s'est fixée...
    if (now - last_animation_step_ms > ANIMATION_STEP_DURATION_MS) {

        // Alors on affiche le motif binaire qui correspond à
        // l'étape courante de l'animation :
        ledWrite(ANIMATION[animation_step]);

        // Puis on passe à l'étape suivante :
        ++animation_step %= ANIMATION_STEPS;
        /*
         * L'instruction ci-dessus est une forme condensée de
         * la séquence suivante :
         * 
         *     animation_step = animation_step + 1;
         *     animation_step = animation_step % ANIMATION_STEPS;
         * 
         * Le compteur `animation_step` s'incrémente progressivement
         * pour balayer, dans l'ordre, chaque étape de l'animation,
         * puis revient à zéro après avoir atteint la dernière étape,
         * grâce à l'utilisation de l'opérateur de congruence % (modulo).
         * 
         * Enfin, on sauvegarde la date qui marque temporellement
         * l'affichage du dernier motif opéré sur le chenillard :
         */
        last_animation_step_ms = now;

    }

}