# Implémentation d'un chenillard à 8 LEDs

L'objectif de cet atelier est de vous faire découvrir la programmation d'une carte Arduino avec l'environnement de développement PlatformIO. Pour cela, je vous propose de réaliser un petit circuit électronique sur une platine de prototypage (*breadboard*) en connectant une rampe de 8 LEDs à une carte Arduino. J'utilise ici une carte [Arduino Nano][nano], mais vous pouvez tout à fait réaliser le même montage avec une carte [Arduino Uno][uno]. Le raccordement des LEDs sur les broches de commandes numériques de la carte Arduino est exactement le même.

<table>
    <tbody>
        <tr>
            <td><img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/1040x660/604a3538c15e081937dbfbd20aa60aad/a/0/a000066_featured_1_2.jpg" alt="Arduino Uno"></td>
            <td><img src="https://store-cdn.arduino.cc/uni/catalog/product/cache/1/image/1040x660/604a3538c15e081937dbfbd20aa60aad/A/0/A000005_featured_2.jpg" alt="Arduino Nano"></td>
        </tr>
    </tbody>
</table>


## Schéma de câblage du prototype

Les anodes (+) de chaque LED sont respectivement reliées, dans le même ordre, aux broches **D5** à **D12** (de droite à gauche) de la carte Arduino. Les cathodes (-), quant à elles, sont toutes reliées à la masse, c'est-à-dire à la broche **GND** de la carte Arduino, au travers d'une résistance de **220 Ω** pour limiter l'intensité du courant qui traversera les LEDs, et les protéger d'une dégradation qui leur serait fatale.

On utilisera ici deux platines de prototypage pour que le câblage ne soit pas trop étriqué :

<table>
    <tbody>
        <tr>
            <td><img src="assets/wiring.png" alt="Circuit"></td>
            <td><img src="assets/breadboard.jpg" alt="Circuit"></td>
        </tr>
    </tbody>
</table>


## Configuration du projet PlatformIO

La configuration du projet est définie par les directives inscrites dans le fichier `platformio.ini` :

```ini
[env:led-chaser]
platform  = atmelavr
board     = nanoatmega328
framework = arduino
```

Le projet est ici configuré pour être téléversé sur une carte Arduino **Nano**. Si, de votre côté, vous utilisez une carte Arduino **Uno**, vous devez remplacer la désignation de la carte `board` par la valeur `uno` :

```ini
[env:led-chaser]
platform  = atmelavr
board     = uno
framework = arduino
```


## Organisation des codes sources

Chaque exercice proposé correspond à un programme Arduino indépendant des autres exercices. Néanmoins, pour ne pas avoir à gérer autant de projets PlatformIO qu'il y'a d'exercices, on peut s'arranger pour tous les faire coexister au sein d'un même projet. Il existe différentes façons de mettre cela en oeuvre avec PlatformIO. Nous opterons ici pour la plus simple :

- Chaque exercice est traité dans un fichier d'en-tête (fichier portant l'extension `.h`) indépendant, stocké dans le dossier `include`.

- Le projet comporte un seul et unique programme principal `src/main.cpp` qui est chargé d'incorporer un et un seul fichier d'en-tête à la fois, correspondant à la solution de l'exercice qu'on souhaite compiler et téléverser sur la carte Arduino. Chaque fichier solution est incorporé par une directive de précompilation de type `#include`. Par conséquent, pour un exercice donné, il faudra désactiver l'incorporation des fichiers solutions des autres exercices en commentant les lignes correspondantes.

Chaque fichier solution est spécifique et indépendant des autres. Vous ne pouvez en compiler qu'un seul à la fois.


**Bon code !**


[nano]: https://store.arduino.cc/arduino-nano
[uno]:  https://store.arduino.cc/arduino-uno-rev3