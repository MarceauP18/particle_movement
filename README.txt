README : 


Projet réalisé par Alexandre Simaillaud et Marceau Payot


Objectif : Ajouter des particules dans un environnement défini, et que celles-ci réagissent en suivant les lois physiques.

Lois physiques choisies : 
- Contrainte entre chaque particules : elles rebondissent les unes avec les autres
- Contrainte avec des plans et/ou des sphères fixes : Les particules rebondissent sur les plans et les sphères fixes en suivant les lois physiques
- Forces exercées sur les particules : Poids et Frottements


Features ajoutées pour améliorer l'expérience utilisateur :
- Ajout de 4 plans créants une boîte fermée dans laquelle les particules peuvent évoluer
- Ajout d'une sphère fixe sur laquelle les particules peuvent rebondir
- Au clic, ajout d'une particule à l'endroit donné, avec une couleur aléatoire entre 15
- Possibilité de modifier les attributs physique de la particule au clic : Poids, Rayon, Vitesse initiale


Différences avec la réalité : 
- Formule pour le contact plan par forcément 100% réaliste : Par exemple, pour un plan horizontal (de normale (0,1)), la particule va forcément rebondir de manière verticale selon notre formule, même si elle arrive sur le plan avec une vitesse horizontale non nulle. En réalité, quand on lance une balle de tennis contre le sol devant nous, le rebond lui donne naturellement une vitesse en x, alors que le plan est horizontal.


Aides extérieures : 
- Utilisation de ChatGPT pour créer les opérations entre vecteurs pour gagner du temps
- Aide d'internet pour la formule de force de frottement et l'obtention des constantes


Utilisation du projet : 
- Téléchargement de Tkinter
- Build dans VSCode de CMake (téléchargement des extensions associées)
- Dans le terminal, se mettre dans le répertoire de Build
- Executer la commande : python3 particle_gui.py

Suggestion de manipulation: 
- Changer le plan linéaire du sol en un plan incliné - remplacer la ligne 78 de particle_gui par : 
self.addPlan((-6,-8),(0,1)) #Sol
- Rebuild le projet et réexecuter la commande pour expérimenter un plan incliné