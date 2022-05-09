# Réponses

- **Q1.1** On crée une fonction externe à la classe `CircularCollider` prenant en paramètres la valeur à modifier et les valeurs limites, ce qui fait que l’algorithme est applicable dans la classe `CircularCollider` sans duplication de code.

- **Q1.2** Il est préférable d’expliciter le constructeur et le constructeur de copie même si ceux par défaut nous conviennent car, de cette manière, on peut s’assurer de ce que le code va faire, ce qui facilite la compréhension globale du programme.

- **Q1.3** Pour chaque coordonnée du vecteur `to - from`, on utilise l'algorithme de _clamping_, qui utilise lui-même deux boucles, l'une pour incrémenter la coordonnée tant qu'elle est inférieure à `-width / 2.0` ou `-height / 2.0` et l'autre pour décrémenter la coordonnée tant qu'elle est supérieure à `+width / 2.0` ou `+height / 2.0`.

- **Q1.4** Il est judicieux de passer tous les arguments des méthodes par référence constante parce qu’on ne modifie aucun des arguments dans les 5 méthodes.

- **Q1.5** Il est judicieux de déclarer toutes les méthodes sauf `move` et l’opérateur `+=` comme `const` car ce sont les seules qui modifient l’instance courante.

- **Q1.6** On réutilise les méthodes décrites plus haut: `isCircularColliderInside()` ou `isPointInside()` pour `>` et `isColliding()` pour `|`.

- **Q1.7** On est obligé d'utiliser une surcharge externe pour l'opérateur `<<` car elle s'applique sur un `ostream`. On utilise également une surchage externe pour les opérateurs `|` et `>` pour conserver une symétrie entre les deux paramètres. La surchage de l'opérateur `>` avec deux paramètres différents (un `CircularCollider` et un `Vec2d`) est interne car elle n'a pas de symétrie.

- **Q1.8** On passe tous les arguments par référence constante car on ne modifie aucun d’entre eux.

- **Q1.9** On passe les 3 méthodes ainsi que celle de la surchage interne de l'opérateur `>` comme `const` car aucune d’entre elles ne modifie l’instance courante.

- **Q2.1** On déclare la méthode `draw()` parce que le dessin ne modifie pas l'instance courante, à l'inverse des autres méthodes.

- **Q2.2** On met le constructeur de copie en privé pour empêcher la copie d'un environnement.

- **Q2.3** Lors de la destruction d’un environnement, les animaux sont détruits aussi.

- **Q2.4** Le fichier qui met en place l’utilisation de ces touches est le fichier `src/Application.cpp`.

- **Q2.5** On utilise une méthode `Vec2d ChasingAutomaton::getForce() const` qui calcule la force en question et une autre `void ChasingAutomation::update(sf::Time dt)`.

- **Q2.6** On peut assigner un entier à chaque valeur possible du type énuméré qui soit proportionnel à la variable `deceleration`. On peut accepter un paramètre de ce type dans le constructeur de `ChasingAutomaton` en lui donnant toutefois une valeur par défaut.

- **Q2.7** On veut empêcher d'autres fonctions de modifier la position et la rotation ; elles devront le faire par d'autres moyens (par exemple initialement dans le constructeur). On veut par contre permettre à un surensemble d'un `Animal` de modifier sa propre position et rotation, d'où l'utilisation d'une méthode protégée.

- **Q2.8** Il faut modifier la méthode `draw()` de `Environment` pour que celle-ci fasse appel à la méthode `draw()` de chaque animal que l'environemment contient.

- **Q2.9** La classe `DummyAnimal` définit une méthode publique `setHeading()` qui accède à la méthode protégée `setRotation()` afin de pouvoir modifier la rotation (donc la direction) depuis l'extérieur de la classe.

- **Q2.10** On retourne un `vector<OrganicEntity*>` car le type `list` est préféré pour les ensembles auxquels on ajoute/retire souvent des éléments, ce qui n'est pas le cas ici. On utilise également un pointeur car on ne veut pas copier l'objet.

- **Q2.11** Il faut modifier la méthode `update()` de `Environment` pour que celle-ci fasse appel à la méthode `update()` de chaque animal que l'environemment contient.

