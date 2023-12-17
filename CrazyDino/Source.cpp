#include <SFML/Graphics.hpp> // Graphics Library
#include <iostream> // Input-Output Library
#include <cstdlib> // General Library -> Random Generator + Memory Management
#include <random> // Random Number Generator
#include <ctime> // Clock + Timer 
#include <sstream> // Strings As Streams -> Use As Either Input Or Output

using namespace std;
using namespace sf;

// Game Starting Function

void startFunction(RenderWindow &window) {
   // Set Initial Position And Velocity
    Vector2f position(100, 100); // -> 2D-Vector
    Vector2f velocity(0, 0);

    // Dino
    Texture texture1; // -> Imaging
    texture1.loadFromFile("dino.png"); // Path Of Image On Pc
    Sprite dino(texture1); // -> Enhance UI
    dino.setPosition(100, window.getSize().y - 100); // Position On Screen
    dino.setScale(0.17, 0.14); // Adjust Size Of Image

    // Creating Obstacles
    Texture texture2;
    texture2.loadFromFile("cactus1.jpg");
    Sprite cactus1(texture2);
    cactus1.setScale(.12, .1);
    cactus1.setPosition(window.getSize().x + 20, window.getSize().y - 80);
	// Initial Position Random y Of Obstacle, x Always Gets Size

    Texture texture3;
    texture3.loadFromFile("cactus1.jpg");
    Sprite cactus2(texture3);
    cactus2.setScale(.2, .1);
    cactus2.setPosition(window.getSize().x + 20, window.getSize().y - 80);
	// Initial Position Random y Of Obstacle, x Always Gets Size

    Texture texture4;
    texture4.loadFromFile("bird.png");
    Sprite bird1(texture4);
    bird1.setScale(.12, .1);
    bird1.setPosition(window.getSize().x + 20, window.getSize().y - 180);
	// Initial Position Random y Of Obstacle, x Always Gets Size

    bool birdspawned = false; // Check Wheather The Bird Is Spawned Or Not

    float gravity = .03; // Set Gravity
    float movespeed = .5; // Set Movement Speed

	// Time Manipulation
    Clock timer;
    Clock clock1;
    Clock clock2;
    Clock clock3;

    // For Random Number
    random_device rd;
    mt19937 random(rd());
    uniform_int_distribution<int> gen(1, 10); // Gives a random number from 1 to 10
    int rndnum;

    // For score
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return; // Handle Errors Involving Font
    }

	// Format To Disply Score On Screen
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setCharacterSize(30);
    scoretext.setColor(Color::Black);
    scoretext.setStyle(Text::Bold);
    scoretext.setPosition(window.getSize().x / 200 * 80, window.getSize().y / 2); // Retrieve The Dimensions

    // Game loop
    while (window.isOpen()) {
        Event event;

        // For closing button on sfml window
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Movement (WAD keys)

        // Jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && dino.getPosition().y > 400) {
            velocity.y = -2.0; // Jump by setting a vertical velocity
        }
        // Left Right
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            velocity.x = -movespeed * 2; // Move left
        } else if (Keyboard::isKeyPressed(Keyboard::D)) {
            velocity.x = movespeed * 2; // Move right
        } else {
            velocity.x = 0; // Stop horizontal movement if no key is pressed
        }

        velocity.y += gravity; // Apply gravity
        position += velocity; // Update position based on velocity

        // Bottom boundary
        if (position.y + 100 >= window.getSize().y) {
            position.y = window.getSize().y - 100;
            velocity.y = 0;
        }
        // Top boundary
        if (position.y < 0) {
            position.y = 0;
            velocity.y = 0;
        }

        // Left
        if (position.x < 0) {
            position.x = 0;
        }
        // Right
        if (position.x + 20 >= window.getSize().x) {
            position.x = window.getSize().x - 20;
        }

        // Moving obstacles
        cactus1.move(-movespeed * (1 + timer.getElapsedTime().asSeconds() / 12), 0); // Speed up as time progresses
        cactus2.move(-movespeed * (1 + timer.getElapsedTime().asSeconds() / 11), 0);

        // Collision/detection
        if (dino.getGlobalBounds().intersects(cactus1.getGlobalBounds()) || dino.getGlobalBounds().intersects(cactus2.getGlobalBounds()) || dino.getGlobalBounds().intersects(bird1.getGlobalBounds())) {
            // Handle game over
	
			sleep(sf::seconds(2.0f)); // Delay for 2 seconds
			window.close();
			
        } else
            // Update rectangle position
            dino.setPosition(position);

		// Randomly Generate Obstacles

        rndnum = gen(random);
        if (clock1.getElapsedTime().asMilliseconds() >= 2500 && rndnum >= 5) { // 50% spawn chance after 2 seconds passed
            cactus1.setPosition(window.getSize().x + 20, window.getSize().y - 80);
            clock1.restart();
        }
        if (clock2.getElapsedTime().asMilliseconds() >= 3500 && rndnum > 9) { // 10% spawn chance after 2.5 seconds
            cactus2.setPosition(window.getSize().x + 20, window.getSize().y - 80);
            clock2.restart();
        }
        if (timer.getElapsedTime().asSeconds() > 10) { // Start moving bird after 10 secs
            birdspawned = true;
            bird1.move(-movespeed / 2 * (1 + timer.getElapsedTime().asSeconds() / 13), 0);
        }
        if (birdspawned && clock3.getElapsedTime().asMilliseconds() >= 4500) { // 10% spawn chance every 2.5 seconds and after 10 secs of game
            bird1.setPosition(window.getSize().x + 20, window.getSize().y - 180);
            clock3.restart();
        }

		// Representation Of Text As Score

        stringstream ss;
        int score = timer.getElapsedTime().asMilliseconds() / 20;
        ss << "Score: " << score;
        scoretext.setString(ss.str());

        window.clear(Color::White);

        // Draw the rectangle + obstacles
        window.draw(dino);
        window.draw(cactus1);
        window.draw(cactus2);
        window.draw(bird1);
        window.draw(scoretext);

        window.display();
    }
}

