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
(Cela évite d'éventuelles données sauvegardées pendant les tests)
Ouvrir PhotonEngine-Editor.exe
Dans l'explorateur en bas à droite, double cliquer sur Scenes
Cliquer sur LV_World.pscene
Cliquer sur load (en bas)
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

En cas de crash: supprimer le contenus du dossier script (attention, pas scripting), puis copier le contenu du dossier src issu du dossier du moteur que vous
avez extrait, et le coller dans le src du repo sur votre pc
En cas de crash (après l'étape précédente), dans LV_World.pscene et LV_Fight2.pscene, depuis le moteur, regarder un a un les objets.
S'il a un script (chemin qui finit par .dll, visible sur la droite), cliquer sur recompile script puis sauvegarder la scène une fois que tous les 
scripts sont recompilés (file (en haut à droite), save scene, et écrire LE BON NOM DE SCENE (sans l'extension)).