- **Q3.1** Les méthodes qui doivent être déclarées comme virtuelles pures sont toutes celles qui vont être utilisées dans les classes héritant de Animal et non implémentées directement dans `Animal`, soit, globalement, toutes les méthodes `getXxx()` qui vont être particulièrement utiles dans les classes `Gerbil` et `Scorpion`. `override` sera utilisé lorsque nous implémenterons les méthodes virtuelles de `Animal` dans le `.hpp` de `Scorpion` et `Gerbil`.

- **Q3.2** Dans le fichier JSON, on peut trouver une valeur nommée `gerbil : energy : initial`. Si on la modifie, cela modifiera la valeur de l’énergie initiale des gerbilles. 

- **Q3.3** Les classes `Updatable` et `Drawable` possèdent des méthodes virtuelles pures respectivement nommées `update` et `draw`. Or dans `Animal`, `OrganicEntity` et `Food`, par exemple, on utilise déjà des méthodes semblables, on va donc inclure `Updatable` et `Drawable` dans ces fichiers puis mettre leurs méthodes respectives en override pour pouvoir les employer. De cette manière, l'héritage multiple impose le prototype de ces méthodes et garantit une cohérence entre les classes qui les implémentent.

- **Q3.4** L'utilisation de test de types à l'éxécution par `dynamic_cast` requière l'utilisation de _Run-time type information_ (RTTI), coûteux en ressources.

- **Q3.5** D'un point de vue sémantique, la tâche première d'un `CircularCollider` n'est pas d'être dessiné mais de gérer les collisions. Il serait intéressant que ce soit lui-même qui gère l'affichage de sa zone de collision, toutefois ça n'a que peu de sens de lui faire hériter de classe `Drawable` uniquement pour des raisons de debug. D'un point de vue technique, la classe `OrganicEntity` est la seule à hériter de `CircularCollider`, il n'y a donc pas de problème à déléguer cette tâche à `OrganicEntity`, qui est explicitement dessinable.

- **Q3.6** On peut implémenter la méthode `OrganicEntity#update()` qui va augmenter la propriété `age` de `dt` à chaque actualisation. La méthode virtuelle `getLongevity()` dans `OrganicEntity` définit la longévité par défault. Elle peut être réécrite par `Gerbil` et `Scorption` en fonction des valeurs indiquées dans le fichier JSON.

- **Q3.7** Pour gérer la mort des animaux, on crée une méthode `isDead()` dans `OrganicEntity` qui va vérifier si le temps vécu par un animal est supérieur à sa longévité ou si son énérgie est inférieure à son énergie minimale. La méthode `update()` de `Environment` vérifie pour chaque entité si `isDead()` renvoir `true`. Si c'est le cas, on assigne cette entité (qui est ici un pointeur vers une `OrganicEntity`) à `nullptr`. On supprime ensuite tous les `nullptr` de la `list` de pointeurs.

- **Q3.8** On utilise déjà un `switch` pour augmenter la vitesse selon l’état de l’animal, on peut alors simplement ajouter une condition pour que l'animal ralentisse s'il est dans l'état `WANDERING` et que son énergie est en dessous d’un certain seuil (par exemple la moitié de son énergie initiale).

- **Q3.9** On aurait pu utiliser une grille avec un système de coordonnées entières. Les collisions auraient été moins réalistes, les déplacements plus compliqués à implémenter et moins fluides, mais cela aurait permis, entre autres, l'utilisation d'un algorithme de _pathfinding_ tel qu'A*.

- **Q3.10** On crée des méthodes virtuelles pures dans `OrganicEntity` nommées par exemple `meet(OrganicEntity* mate)`, `metBy(Animal* mate)`, et `metBy(Food* mate)`. On peut ainsi gérer tous les cas dans les différentes classes `Animal` et `Food` en fonction des fonctionnalités qu’on veut atteindre. On n’utilise ainsi aucun test de type.

- **Q3.11** Pour mettre en place le temps de gestation, on crée une propriété `gestation_end_age` de type `sf::Time` dans la classe `Animal`, puis avec une condition dans `update()`, on vérifie  quand `age > gestation_end_age`, où `gestation_end_age` est l'âge auquel la gestation est terminée et a été initialisé à `age + getGestationTime()`, `getGestationTime()` étant réécrite par chaque type d'animal. On a de cette façon pas besoin de décrémenter la propriété qui gère le temps de gestation.

