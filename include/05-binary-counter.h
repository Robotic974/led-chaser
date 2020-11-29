/*
 * -------------------------------------------------------------------------
 * Atelier de programmation Robotic 974
 * © 2020 Stéphane Calderoni
 * -------------------------------------------------------------------------
 * Introduction à la programmation des cartes Arduino
 * Implémentation d'un chenillard à 8 LEDs
 * -------------------------------------------------------------------------
 * Affichage d'un compteur binaire
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
 * @brief Durée pendant laquelle on affiche la valeur courante du compteur.
 */
const uint8_t STEP_DURATION_MS = 100;

/**
 * @brief Date à laquelle on a effectué le dernier affichage du compteur.
 */
uint32_t last_step_ms = 0;

/**
 * @brief Valeur courante du compteur.
 */
uint8_t counter = 0;

/**
 * @brief Affichage d'un entier sur la rampe de LEDs.
 * 
 * @param n entier compris dans l'intervalle [0,255] (codé sur 8 bits).
 * 
 * @note La valeur de l'entier est ici décomposée en une séquence ordonnée
 *       de bits pour former un octet. Chaque LED permet ainsi d'afficher
 *       la valeur du bit correspondant :
 * 
 *       - si le bit d'indice i vaut 0, alors la LED d'indice i est éteinte
 *       - si le bit d'indice i vaut 1, alors la LED d'indice i est allumée
 */
