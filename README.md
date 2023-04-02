## Description

<ul>
<li>Creator : BRUNE Adrien</li>
<li>Version : 1.0.1</li>
<li>Date : 01/04/2023</li>
</ul>

<img src="./SV_GAME/Src/Resources/Secret_Voldemort_Cover.png" alt="Logo" width="200" height="300">

## Game

<p>Secret Voldemort is a strategic game involving 5 up to 10 players. 
The games grants you a role that defines your behaviour during the game.
The realm of magic is threaten by the dark forces of deatheaters lead by Voldemort himself.
If you join the deatheaters, try to take the power by electing Voldemort as the Minister of magic.
But if you're the resistance well known as the Phenix order, you'll have to fight the deatheaters by taking the Minister or slaying them all.
When the game starts you are alone, you'll have to be perceptive enought to discover players of your team and rallying them to your cause.
By doing so, maybe you will be able to overcome manipulations and be able to win the game, good luck.

Official Secret Hitler rules : https://www.secrethitler.com/assets/Secret_Hitler_Rules.pdf  
Video rules : https://www.youtube.com/watch?v=mbGXIDYdtas&ab_channel=TheRulesGirl

## Preview
<img src="./Preview/Preview_1.png" alt="Preview to add" width="700" height="400">
<img src="./Preview/Preview_2.png" alt="Preview to add" width="700" height="400">
<img src="./Preview/Preview_3.png" alt="Preview to add" width="700" height="400">
<img src="./Preview/Preview_4.png" alt="Preview to add" width="700" height="400">

## Technical point
* One serveur needs to be started to play.
* Ethernet port 30000 is used to communicate.
* The server owner needs to operate a port redirection to redirect Ethernet paquet from livebox to the computer's server.
  * Go to your box configuration.
  * Got to port redirection.
  * Add a rule.
  * set TCP protocol.
  * set the range port to [30000, 30000] (single port 30000 is used).
  * set the computer IP adresse (go to Windows CMD, then tape the command line "ipconfig").
  * Then you will have to give your public IP to other player (find it on internet).
  * Be ready to play.
