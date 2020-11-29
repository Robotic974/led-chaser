/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Balayage bidirectionnel
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
 * @brief Durée pendant laquelle chaque LED reste allumée (exprimée en millisecondes).
 */
const uint8_t FLASH_DURATION_MS = 40;

/**
 * @brief Indice de la broche de commande active dans le tableau LED_PIN.
 */
uint8_t active_pin = 0;

/**
 * @brief Sens de progression de la broche de commande active.
 * 
 * @note Effectuer un balayage bidirectionnel du chenillard revient à allumer puis éteindre
 *       chaque LED, tour à tour, en effectuant des allers-retours sur toute la longeur
 *       du chenillard, donc en suivant une progression par décalage d'une LED à la fois,
 *       dans un sens puis dans l'autre.
 * 
 *       +1 indique une progression dans le sens croissant des indices du tableau LED_PIN.
 *       -1 indique une progression dans le sens décroissant.
 */
int8_t direction = 1;

/**
 * @brief Marqueur temporel (timecode) permettant de dater le dernier décalage opéré sur le chenillard.
 * 
 * @note Le fait de dater l'événement va nous permettre de mesurer le délai qui s'écoule entre
 *       le moment où une LED est allumée et la date courante. Le datage sera exprimé en millisecondes
 *       et obtenu grâce à la fonction millis() qui retourne le nombre de millisecondes qui se sont
 *       écoulées depuis le démarrage du programme. Cette fonction nous fournit donc une échelle de temps
 *       satisfaisante pour mesure la durée qui doit s'écouler entre deux décalages de LED.
 * 
 *       Le fait d'effectuer ces mesures temporelles nous affranchit de l'usage de la fonction delay() qui,
 *       je vous le rappelle, est bloquante : le micro-contrôleur ne peut rien faire d'autre que d'attendre
 *       que la durée spécifiée soit écoulée.
 * 
 *       Alors que si l'on gère l'enchaînement des événements en déterminant à quelle date ils doivent se
 *       produire, on peut aisément déclencher différents événements de manière concurrente, sans bloquer
 *       le flot d'exécution du programme. On peut ainsi conduire l'exécution de plusieurs tâches simultanées
 *       sans qu'elles se bloquent les unes les autres.
 */
uint32_t last_shift_ms = 0;

/**
 * @brief Démarrage du programme.
 */
void setup() {

    // Configuration des broches de commande des LEDs.
    for (uint8_t i=0; i<NUM_LEDS; i++) {
        pinMode(LED_PIN[i], OUTPUT);
    }

    // On allume la première LED du chenillard.
    digitalWrite(LED_PIN[active_pin], HIGH);

    // On initialise le timecode qui marque l'allumage d'une LED.
    last_shift_ms = millis();
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Vous remarquerez qu'ici aucun appel à la fonction delay() n'est effectué.
 *       Par conséquent, la boucle loop() s'effectue continuellement sans jamais
 *       être interrompue par un temps de pause.
 * 
 *       Pourtant cela n'empêche en aucun cas de maintenir une LED allumée pendant
 *       la durée souhaitée, qui est définie par la constante FLASH_DURATION_MS.
 */
void loop() {

    // On calcule la durée en millisecondes qui s'est écoulée
    // depuis le dernier décalage opéré sur le chenillard,
    // c'est-à-dire la dernière fois qu'on a allumé une LED.
    uint32_t t = millis() - last_shift_ms;

    // Si la période pendant laquelle la LED doit rester allumée est dépassée...
    if (t > FLASH_DURATION_MS) {

        // ... alors on éteint la LED encore active.
        digitalWrite(LED_PIN[active_pin], LOW);

        // On vérifie les cas où l'on a atteint les extrémités du chenillard,
        // auquel cas il faut inverser la direction du décalage.
        if ((direction == -1 && active_pin == 0) || (direction == 1 && active_pin == NUM_LEDS - 1)) {
            direction *= -1; // équivaut à : direction = direction * (-1);
        }

        // Puis on applique le décalage (dans le bon sens) et on allume la nouvelle LED.
        digitalWrite(LED_PIN[active_pin += direction], HIGH);
        // L'instruction ci-dessus est une forme condensée de la séquence suivante :
        //   active_pin = active_pin + direction;
        //   digitalWrite(LED_PIN[active_pin], HIGH);

        // On sauvegarde le timecode qui marque l'allumage d'une nouvelle LED.
        last_shift_ms = millis();

    }

}