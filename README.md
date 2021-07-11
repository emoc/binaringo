# binaringo

(Projet en cours / premier jet 11 juillet 2021)

Boîte à rythme algorithmique pour la création de motifs rythmiques MIDI. L'algorithme est de type «Stochastic binary subdivision» décrit par Peter S. Langston dans «[Six Techniques for Algorithmic Music Composition (1988)](http://peterlangston.com/Papers/amc.pdf)».

Il est implémenté dans un programme réalisé avec Processing pour tester mais surtout dans un circuit utilisant un arduino nano, un écran monochrome de petite taille, un encodeur rotatif et quelques boutons multifonctions. L'ensemble tient sur une plaque d'essai de petit format.  
On peut aussi se fabriquer un PCB (réalisé sur kicad), mais ce n'est pas tout à fait au point...

## Prototypes

Deux programmes réalisés avec processing pour tester l'algorithme. La version 001 implémente l'algorithme, sans plus. La version 003 est fonctionnelle pour envoyer les commandes MIDI, sauvegarder les *presets* et les *patterns*.

*Screenshot* de la version 003

![version 003](./assets/sk20170605_stochastic_binary_subdivision_003.png)

## Version circuit

![schéma fritzing](./assets/schema_fritzing_binaringo_003.png)

![schéma kicad](./assets/binaringo_schema_kicad.png)

![photo](./assets/binaringo_002_photo_br.jpg)

Versions actuelles du code :
* **binaringo_009** : dernière version
* **binaringo 010** : version adaptée pour Yamaha QY70 (Note Off MIDI pour chaque note envoyée)

Historique des versions
* binaringo_004 : intéressant à conserver car utilise l'affichage u8g2
* binaringo_007 : version fonctionnelle
* binaringo_008 : version fonctionnelle, refonte avec nouvelles fonctions : swing, fixed on/off, plusieurs "views"
* binaringo_009 : version fonctionnelle débuguée
* binaringo_010 : version pour Yamaha QY70, qqes autres bugs corrigés
