### Imperator to Rome2

Allows you to play Imperator: Rome battles in Total War Rome: 2.

#### Progress

- [x] Simple launcher that launches imperator, watches logs, and responds to certain events printed to the logs.
- [x] Export basic battle stats from the imperator logs
- [x] Parse battle stats into the program
- [ ] Generate a Rome 2 mod from the parsed stats
- [ ] Launch Rome 2 with the generated mod
- [ ] Export the results of the Rome 2 battle to logs
- [ ] Parse battle results into program
- [ ] Update imperator game state with results of battle

#### Requirements

Imperator: Rome and Rome 2 installed on steam

#### Compiling
on linux:
`g++ src/main.cpp -o ImperatorToRome2`
windows:
`g++ src/main.cpp -o ImperatorToRome2.exe`


#### Usage

Install the TotalWarBattle Imperator: Rome mod

Open up the default Imperator: Rome launcher and set the TotalWarBattle as active in the playset then close the launcher.

Make sure you have steam open and then run the executable, it will launch imperator rome.
`./ImperatorToRome2`

While playing imperator when you enter a battle there will be a button in the combat interface that will generate a Rome 2 mod from the ongoing battle and then launch rome 2 and allow you to play the battle. Once the battle is over the results will be exported into your imperator save where you'll be able to continue your game.