// Function for Quit Button

int quitFunction(RenderWindow &window) {
    window.close();
    return 0;
}

// Function for Controls Button

void displayControls(RenderWindow &window, const Font &font) {
   RenderWindow controlsWindow(VideoMode(800, 600), "Controls");

    // Set background image for the controls window
    Texture controlsBackgroundTexture;
    if (controlsBackgroundTexture.loadFromFile("back.png"))
    {
        Sprite controlsBackgroundSprite(controlsBackgroundTexture);
        controlsWindow.draw(controlsBackgroundSprite);
    }

    // Heading text
    Text headingText("Controls", font, 30);
    headingText.setPosition(400 - headingText.getLocalBounds().width / 2, 50);

    // Text for control keys
    Text controlsText("W button :\nJump over the obstacles.\n\nD button :\nMove forward / Increase the speed.\n\nA button :\nSlow down the dino.", font, 20);
    controlsText.setPosition(800 - controlsText.getLocalBounds().width - 20, 300);

    // Additional information
    Text infoText("Press 'Esc' to close", font, 16);
    infoText.setPosition(10, 10);

    while (controlsWindow.isOpen())
    {
        Event controlsEvent;
        while (controlsWindow.pollEvent(controlsEvent))
        {
			// Checks If The Event Is Closed And Compare The Key Pressed
            if (controlsEvent.type == Event::Closed || (controlsEvent.type == Event::KeyPressed && controlsEvent.key.code == Keyboard::Escape))
            {
                controlsWindow.close();
            }
        }

        // Clear the controls window
        controlsWindow.clear();

        // Draw background image (if available)
        if (!controlsBackgroundTexture.loadFromFile("back.png"))
        {
            // Handle image loading error
            cout << "Error loading controls background image.\n";
        }
        else
        {
            Sprite controlsBackgroundSprite(controlsBackgroundTexture);
            controlsWindow.draw(controlsBackgroundSprite);
        }

        // Draw heading
        controlsWindow.draw(headingText);

        // Draw control keys
        controlsWindow.draw(controlsText);

        // Draw additional information
        controlsWindow.draw(infoText);

        // Display the contents of the controls window
        controlsWindow.display();
    }
}

int main() {
  RenderWindow window(VideoMode(800, 600), "Crazy Dino 2023");

    // Load background image
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("R (1).jpg"))
    {
        // Handle image loading error
        cout << "Error loading image.\n";
        return 1;
    }

    Sprite backgroundImage(backgroundTexture);

    // Set the initial view to match the window size
    View view(FloatRect(0, 0, 800, 600));
    window.setView(view);

    // Define font and text for buttons
    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // Handle font loading error
        cout << "Error loading font.\n";
        return 1;
    }

    // Text for buttons
    Text button1("Start", font, 20);
    Text button2("Controls", font, 20); 
    Text button3("Quit", font, 20);

    // Center text
    button1.setPosition(400 - button1.getLocalBounds().width / 2, 300 - 50);
    button2.setPosition(400 - button2.getLocalBounds().width / 2, 300); 
    button3.setPosition(400 - button3.getLocalBounds().width / 2, 300 + 50);

    // Header text
    Text header("Menu", font, 30);
    header.setStyle(Text::Bold);
    header.setPosition(400 - header.getLocalBounds().width / 2, 20);

    // Initialize variables for button selection
    int selectedButton = 0;
    RectangleShape selectionHighlight;
    selectionHighlight.setFillColor(Color::Transparent);
    selectionHighlight.setOutlineColor(Color::Green);
    selectionHighlight.setOutlineThickness(2);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::KeyPressed)
            {
                // Handle arrow key presses to change selected button
                if (event.key.code == Keyboard::Up && selectedButton > 0)
                {
                    selectedButton--;
                }
                else if (event.key.code == Keyboard::Down && selectedButton < 2)
                {
                    selectedButton++;
                }
                else if (event.key.code == Keyboard::Return)
                {
                    // Call a function based on the selected button
                    switch (selectedButton)
                    {
                    case 0:
                        startFunction(window);
                        break;
                    case 1:
                        // Display controls in a separate window
                        displayControls(window, font);
                        break;
                    case 2:
                        quitFunction(window);
                        break;
                    }
                }
            }
        }

        // Update selection highlight position and size based on the selected button's text
        Text selectedText;
        switch (selectedButton)
        {
        case 0:
            selectedText = button1;
            break;
        case 1:
            selectedText = button2;
            break;
        case 2:
            selectedText = button3;
            break;
        }

        selectionHighlight.setSize(Vector2f(selectedText.getLocalBounds().width + 10, selectedText.getLocalBounds().height + 10));
        selectionHighlight.setPosition(selectedText.getPosition().x - 5, selectedText.getPosition().y - 5);
		// Sets The Size Of Entity And Highights It


        // Clear the window
        window.clear();

        // Draw background image using the adjusted view
        window.setView(window.getDefaultView());
        window.draw(backgroundImage);

        // Reset the view to the original view
        window.setView(view);

        // Draw buttons and selection highlight
        window.draw(button1);
        window.draw(button2);
        window.draw(button3);
        window.draw(selectionHighlight);

        // Draw header
        window.draw(header);

        // Display the contents of the window
        window.display();
    }
	return 0;
}