// Construction and naming follow Grant Sanderson (3Blue1Brown),
// "Hilbert's Curve: Is infinite math useful?" (2017)
// https://www.youtube.com/watch?v=3s7h2MHQtxc
// Finite orders are pseudo-Hilbert curves; the true Hilbert curve is their limit.

#include <SFML/Graphics.hpp>  // window, events, VertexArray, Transform
#include <string>
#include <vector>             // list of Hilbert points for the current order

constexpr int MIN_ORDER = 1;
constexpr int MAX_ORDER = 10;
constexpr int START_ORDER = 3;

// Rule 1 & 2 base case (order = 1)
std::vector<sf::Vector2f> generateHilbert(int order) {
	// if order is 1, subdivide into 2x2 grid and trace the 4 quadrants' centers
	if (order == 1) {
		return {
			sf::Vector2f(0.0f , 0.0f), // Start - Bottom Left
			sf::Vector2f(0.0f , 1.0f), // Trace - Top Left
			sf::Vector2f(1.0f , 1.0f), // Trace - Top Right
			sf::Vector2f(1.0f , 0.0f)  // Trace - Bottom Right
		};
	}
	// Rules 3 & 4: Fetch previous order (n - 1)
	std::vector<sf::Vector2f> prev = generateHilbert(order - 1);
	float L = static_cast<float>(1 << (order - 1)); // L = 2^(order - 1)

	std::vector<sf::Vector2f> result;
	result.reserve(prev.size() * 4);

	// Rule 5: Bottom-Left quadrant (+90 deg flip / transpose)
	for (const auto& pt : prev) {
		result.push_back(sf::Vector2f(pt.y , pt.x));
	}

	// Rule 4: Top-Left quadrant (Shift up by L)
	for (const auto& pt : prev) {
		result.push_back(sf::Vector2f(pt.x , pt.y + L));
	}

	// Rule 4: Top-Right quadrant (Shift right by L)
	for (const auto& pt : prev) {
		result.push_back(sf::Vector2f(pt.x + L , pt.y + L));
	}

	// Rule 5: Bottom-Right quadrant (-90 deg flip / transpose)
	for (const auto& pt : prev) {
		result.push_back(sf::Vector2f((2.f * L - 1.f) - pt.y, (L - 1.f) - pt.x));
	}

	return result;
}

sf::View createHilbertView(int order) {
	// Grid runs from 0 to 2^(order - 1). Scale that box to a fixed fraction of the window size.
	// Window size = 1024x1024  
	const float maxCoord = static_cast<float>((1 << order) - 1);	// 2^(order - 1)
	const float margin = 0.1f; 	// 10% empty border on each side, at every n
	const float worldSize = maxCoord / (1.0f - 2.0f * margin);

	sf::View view;
	view.setCenter( {maxCoord/2.0f, maxCoord/2.0f} );
	view.setSize({ worldSize, -worldSize}); // Negative height flips +Y upwards

	return view;
}

/*
sf::View createHilbertView(int order) {
	float maxCoord = static_cast<float>((1 << order) - 1);	// 2^(order - 1)
	float padding = 1.0f;
	float worldSize = maxCoord + 2.0f * padding;

	sf::View view;
	view.setCenter( {maxCoord/2.0f, maxCoord/2.0f} );
	view.setSize({ worldSize, -worldSize}); // Negative height flips +Y upwards

	return view;
}
*/

// To draw shapes on the screen, SFML requires this function (VertexArray)
// Each element inside VertexArray is an {sf::Vertex} which packages multiples pieces of data together

sf::VertexArray buildCurve(int order) {
	const std::vector<sf::Vector2f> points = generateHilbert(order);
	sf::VertexArray curve(sf::PrimitiveType::LineStrip, points.size());
	for (std::size_t i = 0; i < points.size(); ++i) {
		curve[i].position = points[i];
		curve[i].color = sf::Color::White;
		// Position & Color ==> where it is in 2D space
	}
	return curve;
}

std::string windowTitle(int order) {
	return "Pseudo-Hilbert Curve - Order " + std::to_string(order);
}

int main() {
	int order = START_ORDER;

	sf::RenderWindow window( sf::VideoMode( { 1024, 1024 } ), windowTitle(order) );
	window.setVerticalSyncEnabled(true);

	sf::VertexArray curve = buildCurve(order);

	while ( window.isOpen() ) {
		while ( const std::optional event = window.pollEvent() ) {
			if ( event->is<sf::Event::Closed>() )
				window.close();

			if ( const auto* key = event->getIf<sf::Event::KeyPressed>() ) {
				const bool increase =
					key->code == sf::Keyboard::Key::Up ||
					key->code == sf::Keyboard::Key::Equal ||
					key->code == sf::Keyboard::Key::Add;
				const bool decrease =
					key->code == sf::Keyboard::Key::Down ||
					key->code == sf::Keyboard::Key::Hyphen ||
					key->code == sf::Keyboard::Key::Subtract;

				if ( increase && order < MAX_ORDER ) {
					++order;
					curve = buildCurve(order);
					window.setTitle(windowTitle(order));
				} else if ( decrease && order > MIN_ORDER ) {
					--order;
					curve = buildCurve(order);
					window.setTitle(windowTitle(order));
				}
			}
		}

		window.clear(sf::Color(15, 15, 20));
		window.setView(createHilbertView(order));
		window.draw(curve);
		window.display();
	}
}