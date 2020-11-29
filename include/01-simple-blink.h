/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Clignotement simple d'une seule LED
 * -------------------------------------------------------------------------
 */

#include <Arduino.h>

/**
 * @brief Définition de la broche de commande de la LED.
 */
const uint8_t LED_PIN = 5;

/**
 * @brief Démarrage du programme.
 * 
 * @note On indique au micro-contrôleur que la broche de commande de la LED
 *       doit être configurée comme une broche de sortie d'un signal électrique.
 */
void setup() {

    pinMode(LED_PIN, OUTPUT);
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Pour faire clignoter la LED, il suffit d'envoyer un signal électrique
 *       sur la broche de commande dont le potentiel doit alterner entre +5V et 0V.
 * 
 *       Cela revient à envoyer un signal numérique (binaire) alternant entre :
 *         -    la valeur 1 (HIGH)
 *         - et la valeur 0 (LOW)
 * 
 *       Une fois appliqué, le signal est maintenu pendant une courte durée en
 *       imposant au micro-contrôleur de faire une pause grâce à la fonction
 *       delay(), dont la durée est exprimée en millisecondes.
 * 
 *       Vous pouvez jouer sur la valeur des durées spéficiées pour obtenir
 *       des effets de clignotement différents.
 * 
 *       Attention : la fonction delay() est "bloquante" et interrompt le cours
 *       du programme pendant la durée spécifiée. Par conséquent, le micro-contrôleur
 *       ne pourra RIEN faire d'autre tant que cette durée n'est pas écoulée.
 */
void loop() {

    digitalWrite(LED_PIN, HIGH);
    delay(100);

    digitalWrite(LED_PIN, LOW);
    delay(900);
    
}