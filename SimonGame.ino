const int speakerPin = 11; // Pin connected to the speaker

//*****************************************

int noteDuration = 400; // Duration to play the main notes

int noteFrequencies[4] = {100, 350, 600, 850}; // Frequencies for the 4 notes used in the game

int gameSequence[20]; // Array to store game pattern during play

int gameDifficulty = 1;

//*****************************************

void setup() {
  pinMode(2, OUTPUT); // Set up LED pins
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, INPUT); // Set up button pins
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  
  Serial.begin(9600); // Enable serial output for debugging
}

void loop() {
  initializePins(); // Set the LED and button pins HIGH
  displayMainMenu();
}

void displayMainMenu() {
  while (true) {
    if (digitalRead(6) == LOW) {
      gameDifficulty = 1;
      generateGameSequence();
      playGame();
    }
    if (digitalRead(7) == LOW) {
      gameDifficulty = 2;
      generateGameSequence();
      playGame();
    }
    if (digitalRead(8) == LOW) {
      gameDifficulty = 3;
      generateGameSequence();
      playGame();
    }
    if (digitalRead(9) == LOW) {
      gameDifficulty = 4;
      generateGameSequence();
      playGame();
    }
  }
}

void initializePins() {
  for (int pin = 2; pin < 10; pin++) {
    digitalWrite(pin, HIGH);
  }
}

void playGame() {
  int roundNumber = 0;
  int playerTurn = 1;
  bool buttonPressed = false;
  int currentNote;
  int playerInput = 0;
  bool gameLost = false;

  playNote(1, 100);
  playNote(2, 100);
  playNote(3, 100);
  playNote(4, 100);
  delay(1000);

  for (int currentRound = 1; currentRound <= (gameDifficulty * 5); currentRound++) {
    roundNumber++;
    playerTurn = 1;
    buttonPressed = false;
    playerInput = 0;

    for (int j = 1; j < currentRound; j++) {
      playNote(gameSequence[j - 1], noteDuration); // Play the current round note sequence
    }

    while (playerTurn < currentRound) {
      currentNote = gameSequence[playerTurn - 1];
      while (!buttonPressed) {
        if (digitalRead(6) == LOW) { // Button 1 Pressed
          buttonPressed = true;
          playerInput = 1;
        }
        if (digitalRead(7) == LOW) { // Button 2 Pressed
          buttonPressed = true;
          playerInput = 2;
        }
        if (digitalRead(8) == LOW) { // Button 3 Pressed
          buttonPressed = true;
          playerInput = 3;
        }
        if (digitalRead(9) == LOW) { // Button 4 Pressed
          buttonPressed = true;
          playerInput = 4;
        }
        if (buttonPressed) { // A button was pressed
          playNote(playerInput, noteDuration); // Play the pressed note
          if (currentNote == playerInput) { // Correct note
            playerTurn++;
          } else { // Incorrect note
            endGame(false);
          }
        }
      }
      buttonPressed = false;
    }
  }

  if (!gameLost) {
    endGame(true);
  }
}

void generateGameSequence() {
  randomSeed(analogRead(1));
  for (int i = 0; i < (gameDifficulty * 5); i++) { // Add more turns to the game based on difficulty
    gameSequence[i] = random(1, 5);
  }
}

void playNote(int noteIndex, int duration) {
  digitalWrite(noteIndex + 1, LOW);
  tone(speakerPin, noteFrequencies[noteIndex - 1], duration);
  delay(duration * 2);
  digitalWrite(noteIndex + 1, HIGH);
}

void endGame(bool win) {
  if (win) {
    Serial.println("You Win!");
    for (int i = 0; i < 10; i++) {
      playNote(1, 50);
      playNote(2, 50);
      playNote(3, 50);
      playNote(4, 50);
    }
  } else {
    Serial.println("You Lose!");
    playNote(4, 300);
    playNote(3, 300);
    playNote(2, 300);
    playNote(1, 300);
    playNote(4, 600);
  }
  Serial.println("Game over");
  displayMainMenu();
}
