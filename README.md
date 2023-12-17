# Matrix Project - Snake Game 🐍

<details>
  <summary><h2><b>TASK REQUIREMENTS 📁</b></h2></summary>
  <h2>Menu task</h2>
     Create a menu for your game, emphasis on ‘the game. You should scroll on the LCD with the joystick. The menu should include the following functionality:

  **1. Intro Message** - When  powering  up  the  project,  a  greeting  message should be shown for a few moments.
  
  **2. Menu:**
  
    **(a)Start game** - Starts the initial level
    
    **(b)Settings**
    
    **i.** LCD  brightness  control. Save  value  to  EEPROM  and  load  it upon system start.
        
    **ii.** Matrix brightness control
        
    **(c)About** - should include details about the creator of the game. At least game name, author and github link or user.
    
  **3.End Message** - When the game ends, add a message.
  
  **4. During gameplay**
  
    **(a)** Show relevant details, that are dynamic: time.  
    
    **(b)** Implement an end game.
</details> 

<details>
  <summary><h2><b>GAME RULES 🎮</b></h2></summary>
  The objective of the snake is to eat the blinking food 🍕. To start the game, the player has to enter the start game submenu, and then press the button. While playing, the score appears on the LCD. For now, the minigame ends when the score is 10.
  
  <h3>Bugs 🐜</h3>
  <ul>
    <li>When accessing the 'LCD bright' option, it does not work, meaning you cannot adjust the LCD brightness. </li>
    <li>You cannot mute the game sound, even though there is an on/off option in the menu.</li>
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
