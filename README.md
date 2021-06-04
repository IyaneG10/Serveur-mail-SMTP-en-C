# Serveur SMTP

## Description du projet

Développement d'un système de messagerie personnelle très léger. Plus particulièrement un serveur SMTP fonctionnant à la fois en IPv4 et en IPv6.

## Réalisations

* Création d’un serveur TCP fonctionnant sous IPv4 et IPv6 en C
* Mise en place d’une double socket (loopback et réseau)
* Gestion des codes d'erreur
* Gestion des enregistrements MX
* Résolution d'adresse
* Communication avec serveurs distants (Gmail, Yahoo, ...)
* Gestion des files system
* Tests via Thunderbird

## Environnement technique :

Langage C, TCP/IP, Multithreading, Makefile, Thunderbird, RFC 5321
