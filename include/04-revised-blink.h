/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Clignotement revisité d'une LED : sans utiliser la fonction delay()
 * -------------------------------------------------------------------------
 */

#include <Arduino.h>

/**
 * @brief Définition de la broche de commande de la LED.
 */
const uint8_t LED_PIN = 5;

/**
 * @brief Durée pendant laquelle chaque LED reste allumée (exprimée en millisecondes).
 */
const uint8_t FLASH_DURATION_MS = 100;

/**
 * @brief Période du clignotement (exprimée en millisecondes)
 */
const uint16_t FLASHING_PERIOD_MS = 1000;

/**
 * @brief Démarrage du programme.
 */
void setup() {

    pinMode(LED_PIN, OUTPUT);
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Ici on n'utilise plus du tout la fonction delay() pour maintenir la LED allumée.
 *       On détermine simplement son état en fonction du temps qui s'écoule grâce à la
 *       fonction millis() qui retourne le nombre de millisecondes écoulées depuis le
 *       démarrage du programme.
 * 
 *            |<- f ->|
 *            |       |
 *       HIGH +-------+             +-------+             +-------+             
 *            |       |             |       |             |       |             
 *            |       |             |       |             |       |             
 *            |       |             |       |             |       |             
 *        LOW +       +-------------+       +-------------+       +-------------> T : millis()
 *            |                     |
 *            |<-------- P -------->|
 *       
 *       On souhaite maintenir la LED allumée pendant une brève durée `f` pour produire
 *       un flash, puis l'éteindre jusqu'au prochain flash. Et on souhaite que chaque
 *       flash se produise à intervalles réguliers de période `P`. Ainsi :
 * 
 *       - `f` est définie par la constante `FLASH_DURATION_MS`
 *       - `P` est définie par la constante `FLASH_PERIOD_MS`
 * 
 *       Pour induire un traitement périodique sur l'écoulement du temps, on utilisera ici
 *       l'opérateur de congruence % (modulo). En effet, pour 2 entiers a >= b :
 * 
 *       a % b  retourne le reste de la division euclidienne de a par b
 * 
 *       Combiné à la fonction millis(), qui retourne une date `T` absolue, on obtient
 *       un moyen simple de mesurer le temps de manière relative à chaque période `P`.
 * 
 *       En effet,  millis() % FLASHING_PERIOD_MS  retournera toujours une date `t` qui
 *       satisfait la relation d'ordre suivante :
 * 
 *           0 <= t < FLASHING_PERIOD_MS
 * 
 *       Autrement dit, à tout instant `T` absolu, on saura toujours lui faire correspondre
 *       un instant `t` relatif à la période `P` dans laquelle on se trouve.
 * 
 *       Pour déterminer si la LED doit être allumée ou éteinte, il suffira donc de savoir
 *       si l'instant `t` est antérieur ou postérieur à `FLASH_DURATION_MS`.
 */
void loop() {

    // ------------------------------------------
    // Le code est, cette fois-ci, non bloquant !
    // ------------------------------------------
    // 
    // En effet, à aucun moment on n'interrompt l'exécution du programme par une pause
    // comme on le faisait auparavant en utilisant la fonction delay(). On effectue
    // simplement un contrôle du temps écoulé pour déterminer, de manière instantanée,
    // si on doit allumer ou éteindre la LED :

    digitalWrite(LED_PIN, (millis() % FLASHING_PERIOD_MS) < FLASH_DURATION_MS);

    // qui est une forme condensée de la séquence suivante :
    // 
    // 
    // uint32_t t = millis() % FLASHING_PERIOD_MS;
    // 
    // if (t < FLASH_DURATION_MS) {
    // 
    //     digitalWrite(LED_PIN, HIGH);
    // 
    // } else {
    // 
    //     digitalWrite(LED_PIN, LOW);
    // 
    // }

}