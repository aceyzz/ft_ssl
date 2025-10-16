<img title="42_ft_ssl_md5" alt="42_ft_ssl_md5" src="./utils/banner.png" width="100%">

<br>

`ft_ssl_md5` — 42

Ce projet a pour but d'implémenter un programme en C qui chiffre des données en utilisant l'algorithme MD5 et SHA-256. Le programme doit être capable de lire des données à partir de fichiers ou de l'entrée standard, et de produire des sorties formatées selon les spécifications.  

<br>

## Table des matières

- [Éxigences](#éxigences)
	- [MD5](#md5)
	- [SHA-256](#sha-256)
- [Compilation et exécution](#compilation-et-exécution)
	- [Lancer la VM avec Docker](#lancer-la-vm-avec-docker)
	- [Accéder à la VM](#accéder-à-la-vm)
	- [(facultatif) Installer OhMyBash](#facultatif-installer-ohmybash)
	- [Compiler le projet](#compiler-le-projet)
	- [Exécuter le programme](#exécuter-le-programme)
- [Aperçu rapide](#aperçu-rapide)
- [Flux de données](#flux-de-données)
- [Récap CLI \& exemples](#récap-cli--exemples)
- [Formats de sortie détaillés](#formats-de-sortie-détaillés)
- [Architecture du code](#architecture-du-code)
- [Implémentation des algorithmes](#implémentation-des-algorithmes)
	- [MD5](#md5-1)
	- [SHA‑256](#sha256)
	- [SHA‑512 (bonus)](#sha512-bonus)
- [Registre des algorithmes \& extensibilité](#registre-des-algorithmes--extensibilité)
- [Gestion des erreurs et codes de retour](#gestion-des-erreurs-et-codes-de-retour)
- [Tests de conformité \& vérification](#tests-de-conformité--vérification)
- [Performances et limites](#performances-et-limites)
- [Arborescence du projet](#arborescence-du-projet)
- [Liens utiles](#liens-utiles)
	- [Liens utiles pour MD5](#liens-utiles-pour-md5)
	- [Liens utiles pour SHA-256/SHA-512](#liens-utiles-pour-sha-256sha-512)
- [Grade](#grade)


<br>

## Éxigences

- Le nom de l'éxécutable doit être `ft_ssl`  
- Implémenter les algorithmes de hachage MD5 et SHA-256 en C  
- Makefile avec règles `all`, `clean`, `fclean` et `re`  
- Error handling pour les entrées invalides  
- Fonctions autorisées : `malloc`, `free`, `write`, `read`, `open`, `close`, ( `exit`, `strerror` )  

### MD5

Le programme doit inclure la commande `md5` avec les options suivantes :

- `-p` : Affiche l'entrée standard (`STDIN`) sur la sortie standard (`STDOUT`) et ajoute le hash `MD5` à la fin.
- `-q` : Mode silencieux, affiche uniquement le hash.
- `-r` : Inverse le format de sortie (hash suivi du nom de fichier ou de la chaîne).
- `-s` : Calcule le hash de la chaîne donnée en argument.

> Le comportement doit émuler celui de l'exécutable `md5sum`, mais les options sont personnalisées pour ce projet et peuvent différer de celles du programme original.  
> Si aucune option n'est fournie, le programme lit depuis la console et affiche le résultat sur la sortie standard.  

### SHA-256

Le programme doit inclure la commande `sha256`, avec les options suivantes :

- `-p` : Affiche l'entrée standard (`STDIN`) sur la sortie standard (`STDOUT`) et ajoute le hash SHA-256 à la fin.
- `-q` : Mode silencieux, affiche uniquement le hash.
- `-r` : Inverse le format de sortie (hash suivi du nom de fichier ou de la chaîne).
- `-s` : Calcule le hash de la chaîne donnée en argument.

> Le comportement doit être identique à celui de la commande MD5 ci-dessus, la seule différence étant l'algorithme de hachage utilisé.  

<br>

## Compilation et exécution

### Lancer la VM avec Docker

```bash
git clone https://github.com/aceyzz/ft_ssl_md5.git
cd ft_ssl_md5/project
docker compose up --build
```

### Accéder à la VM

```bash
docker exec -it ft_ssl bash
```

### (facultatif) Installer [OhMyBash](https://ohmybash.github.io/)

```bash
bash -c "$(curl -fsSL https://raw.githubusercontent.com/ohmybash/oh-my-bash/master/tools/install.sh)"
```

### Compiler le projet

```bash
make
```

### Exécuter le programme

```bash
./ft_ssl <command> [options] [arguments]
```

<br>

## Aperçu rapide

`ft_ssl` calcule des empreintes cryptographiques en flux. Les entrées viennent de fichiers, de chaînes (`-s`) ou de `stdin` (implicite ou via `-p`). Les données sont découpées en blocs fixes, compressées par l’algorithme choisi, puis le digest final est formaté en hexadécimal et imprimé selon les drapeaux (`-p`, `-q`, `-r`).

Algorithmes inclus : **MD5**, **SHA-256**, **SHA-512**. L’ajout d’un nouvel algorithme se fait grâce à un registre de fonctions.

<br>

## Flux de données

1. **Parsing CLI** (`parse_cli`) convertit les arguments en **tâches** (`t_task`) :  
   - `IN_FILE` avec `label` = chemin.  
   - `IN_STRING` avec `label` = la chaîne.  
   - `IN_STDIN` avec tampon lu une seule fois si nécessaire.  
2. **Validation** (`ensure_task_data`) vérifie l’existence des fichiers et la présence des données pour `stdin`/`string`.  
3. **Exécution** (`run_tasks`) applique `hash_and_print` séquentiellement sur la liste chaînée.  
4. **Hash** :  
   - Fichier : `api->hash_fd_hex(fd)` lit en blocs et met à jour le contexte.  
   - Données en mémoire : `api->hash_hex(data, len)`.  
5. **Formatage** (`format.c`) choisit la ligne à imprimer selon `-p`, `-q`, `-r`, le type d’entrée et l’éventuelle provenance `-p` (`from_p`).

## Récap CLI & exemples

Commandes : `md5`, `sha256`, `sha512`.  
Drapeaux : `-p`, `-q`, `-r`, `-s <str>` ou `-s<str>`.

Exemples simples :
```bash
# Chaîne
./ft_ssl md5 -s "hello"
./ft_ssl sha256 -s"hello"

# Fichier
./ft_ssl sha512 nom_du_fichier

# Stdin implicite
echo -n "abc" | ./ft_ssl md5
```

Chainage `-p` :
```bash
# Ré-imprime stdin puis affiche le hash
echo -n "data" | ./ft_ssl sha256 -p
```

Mode silencieux :
```bash
./ft_ssl md5 -q -s "quiet"
```

Sortie inversée :
```bash
./ft_ssl sha256 -r -s "abc"
./ft_ssl sha256 -r ./file.bin
```

Plusieurs tâches :
```bash
./ft_ssl md5 -s "one" -s "two" ./file1 ./file2
```

<br>

## Formats de sortie détaillés

- **Par défaut** : `NAME(input)= <hex>` pour fichiers et chaînes. `(stdin)= <hex>` pour `stdin`.  
- **`-q`** : affiche uniquement `<hex>`.  
- **`-r`** : `<hex> <suffix>` où `suffix` est le nom de fichier ou `"chaine"`.  
- **`-p`** : réimprime `stdin` tel quel avant le hash. Si combiné avec `-q`, la donnée réimprimée puis le digest sont chacun sur leur ligne.  
- **`-s`** : accepte `-s"abc"` ou `-s "abc"`. La chaîne n’est pas interprétée (pas d’échappement spécial).  
- Les lignes pour `stdin` issu de `-p` affichent la donnée entre guillemets puis l’empreinte si `-q` n’est pas actif.

<br>

## Architecture du code

- **`sources/main.c`** : point d’entrée, orchestration globale.  
- **`sources/digest.c`** : registre des algorithmes (`g_algos`), recherche par commande, impression de l’usage.  
- **`sources/parse_run/parse.c`** : parsing des flags courts, construction ordonnée des tâches, cache `stdin` unique.  
- **`sources/parse_run/run.c`** : validation des tâches, boucle d’exécution.  
- **`sources/utils/format.c`** : toutes les variantes d’affichage, neutres vis‑à‑vis de l’algorithme.  
- **`sources/utils/io.c`** : lecture incrémentale de `fd`, `stdin`, fichiers.  
- **`sources/utils/utils.c`** : conversion binaire → hex.  
- **`sources/utils/log.c`** : logs colorés et sorties d’erreur.  
- **`sources/utils/tasks.c`** : liste chaînée de tâches, duplication défensive des données.  
- **`sources/algo/*.c`** : implémentations MD5, SHA‑256, SHA‑512 pures C, sans dépendances externes.

<br>

## Implémentation des algorithmes

### MD5

- Contexte : quatre registres 32‑bit `a,b,c,d`.  
- Bloc : 512 bits, ordonnancement **little‑endian**.  
- 64 tours, fonctions booléennes F/G/H/I, table T et décalages `S`.  
- Padding : `0x80`, zéros, longueur sur 64 bits **little‑endian**.  
- Sortie : 128 bits, encodés en hex (ordre MD5).

### SHA‑256

- Contexte : huit mots 32‑bit `h[0..7]`.  
- Bloc : 512 bits, **big‑endian**, extension de message `w[0..63]`.  
- 64 tours, Σ/σ, `Ch`, `Maj`, constantes `K[64]`.  
- Padding : `0x80`, zéros, longueur sur 64 bits **big‑endian**.  
- Sortie : 256 bits, hex big‑endian.

### SHA‑512 (bonus)

- Contexte : huit mots 64‑bit `h[0..7]`.  
- Bloc : 1024 bits, **big‑endian**, extension `w[0..79]`.  
- 80 tours, Σ/σ 64‑bit, `Ch`, `Maj`, `K[80]`.  
- Compteur 128‑bit (high/low) pour la longueur.  
- Sortie : 512 bits, hex big‑endian.

<br>

## Registre des algorithmes & extensibilité

Aucun `if/else` long : `g_algos` contient `cmd`, `name`, taille de sortie, et deux pointeurs de fonctions (`hash_hex`, `hash_fd_hex`).  
Pour ajouter un algorithme :
1. Créer `includes/shaX.h` et `sources/algo/shaX.c` avec `*_hex` et `*_fd_hex`.  
2. Ajouter une entrée dans `g_algos` (fichier `sources/digest.c`).  
3. Recompiler. Aucune autre modification nécessaire.

<br>

## Gestion des erreurs et codes de retour

- Fichier inexistant : message `ft_ssl: <path>: No such file or directory`.  
- Échec hash/lecture : `ft_ssl: hash failed`.  
- Option inconnue : `ft_ssl: illegal option -- <x>`.  
- `-s` sans argument : `ft_ssl: option requires an argument -- s`.  
- Le programme renvoie `EXIT_FAILURE` si l’exécution des tâches échoue, sinon `0`.

<br>

## Tests de conformité & vérification

Comparaison simple avec OpenSSL :
```bash
# Chaîne
echo -n "abc" | ./ft_ssl sha256    ; echo -n "abc" | openssl sha256
echo -n "abc" | ./ft_ssl sha512    ; echo -n "abc" | openssl sha512
echo -n "abc" | ./ft_ssl md5       ; echo -n "abc" | md5

# Fichier
./ft_ssl sha256 file.bin            ; openssl sha256 file.bin
./ft_ssl sha512 file.bin            ; openssl sha512 file.bin
./ft_ssl md5 file.bin               ; md5 file.bin
```

<br>

## Performances et limites

- Lecture par blocs (`4096` octets) côté I/O, puis blocs internes (`64` ou `128` octets) côté algorithme.  
- Mémoire constante en taille des contextes. Aucune dépendance externe.  
- Les entrées très grandes sont supportées en flux. SHA‑512 maintient un compteur 128‑bit pour la longueur.  
- Encodages des longueurs respectent les spécifications endian pour chaque algorithme.

<br>

## Arborescence du projet

```bash
includes/
  ft_ssl.h  md5.h  sha256.h  sha512.h  colors.h
  libft/
    libft.h
sources/
  main.c
  digest.c
  parse_run/
	parse.c  run.c
  utils/
	format.c  io.c  log.c  tasks.c  utils.c
  algo/
	md5.c  sha256.c  sha512.c
project/
  (docker, env de build)
```

<br>

## Liens utiles

- [Principe du hashing en vidéo](https://www.youtube.com/watch?v=QZY3IjFBtFY)  

### Liens utiles pour MD5

- [Page Wikipédia sur MD5](https://fr.wikipedia.org/wiki/MD5)  
- [Spécification de l'algorithme MD5](https://tools.ietf.org/html/rfc1321)  
- [Excellente vidéo sur MD5](https://www.youtube.com/watch?v=5MiMK45gkTY&t=53s&pp=ygUNbWQ1IGFsZ29yaXRobQ%3D%3D)  

### Liens utiles pour SHA-256/SHA-512

- [Page Wikipédia sur SHA-256](https://fr.wikipedia.org/wiki/SHA-2#SHA-256)  
- [Excellente vidéo sur SHA-256](https://www.youtube.com/watch?v=orIgy2MjqrA)  
- [Pseudocode pour SHA-256](https://stackoverflow.com/questions/11937192/sha-256-pseuedocode)  

<br>

## Grade

> En cours d'évaluation
