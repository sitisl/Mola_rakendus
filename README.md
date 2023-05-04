# Mola rakendus

TalTech Tarkvara Projekt course 2023 \
Authors:
* Markus Visnapuu 
* Rannar Randmäe 
* Siim Tišler


## Introduction
This is the repo of Group 15 from Software Project course at TalTech 2023. 
The goal of the project was to create a messaging app in C that has a GUI and support for profile pictures and sending text, pictures and emojis between multiple clients.
This repo is only for more convenient code management and project is intended olny as a study project.

## Installation
To run the messaging applications all files from FormLogin have to be downloaded and executed as a unit.
For the communication to work between clients a server application has to be running as well, for that all files form Socket have to be downloaded and executed as a unit.
As of now the server is limited to six clients, but it can be changed in initServ.h file.

## Dependencies
The min Qt version used for creating this project was 6.2.4_msvc2019_64
Atleast Qt 6.2.4 needs to be installed with MSVC 2019 64-bit. Qt online installer can be downloaded from this site
https://www.qt.io/download-open-source

Visual Studio 2019/2022 IDE is used for running and developing. Qt VS Tools extension is needed.

Go to Extensions > Qt VS Tools > Qt versions. Change the version to your downloaded version
Also in Visual Studio FormsLogin project settings change your Qt version installer to the same version.

## Technologies Used
* C and C++ programming languages
* Qt framework for GUI
* Visual Studio and Qt Creator as an integrated development environment (IDE)
* Winsock2 for networking and TCP communication
* Git for version control