void ledWrite(uint8_t n) {

    // On parcourt, un à un, chaque bit qui compose le codage binaire
    // de l'entier que l'on souhaite afficher. Cela revient finalement
    // à parcourir chaque LED de la rampe :
    for (uint8_t i=0; i<NUM_LEDS; i++) {

        // L'astuce consiste ici à isoler le bit qui nous intéresse (donc
        // celui d'indice `i`) en combinant astucieusement les opérateurs
        // binaires suivants :
        // 
        // -----------------------------------------------------------------
        // Opérateur de décalage à droite : a >> b
        // -----------------------------------------------------------------
        // 
        // Si on considère la représentation binaire de l'entier `a`, cet
        // opérateur permet de décaler tous les bits qui composent `a` de
        // `b` crans vers la droite :
        // 
        // Exemple : 24 >> 2
        // 
        // Commençons par détailler le codage binaire du nombre 24 :
        // 
        // +-------------------------------+
        // |        indices des bits       |  qui correspondent à des puissances de 2
        // +-------------------------------+
        // | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
        // +-------------------------------+        4    3
        // | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 |  24 = 2  + 2  = 2^4 + 2^3
        // +-------------------------------+
        // |        valeurs des bits       |
        // +-------------------------------+
        // 
        // Si maintenant on opère sur 24 un décalage à droite de 2 bits :
        // 
        // +-------------------------------+
        // | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 |  24
        // +-------------------------------+             2    1
        // | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |  24 >> 2 = 2  + 2  = 2^2 + 2^1 = 4 + 2 = 6
        // +-------------------------------+
        // 
        // Les bits de droite sont "perdus" et des bits à 0 "apparaissent" à gauche.
        // 
        // -----------------------------------------------------------------
        //  Opérateur binaire AND : a & b
        // -----------------------------------------------------------------
        // 
        // Si on considère les représentations binaires des entiers `a` et `b`,
        // cet opérateur permet d'effectuer une opération logique, bit à bit,
        // sur `a` et `b` selon la table de correspondance suivante :
        // 
        // +---------------+
        // | x | y | x & y |
        // +---------------+
        // | 0 | 0 |   0   |
        // | 0 | 1 |   0   |
        // | 1 | 0 |   0   |
        // | 1 | 1 |   1   |
        // +---------------+
        // 
        // Exemple : 37 & 7
        // 
        // +-------------------------------+
        // | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 1 |  a = 37
        // +-------------------------------+
        // | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 |  b =  7
        // +===============================+
        // | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 |  a & b = 5
        // +-------------------------------+
        //           ^           ^   ^   ^
        //           |           `---+---'
        //           |               |
        // ce bit de a est masqué    |
        //    par celui de b         |
        //                           |
        //                   alors que ceux-ci
        //                   demeurent visibles
        // 
        // L'opérateur & fonctionne ainsi comme un masque et ne laisse entrevoir
        // que les bits de `a` qui correspondent aux bits à 1 de `b`.
        // 
        // -----------------------------------------------------------------
        // Combinaison des opérateurs >> et &
        // -----------------------------------------------------------------
        // 
        // Par conséquent, en combinant ces deux opérateurs, on peut aisément
        // isoler un bit en particulier sur la représentation binaire d'un
        // entier. Supposons que l'on souhaite isoler le bit d'indice 2 de
        // l'entier 37 :
        // 
        // +-------------------------------+
        // | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |  indices des bits
        // +-------------------------------+
        // | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 1 |  37
        // +-------------------------------+
        //                       ^
        //           le bit qui nous intéresse
        // 
        // Il suffira simplement d'opérer un décalage à droite de 2 bits :
        // 
        // +-------------------------------+
        // | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |  indices des bits
        // +-------------------------------+
        // | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 1 |  37 >> 2
        // +-------------------------------+
        //                               ^
        // Puis d'appliquer un masque sur le bit d'indice 0 :
        // 
        // +-------------------------------+
        // | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 1 |  37 >> 2
        // +-------------------------------+
        // | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  1  (masque de lecture du bit d'indice 0)
        // +===============================+
        // | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  (37 >> 2) & 1 = 1
        // +-------------------------------+
        //                               ^
        // On obtient 1 comme résultat final, qui correspond bien à la valeur
        // du bit d'indice 2 de l'entier de départ 37.
        // 
        // Plus généralement, si on souhaite obtenir la valeur du
        // bit d'indice `i` d'un entier quelconque `n`, il suffit
        // d'appliquer l'opération suivante :
        // 
        //     (n >> i) & 1
        // 
        // Lorsque l'on code des expressions faisant intervenir des opérateurs
        // binaires, on préfèrera souvent les écrire sous la forme :
        // 
        //     (n >> i) & 0b1
        // ou  (n >> i) & 0x1
        // 
        // L'opérande `1` de masquage est ainsi écrit sous sa forme :
        //     0b1 (binaire)
        // ou  0x1 (hexadécimale)
        // 
        // Mais cela revient strictement à la même chose !

        digitalWrite(LED_PIN[i], (n >> i) & 0x1);

        // Notez qu'on pourrait tout aussi bien procéder de la
        // manière suivante, en opérant un décalage à gauche :
        // 
        //   digitalWrite(LED_PIN[i], n & (1 << i));
        // 
        // Je vous laisse y réfléchir ;-)
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

    // On initialise la date de démarrage du compteur
    last_step_ms = millis();
    
}

/**
 * @brief Boucle de contrôle principale.
 * 
 * @note Désormais, on n'utilise plus la fonction delay().
 */
void loop() {

    // On détermine la date courante.
    uint32_t now = millis();

    // Si la durée écoulée depuis le dernier affichage du compteur
    // a dépassé la période que l'on s'est fixée...
    if (now - last_step_ms > STEP_DURATION_MS) {

        // Alors on affiche la valeur courante du compteur
        // et on l'incrémente d'une unité juste après :
        ledWrite(counter++);
        // L'instruction ci-dessus est une forme condensée de la séquence suivante :
        //   ledWrite(counter);
        //   counter = counter + 1;
        // 
        // Notez également que lorsque `counter` atteint la valeur maximale 255
        // d'un entier non signé encodé sur 8 bits :
        // 
        // +-------------------------------+
        // | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  255
        // +-------------------------------+
        // 
        // Une nouvelle incrémentation entraîne un "débordement" qui conduit
        // à la réinitialisation pure et simple du compteur à zéro :
        // 
        // +-------------------------------+
        // | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  0
        // +-------------------------------+

        // Enfin, on sauvegarde la date courante, qui marque le dernier affichage opéré.
        last_step_ms = now;

    }

}