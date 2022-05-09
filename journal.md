# Journal

**Maxime Merlin** et **Simon Liétar**

- **12/03** – Première séance de programmation du projet : Création d'une classe `CircularCollider` et gestion du déplacement dans le monde torique grâce à l'algorithme de clamping. Partie 1 réussie sans problème.
- **18/03** – Partie 2.1 et rédaction des réponses.
- **19/03** – Partie 2.2 : ajout des classes `Animal`, `Environment` et `ChasingAutomaton`.
- **26/03** – Partie 2.3. Séparation des fichiers `.hpp` et `.cpp` après avoir tout mis dans le `.hpp` les semaines précédentes. Plusieurs erreurs de dépendance circulaire corrigées en modifiant les `#include`. Pas d'erreurs lors des tests. Plusieurs _notes_ et _warnings_ à régler toutefois.
  - Le système de déambulation donne parfois lieu à des comportements étranges (à investiguer).
- **02/04** – Parties 3.1 et 3.2, correction des _warnings_.
  - Le ralentissement pour se nourrir donne lieu à une accélération non voulue et de cause inconnue.
- **08/04** – Nettoyage du code, réécriture de `Animal#draw()`, correction de bugs.
- **09/04** – Partie 3.3
- **16/04** – Partie 3.4
  - Le processus de naissance ne correspond pas exactement à l'énoncé : il y a un délai avant la naissance de chaque enfant et non après celle de tous les enfants.
  - Certains cas du système à états restent à tester (par exemple l'arrivée d'un partenaire sur un autre autre qui est en train de se nourrir).
  - Certaines méthodes (par exemple `Animal#draw()` et `Animal#updateState()`) manquent de modularité.
- **17/04** – Corrections mineures, amélioration de `Animal#updateState()`, écriture des réponses.
- **30/04** –  Ajout de `Waves`, de la fonctionnalité `RUNNING_AWAY`, de `WaveGerbil`, de `Rock`. Début du codage de `NeuronalScorpion` et `Sensors`. Correction de bugs.
- **2/05** – Corrections mineures, modularisation de plusieurs fonctionnalités, amélioration de plusieurs parties, notamment les `Waves`.
- **6/05** – Correction de la vitesse du `NeuronalScorpion`, ajout de `Stats`, amélioration de `Environment`.
- **7/05** – Fin de `Stats`, correction de bugs, début d'ajout d'extensions mineures :
  - Les animaux naissent avec une petite taille et champ de vision, les deux augmentant en rapport avec leur âge.
  - Ajout d'une horloge globale pour déterminer des saisons, qui vont augmenter ou réduire la reproduction entre animaux via la vitesse et l'énergie requise.
  - Ajout d'une fonctionnalité de simulation sans affichage graphique, ce qui permet de mieux se rendre compte des résultats de la simulation avec un graphique sur gnuplot.
  - **8/05**  – Améliorations mineures.
  - **11/05** – Ajout de getters et correction de bugs.
  -  **14/05** – Améliorations du code :
    - Toutes les variables et fonctions suivent la même convention d'écriture.
    - Ajout de commentaires sur les méthodes importantes dans les fichiers `.hpp`.
    - Ajout de la navigation entre graphiques.
    - Accès aux statistiques via `StatsAccessor`.
  - **21/05** – Ajout de la fonctionnalité de fuite en groupe pour les gerbilles. Correction de bugs.
  - **22/05** – Améliorations, ajout du brouillard.
  - **23/05** – Correction de segfault, correction de bugs, amélioration du brouillard.
  - **24/05** – Amélioration de `FoodGenerator`et du double dispatch`, fonctionnalité d'activation du brouillard et les jeunes animaux suivent leur mère pendant un certain temps.
  - **25/05** –
  
### Bugs connus/Améliorations

- [ ] la méthode `Animal#draw()` devrait être modularisée, en particulier pour l'affichage d'informations de debug
- [ ] il faudrait réécrir la gestion de la vitesse avec l'enum `Speed`
- [ ] il manque plusieurs `const` et passages par référence
- [ ] plusieurs propriétés et méthodes sont publiques au lieu d'être protégées ou privées 

### Liste des extensions :

- ##### Modifications :
    -  
    - 
    -
    - 
    - 
    - 
    - 
- ##### Ajouts :
    - Les gerbilles et scorpions naissent petits et grandissent jusqu'à ce qu'ils soient capables de se reproduire. Cela inclue la taille du sprite, le rayon de vue et le rayon de collision.
    -  Affichage des FPS et du temps écoulé dans le monde dans la fenêtre de contrôle.
    - Si une gerbille voit un scorpion, elle prévient toutes les gerbilles dans 1.5 son rayon de vue.
    - Fonctionnalité de simulation sans affichage, appelée `headless`.
    -  Les waves sont adaptées pour fonctionner dans un monde torique.
    -  L'affichage debug affiche l'énergie d'un animal comme `energy - getMinEnergy()`.
    - Brouillard aléatoire qui affecte le champ de visions des animaux.
    - 
    - 
    - 
    - 




