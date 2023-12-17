# Matrix Project - Snake Game 🐍

<details>
  <summary><h2><b>TASK REQUIREMENTS 📁</b></h2></summary>
  <h3>Menu task</h3>
     Create a menu for your game, emphasis on ‘the game. You should scroll on the LCD with the joystick. The menu should include the following functionality:

  **1. Intro Message** - When  powering  up  the  project,  a  greeting  message should be shown for a few moments.
  
  **2. Should contain roughly the following categories:**
  
  **(a)Start game** - Starts the initial level
    
  **(b)Settings**
    
    - LCD  brightness  control. Save it to eeprom;
        
    - Matrix brightness control. Make sure to display something on the matrix when selecting it. Save it to eeprom.
        
   **(c)About** - Should include details about the creator of the game. At least game name, author and github link or user.
    
  **3. While playing the game** - Display all relevant info (lives, levels, score etc.)
  
  **4. Upon game ending** - Display relevant game info: score, time, lives left etc. Must inform player if he/she beat the highscore. This menu should only be closed by the player, pressing a button.

  <h2>Game requirements</h2>
    – Minimal components: an LCD, a joystick, a buzzer and the led matrix;
    
    – You must add basic sounds to the game;
    
    – It must be intuitive and fun to play;
    
    – It must make sense in the current setup;
    
    - It can be a classic snake game, basically where the length of the snake increases in time;
    
    – The ”food” should blink, so as to separate it from the rest of the snake.
</details> 

<details>
  <summary><h3><b>GAME RULES 🎮</b></h3></summary>
  <h3>Game info 💬</h3>
  The objective of the snake is to eat the blinking food 🍕. To start the game, the player has to enter the start game submenu, and then press the button. While playing, the score appears on the LCD. For now, the minigame ends when the score is 10.
  
  <h3>Bugs 🐜</h3>
  <ul>
    <li>The snake does not grow when it eats the blinking food.</li>
    <li>When accessing the 'LCD bright' option, it does not work, meaning you cannot adjust the LCD brightness. </li>
    <li>You cannot mute the game sound, even though there is an on/off option in the menu.</li>
    <li>At the end of the game, the score is displayed for 5 seconds, and it cannot be closed by the player pressing the button.</li>
  </ul>
</details> 

<details>
  <summary><h2><b>THE MENU 📒</b></h2></summary>
  
  You can move through the menu using the joystick up and down. To enter an option move the joystick to the right and to exit an option move the joystick to the left. Same for the submenu. While scrolling through the menu, a visual representation of the current item is displayed on the matrix.
  
  <h3>Main meniu 📋</h3>
    <ul>
      <li>Start game</li>
      <li>Settings</li>
      <li>How to play</li>
      <li>About</li>
    </ul> 
    
  <h3>Settings submenu 🔧</h3>
    <ul>
      <li>LCD bright</li>
      <li>Matrix bright</li>
      <li>Sounds</li>
    </ul> 
</details> 

<details>
  <summary><h2><b>HARDWEAR COMPONENTS 🔌</b></h2></summary>
<ol>
  <li>16x2 LCD</li>
  <li>1 8x8 led matrix</li>
  <li>MAX7219 driver</li>
  <li>joystick</li>
  <li>buzzer</li>
  <li>potentiometer (for controlling the LCD contrast)
</ol>  
</details> 

## Photo of my setup 📸

<img src="https://github.com/RuxiC/Matrix_Project_Snake_Game/blob/main/Matrix_Project/matrix_game.jpeg?raw=true" alt="A photo of my setup" width="500" height="350">
