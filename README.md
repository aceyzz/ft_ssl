<img title="42_ft_ssl_md5" alt="42_ft_ssl_md5" src="./utils/banner.png" width="100%">

<br>

# `ft_ssl_md5` — 42

Ce projet a pour but d'implémenter un programme en C qui chiffre des données en utilisant l'algorithme MD5 et SHA-256. Le programme doit être capable de lire des données à partir de fichiers ou de l'entrée standard, et de produire des sorties formatées selon les spécifications.  

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

Vous devez créer la commande `sha256` pour votre programme `ft_ssl`, avec les options suivantes :

- `-p` : Affiche l'entrée standard (`STDIN`) sur la sortie standard (`STDOUT`) et ajoute le hash SHA-256 à la fin.
- `-q` : Mode silencieux, affiche uniquement le hash.
- `-r` : Inverse le format de sortie (hash suivi du nom de fichier ou de la chaîne).
- `-s` : Calcule le hash de la chaîne donnée en argument.

> Les options demandées ici sont personnalisées et peuvent fonctionner différemment du programme original, ce qui est normal.  
> Le comportement doit être identique à celui de la commande MD5 ci-dessus, la seule différence étant l'algorithme de hachage utilisé.  


<br>