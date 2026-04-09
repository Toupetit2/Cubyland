Tuto lancement:

Télécharger le photon engine en 2.0.8 (lien sur le Discord GTECH 2, dans dev-lien)
Extraire le fichier téléchargé
Cloner ce repository
Ouvrir le fichier du moteur qui a été extrait, et copier l'intégralité
Coller dans le dossier de ce repository sur votre machine et cliquer sur Remplacer tout
Par sécurité, dans le dossier scripting:
- Ouvrir defaultCubyData.json
- Copier l'intégralité
- Ouvrir cubyData.json
- Coller
- Aller dans le dossier scripts (pas scripting!) et supprimer le contenu
(Cela évite d'éventuelles données sauvegardées pendant les tests)
Ouvrir PhotonEngine-Editor.exe
Dans l'explorateur en bas à droite, double cliquer sur Scenes
Cliquer sur LV_World.pscene puis sur load (en bas)
Pour chaque objet listé ci-dessous, cliquer dessus et à droite de l'écran, cliquer sur recompile script (si jamais, les entités sont à gauche de l'écran)
Cuby1
Cuby2
Cuby3
Main Camera
Sauvegarder la scene et la nommer LV_World (en haut à gauche, file -> save)
Faire parreil dans LV_Fight2.pscene pour:
Cam
Fight
Save et nommer LV_Fight2
(ceci est dû à la structure du moteur)
Load de nouveau LV_World.pscene
Appuyer sur play

Contrôles : ZQSD (dans la scène où vous contrôlez un cône), attention à bien être "dans" le jeu (cliquer dans le viewport, clic droit ou gauche et maintenir)
Pour commencer un combat, se diriger vers un cube (plus il est grand, plus son niveau est élevé)
Dans la scène de combat, vous pouvez tourner la caméra si vous maintenez clic droit ou clic gauche.
Le combat se passe dans l'interface.

Au début, choisissez votre cuby puis cliquez sur le bouton qui apparaît en bas de l'interface
Attaquer: permet d'attaquer (vous êtes prioritaire sur l'ennemi)
Fuir: Enclenche une fuite sur 2 tours (vous ne pouvez plus attaquer pendant la fuite et êtes sans défense face aux attaques de l'ennemi)
Si vous gagnez et que vous n'êtes pas au niveau max (3), vous gagnez 50 points d'XP
Si vous perdez ou fuyez, vous ne gagnez rien.
Au bout de 100 points d'XP (2 victoires) vous gagnez 1 niveau et vos stats augmentent (ainsi que le nom du cuby)
Lorsque le combat est terminé, vous revenez sur sur la scène d'avant et vous pouvez recommencer.

En cas de crash, copier le contenu (dans le dossier extrait du téléchargement) du dossier src et le coller dans le dossier src du repo (de notre jeu)

En cas de problème, contacter le support