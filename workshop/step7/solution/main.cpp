#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

const sf::Vector2f resolution{800.f, 600.f};

class Game
{
private:
    const float ballSpeed{6.f};
    const float playerSpeed{12.f};

    sf::CircleShape ball;
    sf::Vector2f ballVelocity;

    sf::RectangleShape player;
    sf::Vector2f playerVelocity;

    void initializeBall()
    {
        const float ballRadius{12.f};

        ball.setRadius(ballRadius);
        ball.setOrigin({ballRadius / 2.f, ballRadius / 2.f});
        ball.setFillColor(sf::Color::White);
        ball.setPosition(resolution / 2.f);
    }

    void initializePlayer()
    {
        const sf::Vector2f playerSize{128.f, 24.f};

        player.setSize(playerSize);
        player.setOrigin(playerSize / 2.f);
        player.setFillColor(sf::Color::White);
        player.setPosition(
            {resolution.x / 2.f, resolution.y - playerSize.y * 2});
    }

    void updateBallMovement()
    {
        ball.setPosition(ball.getPosition() + ballVelocity);
    }

    void updateBallCollisionsAgainstBoundaries()
    {
        const float ballLeft = ball.getPosition().x - ball.getRadius();
        const float ballRight = ball.getPosition().x + ball.getRadius();
        const float ballTop = ball.getPosition().y - ball.getRadius();
        const float ballBottom = ball.getPosition().y + ball.getRadius();

        const float boundaryLeft = 0.f;
        const float boundaryRight = resolution.x;
        const float boundaryTop = 0.f;
        const float boundaryBottom = resolution.y;

        const auto setBallX = [this](const float x)
        {
            sf::Vector2f newPosition = ball.getPosition();
            newPosition.x = x;
            ball.setPosition(newPosition);
        };

        const auto setBallY = [this](const float y)
        {
            sf::Vector2f newPosition = ball.getPosition();
            newPosition.y = y;
            ball.setPosition(newPosition);
        };

        if (ballLeft < boundaryLeft)
        {
            ballVelocity.x = -ballVelocity.x;
            setBallX(boundaryLeft + ball.getRadius());
        }
        else if (ballRight > boundaryRight)
        {
            ballVelocity.x = -ballVelocity.x;
            setBallX(boundaryRight - ball.getRadius());
        }

        if (ballTop < boundaryTop)
        {
            ballVelocity.y = -ballVelocity.y;
            setBallY(boundaryTop + ball.getRadius());
        }
        else if (ballBottom > boundaryBottom)
        {
            ballVelocity.y = -ballVelocity.y;
            setBallY(boundaryBottom - ball.getRadius());
        }
    }

    void updatePlayerInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            playerVelocity.x = -playerSpeed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            playerVelocity.x = playerSpeed;
        }
        else
        {
            playerVelocity.x = 0;
        }
    }

    void updatePlayerMovement()
    {
        player.setPosition(player.getPosition() + playerVelocity);
    }

    void updatePlayerCollisionsAgainstBoundaries()
    {
        const float playerHalfWidth = player.getSize().x / 2.f;
        const float playerLeft = player.getPosition().x - playerHalfWidth;
        const float playerRight = player.getPosition().x + playerHalfWidth;

        const float boundaryLeft = 0.f;
        const float boundaryRight = resolution.x;

        const auto setPlayerX = [this](const float x)
        {
            sf::Vector2f newPosition = player.getPosition();
            newPosition.x = x;
            player.setPosition(newPosition);
        };

        if (playerLeft < boundaryLeft)
        {
            setPlayerX(boundaryLeft + playerHalfWidth);
        }
        else if (playerRight > boundaryRight)
        {
            setPlayerX(boundaryRight - playerHalfWidth);
        }
    }

public:
    Game() : ballVelocity{ballSpeed, ballSpeed}, playerVelocity{0.f, 0.f}
    {
        initializeBall();
        initializePlayer();
    }

    void update()
    {
        updateBallMovement();
        updateBallCollisionsAgainstBoundaries();

        updatePlayerInput();
        updatePlayerMovement();
        updatePlayerCollisionsAgainstBoundaries();
    }

    void draw(sf::RenderTarget& renderTarget)
    {
        renderTarget.draw(ball);
        renderTarget.draw(player);
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y),
        "Arkanoid", sf::Style::Default, settings);

    window.setFramerateLimit(60);

    // ------------------------------------------------------------------------

    Game game;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        game.update();

        window.clear();
        game.draw(window);
        window.display();
    }

    // ------------------------------------------------------------------------

    return 0;
}
