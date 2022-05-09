# Projet 2019 – Scorpions et gerbilles

[Énoncé](https://progsv.epfl.ch/www/projet/enonce-projet.html)


## Extensions

- **Mode sans affichage.** La classe `HeadlessApplication` héritant de `Application` utilise un environnement sans jamais le dessiner et en affiche les statistiques. La simulation dure au plus 10000 unités de temps avec un `dt` de 100 ms à chaque cycle, mais s'arrête s'il n'y a plus d'animaux dans le monde.
- **Dessin des ondes dans un monde partiellement torique.** Pour chaque onde, 9 cercles sont dessinés de façon à ce que l'onde puisse revenir sur le monde. Le mode debug annule ceci car cela peut rapidement devenir gênant bien que cela permette de comprendre des collisions autrement étonnantes.
- **Influence de l'âge des animaux.** L'âge des animaux influence leur taille, leur vitesse et leur distance de vue. Ceci est calculé par `Animal#getGrowthFactor()`.
- **Affichage et influence de brouillard.** Un brouillard est affiché grâce à shader écrit en GLSL. Il influence la distance de vue des animaux qui se trouvent dessous en la réduisant de jusque 70 %.
- **Fuite en groupe.** Les animaux en fuite sont capables de prévenir leurs congénères non-en fuite de leurs prédateurs si celles-ci sont à une distance inférieure à 1.5 fois la distance de vue de l'animal en fuite.
- **Durée de vie de la nourriture.** La nourriture a une durée de vie limitée afin d'éviter sa prolifération lors des longues simulations sans affichages.
- **Interface de simulation.** L'interface de simulation dispose des touches A/Q et E pour changer de graphique et affiche l'horloge et la fréquence d'affichage.


## Divergences avec l'énoncé

- Les naissances ne se produisent pas de la même façon.
- La fonction `clamping` ne fonctionne pas de la même façon.