- **Q3.12** On créé une méthode virtuelle pure `giveBirth()` dans `Animal` qu’on implémente dans `Gerbil` et `Scorpion` par la création d'un objet à la même position que l'object actuel. On utilise la méthode `addEntity()` de `Environment` pour ajouter le nouvel animal à l'environnement.

- **Q3.13** Le nombre de bébés est stocké dans une propriété `nb_children` initialisée à 0, que l’on modifie dans `meet()`. Les mâles, bien qu'il ne puissent pas porter de bébés, ont quand même cette propriété mais elle reste à 0.

- **Q4.1** La class `Wave` hérite de `CircularCollider` (qui hérite elle même de `Drawable`) pour pouvoir gérer les collisions et être affichée, et de `Updatable` pour avoir un rayon variable en fonction du temps et pouvoir se fragmenter.

- **Q4.2** (à voir).

- **Q4.3** On doit appeler les méthodes `draw()` et `update()` de `Wave` dans leurs analogues de `Environment`.

- **Q4.4** On doit détruire les objets de la classe `Rock` lorsque `Environment#clean()` est appelée.

- **Q4.5** On laisse chaque `Sensor` stocker son angle avec une propriété `angle` pour que sa méthode `draw()` ait directement accès à sa position et puisse le dessiner indépendamment du scorpion.

- **Q4.6** On utilise `Vec2d getPositionOfSensor(double angle) const`. La méthode est appelée par chaque `Sensor`, qui fournit son angle et récupère sa position absolue. Elle ne modifie pas le scorpion, donc est marquée `const`.

- **Q4.7** Le calcul est géré par `Sensor#lookForWaves()`. De façon analogue à `getEntitiesInSightForAnimal()` et `getFragmentingCollidersForWave()` codées précédemment, on ajoute une méthode `getCollidingWavesForSensor()` à `Environment` pour recenser les ondes qui touchent le senseur. La collision est détectée via un appel à la méthode `isWaveColliding()` de `Sensor`. La méthode `lookForWaves()` termine ensuite le calcul.

- **Q4.8** On utilise une propriété `scorpion` de type `Scorpion*` sur la classe `Sensor` pour stocker un pointeur sur le scorpion correspondant au senseur. Cela implique que le scorpion doit exister au moins jusqu'à la destruction du senseur. Cela ne pose pas de problème puisque les senseurs sont stockés sans pointeurs dans le scorpion.

- **Q4.9** Les senseurs doivent être créés par le scorpion dans son constructeur pour avoir accès à `this`.

- **Q4.10** On modélise les états de façon semblable à `Animal`, avec une propriété `state` dont le type correspond à l'enum `NeuronalState`.

- **Q4.11** On modélise les horloges de la même façon que pour `Animal`, avec la même propriété `interactionEndAge` qui marque l'âge de fin d'un état ou d'un processus et que l'on compare à la propriété `age` héritée de `OrganicEntity`.

- **Q4.12** Les actions sont:
  - s'il n'y a pas de cible visible, que l'on est dans l'état `MOVING` et que `age > interactionEndAge`:
    - passer à l'état `IDLE`
    - régler `interactionEndAge` à `age + sf::seconds(5)` (qui entraînera éventuellement un passage à `WANDERING`)
  - s'il n'y a pas de cible visible, que l'on est dans l'état `IDLE` et que le score est suffisant pour une action:
    - passer à l'état `MOVING`
    - régler `interactionEndAge` à `age + sf::seconds(3)` (qui entraînera éventuellement un passage à `IDLE`)
  - s'il n'y a pas de cible visible, que l'on est dans l'état `IDLE` et que `age > interactionEndAge`:
    - passer à l'état `WANDERING`

- **Q5.1**

- **Q5.2** On utilise une classe dédiée `StatsAccessor` pour pouvoir la réutiliser dans l'extension qui propose une version sans affichage. Pour éviter d'avoir recours à trop de getters, on la règle comme `friend` avec `Environment` et `OrganicEntity` pour lire leurs propriétés privées. Pour pouvoir différencier les types d'entités, on utilise du double dispatch en appelant sur chaque entité la méthode `OrganicEntity#fetchStats()` qui appelle `StatsAccessor#processEntity()` correspondante en fonction de son type, `Food`, `Gerbil` ou `Scorpion`.

- **Q5.3** 

