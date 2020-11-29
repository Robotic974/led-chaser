/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Balayage unidirectionnel
 * -------------------------------------------------------------------------
 */

#include <Arduino.h>

/**
 * @brief Définition du nombre de LEDs.
 */
const uint8_t NUM_LEDS = 8;

/**
 * @brief Définition des broches de commande des LEDs.
 * 
 * @note On souhaite ici disposer d'une liste ordonnée des broches de commande
 *       permettant d'alimenter chacune des LEDs du chenillard en respectant
 *       l'ordre avec lequel les LEDs sont disposées sur la breadboard.
 * 
 *       Or, pour des raisons diverses, il peut arriver que vous ne puissiez pas
 *       brancher vos LEDs sur la carte Arduino en respectant cet ordre, ni même
 *       en utilisant des broches qui soient consécutives.
 * 
 *       Cependant, pour optimiser et faciliter l'implémentation du chenillard,
 *       il est beaucoup plus commode de pouvoir commander des LEDs dont les
 *       broches de commande se suivent de proche en proche.
 * 
 *       Pour satisfaire cette commodité, et ce, quel que soit la manière dont
 *       les LEDs sont branchées, il suffit de les ranger dans l'ordre souhaité
 *       à l'intérieur d'un tableau. De cette manière, chaque broche de commande
 *       sera indexée par une suite d'entiers consécutifs de la façon suivante :
 * 
 *       +---------------------------------------------------------------+
 *       |             indices de rangement dans le tableau              |
 *       +---------------------------------------------------------------+
 *       |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |
 *       +---------------------------------------------------------------+
 *           |       |       |       |       |       |       |       |
 *           v       v       v       v       v       v       v       v
 *       +---------------------------------------------------------------+
 *       | LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8 |
 *       +---------------------------------------------------------------+
 *       |             ordre de placement sur la breadboard              |
 *       +---------------------------------------------------------------+
 * 
 *       où chaque LED_i (i étant le rang de la i-ème LED dans le chenillard) peut
 *       être branchée sur n'importe quelle broche numérique de la carte Arduino.
 * 
 *       Il se trouve que, dans le cas du montage que je vous ai proposé, les 8 LEDs
 *       sont branchées dans l'ordre, sur des broches consécutives de la carte
 *       Arduino (les broches D5 à D12). Mais on aurait pu procéder autrement.
 *       D'ailleurs, vous pouvez vous amuser à intervertir les branchements pour
 *       vérifier que la modélisation retenue ici reste satisfaisante. Par contre,
 *       veillez à reporter les numéros des broches de chaque LED, ci-dessous,
 *       dans l'ordre où elles se succèdent sur la breadboard.
 */
const uint8_t LED_PIN[] = { 5, 6, 7, 8, 9, 10, 11, 12 };

/**
 * @brief Démarrage du programme.
 * 
 * @note On balaie l'ensemble des indices i du tableau, dans l'ordre (de 0 à 7),
 *       pour configurer la broche de commande de la LED correspondante comme
 *       une broche de sortie (pour pouvoir lui envoyer un signal électrique).
 */
void setup() {

    for (uint8_t i=0; i<NUM_LEDS; i++) {
        pinMode(LED_PIN[i], OUTPUT);
    }
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note On balaie les indices i du tableau, dans l'ordre (de 0 à 7), pour :
 *       1. allumer la LED correspondante,
 *       2. faire une pause de 40 ms (pendant laquelle la LED restera allumée),
 *       3. puis éteindre la LED, avant de passer à la suivante.
 */
void loop() {

    for (uint8_t i=0; i<NUM_LEDS; i++) {

        digitalWrite(LED_PIN[i], HIGH);
        delay(40);
        
        digitalWrite(LED_PIN[i], LOW);

    }

}