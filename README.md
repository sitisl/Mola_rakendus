# Mola rakendus

TalTech Software Project course 2023 \
Authors:
* Markus Visnapuu 
* Rannar Randmäe 
* Siim Tišler


## Introduction
This is the repo of Group 15 from Software Project course at TalTech 2023. 
The goal of the project was to create a messaging app in C that has a GUI and support for profile pictures and sending text, pictures and emojis between multiple clients.
This repo is only for more convenient code management and project is intended only as a study project.

## Installation
To run the messaging applications all files from FormLogin have to be downloaded and executed as a unit.
For the communication to work between clients a server application has to be running as well, for that all files form Socket have to be downloaded and executed as a unit.
As of now the server is limited to six clients, but it can be changed in initServ.h file.

## Dependencies
The min Qt version used for creating this project was 6.4.3_msvc2019_64
Atleast Qt 6.4.3 needs to be installed with MSVC 2019 64-bit. Qt online installer can be downloaded from this site
https://www.qt.io/download-open-source

Visual Studio 2019/2022 IDE is used for running and developing. Qt VS Tools extension is needed.

Go to Extensions > Qt VS Tools > Qt versions. Change the version to your downloaded version.

Also in Visual Studio FormsLogin project settings change your Qt version installer to the same version.

## Technologies Used
* C and C++ programming languages
* Qt framework for GUI
* Visual Studio and Qt Creator as an integrated development environment (IDE)
* Winsock2 for networking and TCP communication
* Git for version control

## Screenshots of UI
### Profile avatar selection
![image](https://github.com/sitisl/Mola_rakendus/assets/92330937/60f23961-4bd5-4e9f-b168-1263d76fbc16)
### Chatting, using emojis, sending pictures
![image](https://github.com/sitisl/Mola_rakendus/assets/92330937/a504e147-e768-46c4-a6e0-56ac428467f2)
