# Serveur SMTP en C

Sommaire

-   [1 **Présentation du projet**](#Pr.C3.A9sentation_du_projet)
    -   [1.1 Contexte](#Contexte)
    -   [1.2 Objectif](#Objectif)
    -   [1.3 Description du projet](#Description_du_projet)
    -   [1.4 Validation de l'épreuve](#Validation_de_l.27.C3.A9preuve)
    -   [1.5 Matériel nécessaire](#Mat.C3.A9riel_n.C3.A9cessaire)
    -   [1.6 Planning prévisionnel](#Planning_pr.C3.A9visionnel)
        -   [1.6.1 Introduction (semaines 1&2)](#Introduction_.28semaines_1.262.29)
        -   [1.6.2 Gestion des fichiers (semaine 3)](#Gestion_des_fichiers_.28semaine_3.29)
        -   [1.6.3 Envoi des messages (semaine 4)](#Envoi_des_messages_.28semaine_4.29)
        -   [1.6.4 Aspects multithread (semaine 5)](#Aspects_multithread_.28semaine_5.29)
        -   [1.6.5 Finalisation et Améliorations (semaines 6&7)](#Finalisation_et_Am.C3.A9liorations_.28semaines_6.267.29)

<br/>


-   [2 **Travail effectué**](#Travail_effectu.C3.A9)
    -   [2.1 Période 1 (du 11/07 au 3/07)](#P.C3.A9riode_1_.28du_11.2F07_au_23.2F07.29)
    -   [2.2 Période 2 (du 24/07 au 25/07)](#P.C3.A9riode_2_.28du_24.2F07_au_25.2F07.29)
    -   [2.3 Période 3 (du 25/07 au 31/07)](#P.C3.A9riode_3_.28du_25.2F07_au_31.2F07.29)
    -   [2.4 Période 4 (du 01/08 au 08/08)](#P.C3.A9riode_4_.28du_01.2F08_au_08.2F08.29)
    -   [2.5 Période 5 (dernière période)](#P.C3.A9riode_5_.28derni.C3.A8re_p.C3.A9riode.29)

<br/>


-   [3 **Utilisation du code**](#Travail_effectu.C3.A9)
    -   [2.1 Fonctionnement](#Fonctionnement)
    -   [2.2 Paramétrage client Thunderbird](#Param.C3.A9trage_client_Thunderbird)
    -   [2.3 Test du code](#Test_du_code)

<br/>


-   [4 **Bilan**](#Travail_effectu.C3.A9)
    -   [2.1 Réalisations](#Réalisation)
    -   [2.2 Aspects manquants](#Param.C3.A9trage_client_Thunderbird)
    -   [2.3 Environnement technique](#Test_du_code)



## Présentation du projet
----------------------

### Contexte

La validation du semestre S8 dépend de la validation de cette épreuve.

### Objectif

Le but est de développer un système de messagerie personnelle très
léger. Vous développerez plus particulièrement un serveur SMTP
fonctionnant à la fois en IPv4 et en IPv6.

### Description du projet

Vous développerez ce serveur SMTP sur le même modèle que le projet système réseau, en particulier concernant l'aspect structuré et multi-threadé.

Quelques directives spécifiques au serveur SMTP :

-   Les messages sont stockés dans un répertoire. On trouve dans ce  répertoire un sous-répertoire par utilisateur. Les messages sont  stockés sous la forme d'un fichier de nom numérique représentant l'ordre de réception des messages. Un fichier d'extension `.status` est associé à chaque message pour indiquer les états de ce message    (voir l'épreuve complémentaire sur le serveur IMAP pour trouver  l'état à préciser pour les messages nouvellement arrivés). Il faut  prendre soin de gérer les messages arrivant simultanément en  utilisant la fonction système `lockf` sur le répertoirecorrespondant à la boite aux lettres.
-   Le serveur doit gérer deux sockets d'écoute. Une limitée à l'interface loopback et l'autre à l'interface permettant la connexion au réseau. Les deux sockets seront gérées par deux threads  différents (utilisation de la fonction `lanceThread`). Seuls les clients connectés sur la première socket pourront envoyer des messages en externe. Les clients connectés à la seconde socket ne pourront qu'envoyer des messages locaux à la machine.
-   Pour préciser le répertoire de stockage des messages (`/var/spool/mail/` par défaut) et les suffixes des adresses locales utilisez des options courtes et longues (fonction `getopt_long`). L'option pour le suffixe peut être utilisée plusieurs fois (serveur local gérant plusieurs noms de domaines).
-   Il vous est rappelé que pour distribuer un courriel, il faut trouver l'adresse du serveur SMTP du correspondant. Cela peut être fait en utilisant la fonction système `res_search` et ses fonctions annexes.
-   Vous devez implanter les commandes SMTP suivantes : `EHLO`, `MAIL`, `RCPT`, `DATA` et `EXIT`. N'oubliez pas de retourner le code  d'erreur ad hoc pour toute commande non implantée. Il est fortement conseillé de lire la [RFC
    5321](https://tools.ietf.org/html/rfc5321).

### Validation de l'épreuve

L'épreuve est validée si les sources sont lisibles et si le serveur supporte un test. Le test en question consistera à lancer le serveur sur une machine accessible d'Internet et à faire plusieurs essais d'envoi de messages. Un client de messagerie comme `thunderbird` sera configuré avec votre serveur STMP comme serveur d'envoi. Des messages à diverses adresses de courriels seront envoyés. Des messages seront aussi envoyés d'ailleurs sur Internet à destination de la machine sur laquelle se trouve votre serveur. Il sera vérifié que les messages sont bien sauvés
sur le disque dans le format demandé.

### Matériel nécessaire

Aucun.

### Planning prévisionnel

Attention: planning susceptible d'évoluer

#### Introduction (semaines 1&2)

-   documentation et compréhension du cahier des charges
-   squelette du fonctionnement du serveur, avec gestion minimale des
    commandes `EHLO`, `MAIL`, `RCPT`, `DATA` et `QUIT`
-   configuration d'un client sur `thunderbird`

#### Gestion des fichiers (semaine 3)

-   liste des différents expéditeurs
-   stockage des messages ainsi que leur statut avec le format spécifié
-   gestion d'arrivée simultanée de messages avec `lockf`

#### Envoi des messages (semaine 4)

-   résolution d'adresse avec `res_search` pour la distribution des
    courriels
-   gestion des codes d'erreur
-   gestion de plusieurs destinataires
-   gestion des messages provenant de serveurs différents

#### Aspects multithread (semaine 5)

-   gestion des sockets d'écoute pour les 2 interfaces (loopback et
    réseau)
-   terminaison des threads

#### Finalisation et Améliorations (semaines 6&7)

-   modularité des codes
-   constantes, variables globales et arguments
-   résultats des tests finaux

## Travail effectué
--------------------

### Période 1 (du 11/07 au 23/07)

Durant cette période, j'ai tout d'abord commencé par me documenter sur
le protocole SMTP ainsi que sur la `RFC` (toujours en cours). Ce qui me
permet de définir un planning prévisionnel sans date butoir que j'aurai
certainement à ajuster selon mon avancement et les priorités. Par la
suite, en gardant l'architecture des sockets utilisée pour le projet
PSR, j'ai pu établir une squelette de mon serveur smtp pour la gestion
des mails envoyés. Ainsi, en configurant un client sur `Thunderbird`
avec localhost pour serveur local, j'arrive à me connecter et à afficher
les messages envoyés par les clients et à gérer les réponses qu'il faut
fournir.

Les réponses ainsi que les traitements liés sont dans des fonctions
permettant de gérer les commandes smtp de base à savoir: `EHLO`, `MAIL`,
`RCPT`, `DATA` et `QUIT`.


 Etant donné qu'il s'agit du début, il manque certains éléments à gérer
dans le code, notamment:

-   fonctionnement du serveur en IPv6
-   résolution d'adresse pour l'envoi des mails
-   gestion du port "loopback"
-   gestion des codes d'erreur
-   gestion de la terminaison des threads
-   arrivée simultanée de messages
-   gestion de plusieurs destinataires
-   SAUVEGARDE DES MESSAGES par répertoire pour chaque utilisateur



### Période 2 (du 24/07 au 25/07)

Amélioration de la version 1. La configuration thunderbird se fait comme
suit:

-   Unix Mailspool, Movemail
-   compte: mseck@localhost
-   serveur: localhost

Par ailleurs, des tests ont été réalisés et les mails sont bien considérés comme envoyés et sauvegardés via thunderbird (voir photo)
[Fichier:Capture
requetes](/mediawiki//index.php?title=Sp%C3%A9cial:T%C3%A9l%C3%A9verser&wpDestFile=Capture_requetes "Fichier:Capture requetes")



### Période 3 (du 25/07 au 31/07)

Version actuelle (version 3) Serveur SMTP fonctionnant en IPv4 et IPv6
Ajout du stockage des messages envoyés:

-   Fichier listant tous les utilisateurs, et rajoutant les nouveaux
-   Dossier pour chaque utilisateur avec les fichiers de noms numériques   contenants les messages par ordre d'arrivée

Par ailleurs, j'ai tenté d'utiliser la fonction système `res_search` pour la résolution d'adresse mais pour l'instant j'ai des erreurs. J'ai également un problème de descripteur de fichier lorsque j'implémente lafonction `lockf`, mais cela sera très prochainement résolu.
Concernant cette partie, il me reste à tenir des fichiers contenant le
statut des messages et de renvoyer les codes d'erreurs relatives aux
commandes (après résolution d'adresse)



### Période 4 (du 01/08 au 08/08)

Version actuelle (version 4) Serveur SMTP fonctionnant en IPv4 et IPv6
mais n'écoutant que sur la socket liée à `localhost`sur le `port 4444`.
Lorsqu'un client se connecte, il entame la discussion avec le serveur
qui vérifie que l'adresse spécifiée dans la commande `MAIL` est bien un
utilisateur du réseau local. En effet, il faut saisir manuellement les
utilisateurs dans un fichier dont le chemin est le suivant
`Messages/Users.txt`. Ainsi, le message n'est envoyés que si
l'utilisateur existe, sinon un code d'erreur est envoyé (suivi d'un seg
fault). Lors de la réception de `RCPT`, on récupère le nom de domaine
pour savoir s'il faut traiter le message en interne ou se comporter
comme un `serveur relais`. Dans ce cas, on utilise la fonction système
`res_search` pour trouver l'adresse du serveur à contacter, et établir
ainsi la communication pour relayer le message. Pour chaque utilisateur,
il existe un dossier avec les fichiers de noms numériques contenants les
messages par ordre d'arrivée ainsi que des fichier .status pour l'état
des messages.

Des tests on été effectués et le code est fonctionnel pour envoyer des
mails vers les adresses de Polytech sur le port 25 par exemple.
Cependant, je suis confronté à des problèmes de sécurité sur la couche
de transport lorsque j'essaie de me connecter sur les serveurs Gmail ou
Yahoo.

NB: un seul destinataire par message



### Période 5 (dernière période)

Version actuelle (version 5) En cette fin de période, j'ai pu mettre en
place un serveur SMTP fonctionnant à la fois en IPv4 et IPv6 et se
comportant aussi comme un serveur relais.

## UTILISATION DU CODE
### Fonctionnement

-   Lorsque le client se connecte, le serveur entame la discussion avec  lui en répondant aux requêtes smtp `EHLO`, `MAIL`, `RCPT`, `DATA` et   `QUIT`
-   Le contenu du message est stocké temporairement dans un fichier  `temp_message.txt` dans le dossier Serveur et les entêtes du   messages dans une structure de données
-   Lorsque l'adresse du destinataire n'est pas locale, le serveur se   comporte comme un client pour transférer le message vers le serveur correspondant avec un `enregistrement MX` effectué grâce à la   fonction système `res_query`
-   L'enregistrement MX est parsé pour récupérer le nom du serveur mail  distant, selon la priorité
-   Utilisation de `lockf` dans la réperoire de la boite aux lettres en  cas d'arrivée simultanée de messages
-   Il existe un dossier pour chaque utilisateur avec les fichiers de  noms numériques contenants les messages par ordre d'arrivée avec  l'extension `.txt` et un fichier `.status` associé


### Paramétrage client Thunderbird

La configuration thunderbird se fait comme suit:

-   `Unix Mailspool`, Movemail
-   compte: nom\_utilisateur@localhost
-   serveur: `localhost` , `port:4444`

### Test du code

La compilation se fait avec le Makefile dans le répertoire parent. Dans le répertoire Serveur, on peut trouver le programme principal (`serveur.c`) dans que l'on exécute en mode `super utilisateur` en lui passant quelques paramètres:

-   La fonction main prend en argument obligatoire le nom de domaine correspondant au serveur. Pour cela j'ai dû donner un nom à mon adresse dans le fichier `/etc/hosts` comme suit par exemeple `192.168.236.100 localserver server.localhost.fr`
-   Le répertoire correspondant à la boite aux lettres entre côtes et  précédé de `-R` (en option), si celui-ci n'est pas spécifié, `/var/spool/mail/` sera pris par défaut
-   Les suffixes des adresses locales entre côtes et précédés de `-s`  (en option), la valeur par défaut étant `myserv.fr`

exemple:
`sudo ./serveur "192.1.1.1" -R "./BoiteAuxLettres/" -s "localhost.com" -s "localhost.fr"`
dans le répertoire Serveur

## Bilan

Ce projet m'a permis, entre autres, d'approfondir mes connaissances en programmation système et réseaux et de découvrir par l'expérimentation le fonctionnement des serveurs mail. J'ai pu ainsi comprendre les dessous de l'envoi d'un mail, avec les principales étapes nécessaires.


### Réalisations

* Création d’un serveur TCP fonctionnant sous IPv4 et IPv6 en C 
* Mise en place d’une double socket (loopback et réseau)
* Gestion des codes d'erreur
* Gestion des enregistrements MX
* Résolution d'adresse
* Communication avec serveurs distants (Gmail, Yahoo, ...) 
* Gestion des files system
* Tests via Thunderbird


### Aspects manquants
*   lockf sur le répertoire ne marche pas (bad file descriptor)

### Environnement technique 
Langage C, TCP/IP, Multithreading, Makefile, Thunderbird, RFC 5321
