# SAE 31 - Centre de tri de colis

## Pour commencer : comment utiliser Git ?

### Comment installer Git et clôner le dépôt sur sa machine locale ?

1] Installer Git sur sa machine puis taper dans l'invite de commande :

```sh
sudo apt install git
```

2] Configurer vos informations d'utilisateur :

```sh
git config --global user.name "[Prenom] [Nom]"
git config --global user.email "[login]@unistra.fr"
```

3] Clôner le dépôt "SAE 31 - Centre de tri de colis" sur sa machine locale :

```sh
git clone https://git.unistra.fr/trio-de-ayoub/sae-31-centre-de-tri-de-colis.git
```

Si toutes les étapes ont été respectées, vous avez maintenant un dossier `sae-31-centre-de-tri-de-colis` sur votre machine qui contient tous les éléments du dépôt Git.

### Comment envoyer son travail ?

1] Le fichier `.gitignore` (à la racine du dépôt) indique à Git quels fichiers ignorer : les fichiers compilés, le fichier journal et les pipes nommés (qui apparaissent comme des fichiers sur le disque). Il se crée une seule fois, puis les autres le récupèrent avec `git pull` :

```sh
cat > .gitignore << 'EOF'
# Compilation
Code/bin/
Code/build/
Code/*.o
Code/main
Code/tri
Code/affichage

# Journal et pipes nommés
*.log
*.fifo

# Éditeurs
.vscode/
*.swp
EOF
```

Si un fichier à ignorer a déjà été envoyé, il faut le retirer du suivi (sans le supprimer de sa machine) :

```sh
git rm --cached <fichier>
git commit -m "Retrait d'un fichier ignoré"
```

2] Ajouter les fichiers modifiés à la zone de préparation :

```sh
cd sae-31-centre-de-tri-de-colis
git add .
git status
```

3] Créer un commit :

```sh
git commit -m "nom_commit"
```

4] Envoyer ses commits vers le dépôt distant :

```sh
git push
```

### Comment mettre à jour son clone ?

```sh
cd sae-31-centre-de-tri-de-colis
git pull origin main
```

Il est important de mettre à jour son dépôt avant de modifier un fichier pour obtenir les mises à jour des autres commits.

---

## Le sujet

On simule un centre de tri de colis. L'utilisateur tape des commandes :

- `COLIS <id> <destination> <poids>` : un colis arrive
- `ETAT` : affiche l'état du centre
- `QUIT` : arrête tout proprement

Un colis est valide si son id n'est pas vide, sa destination vaut `A`, `B` ou `C` et son poids est > 0. Une commande invalide ne doit pas faire planter le programme.

Chaque colis valide est envoyé vers sa zone (A, B ou C). Chaque zone compte ses colis et son poids total, et le centre tient des stats globales (colis traités, poids total, dernier colis). Les 3 zones tournent en parallèle.

La simulation s'arrête au bout de **20 colis valides** ou sur `QUIT`.

## Ce qu'on doit utiliser

**3 processus :**

- **Main** : lit les commandes au clavier et les envoie à TRI par un **pipe nommé**.
- **TRI** : fait tout le travail avec **6 threads** (détail ci-dessous).
- **Affichage** : reçoit l'état du centre par un **pipe nommé**, l'affiche dans la console et écrit l'historique dans un fichier journal.

**Les 6 threads de TRI :**

- **Main** : crée les autres threads, lit le pipe nommé venant du processus Main, passe les `COLIS` au thread Tri par un **pipe anonyme**, et gère `QUIT` en arrêtant tout le monde avec des **signaux**.
- **Tri & Routage** : valide le colis et l'envoie à la bonne zone par **3 pipes anonymes** (un par zone).
- **Zone A / B / C** : reçoivent leurs colis et mettent à jour les stats de leur zone.
- **Supervision** : récupère les infos des zones, met à jour les stats globales, vérifie si les 20 colis sont atteints, envoie l'état au processus Affichage (pipe nommé) et prévient le thread Main par **signal**.

**Le point libre :** la communication entre les zones et la Supervision est à notre choix (mutex, sémaphores, variables de condition, etc.) et doit être **justifiée**.

```
Utilisateur → [Main] --pipe nommé--> [TRI : Main → Tri → Zones A/B/C → Supervision] --pipe nommé--> [Affichage]
                 ↑______signaux______|                                   |______signaux______↑
```