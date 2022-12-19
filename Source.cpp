#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <set>
#include<algorithm>
#include<math.h>
#include<tuple>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

class Point {
public:
	Point() {};
	Point(float);
	Point(float, float);
	float getX() const;
	float getY() const;
	bool operator==(Point);
	void printPoint() const;
private:
	float x = 0;
	float y = 0;
};

class Vector {
public:
	Vector() {};
	Vector(float);
	Vector(Point, Point);
	float length() const;
	Point coordinates() const;
	Point getBegin() const;
	Point getEnd() const;
	void printVector() const;
private:
	Point begin = 0;
	Point end = 0;
};

class GroupOfPoints {
public: 
	GroupOfPoints() {};
	GroupOfPoints(std::vector<Point>);
	std::vector<Point> getPoints() const;
	void printGroupOfPoints() const;
	void add(Point);
private:
	std::vector<Point> points;
};

class GroupOfVectors {
public:
	GroupOfVectors() {};
	GroupOfVectors(std::vector<Vector>);
	std::vector<Vector> getVectors() const;
	void printGroupOfVectors() const;
	void add(Vector);
private: 
	std::vector<Vector> vectors;
};

class FindLinearEnvelope;

class LinearEnvelope {
public:
	LinearEnvelope() {};
	LinearEnvelope(GroupOfPoints);
	static void setFindLinearEnvelope(FindLinearEnvelope*);
	GroupOfVectors findLinearEnvelopeFunc() const;
	void printLinearEnvelope() const;
	GroupOfPoints getPoints() const;
private:
	GroupOfPoints points;
	static FindLinearEnvelope* findLinearEnvelope;
};

class FindLinearEnvelope {
public:
	virtual GroupOfVectors findLinearEnvelope(GroupOfPoints) = 0;
};

class Kirkpatrick : public FindLinearEnvelope {
public: 
	GroupOfVectors findLinearEnvelope(GroupOfPoints);
};

class Jarvis : public FindLinearEnvelope {
public:
	GroupOfVectors findLinearEnvelope(GroupOfPoints);
};

class Graham : public FindLinearEnvelope {
public:
	GroupOfVectors findLinearEnvelope(GroupOfPoints);
};

class Recursive : public FindLinearEnvelope {
public:
	GroupOfVectors findLinearEnvelope(GroupOfPoints);
};

std::vector<GroupOfPoints> pocketSortGroupOfPoints(GroupOfPoints);
Point getLeftInGroup(GroupOfPoints);
Point getRightInGroup(GroupOfPoints);
std::vector<Point> getLeftsInLinearEnvelope(std::vector<GroupOfPoints>);
std::vector<Point> getRightsInLinearEnvelope(std::vector<GroupOfPoints>);
std::vector<Point> getLeftPartOfEnvelope(std::vector<Point>);
std::vector<Point> getRightPartOfEnvelope(std::vector<Point>);
float getScalar(Vector v1, Vector v2);
float getAngle(Vector v1, Vector v2);
float getPointValueForLine(Point, Point, Point);
bool checkPoint(Vector, std::vector<Point>);
GroupOfVectors groupOfPointToGroupOFVectors(GroupOfPoints);
GroupOfPoints sortPolar(GroupOfPoints, Point);
float orientedSquare(Point, Point, Point);
bool checkPoints(Point, Point, Point, Point);
float getCosinus(Point, Point, Point);
std::vector<Point> getGrahamEnvelope(std::vector<Point>, Point);
GroupOfPoints sortByX(GroupOfPoints);
std::pair < std::vector<Point>, std::vector<Point>> split(std::vector<Point>, Point, Point, Point);
Point findPoint(std::vector<Point>, Point, Point);
float findTriangleSquare(Point, Point, Point);
float findLength(Point, Point);
std::vector<std::vector<Point>> findRecursiveEnvelope(std::vector<std::vector<Point>>, Point, Point, std::vector<Point>&);
void drawPoint(Point);
void drawPoints(GroupOfPoints);
void drawPoint(Point, sf::Color);
void drawPoints(GroupOfPoints, sf::Color);
void drawLine(Vector);
void drawLines(GroupOfVectors);
GroupOfPoints groupOfVectorsToGroupOfPoints(GroupOfVectors);

FindLinearEnvelope* LinearEnvelope::findLinearEnvelope = new Kirkpatrick();
sf::RenderWindow window(sf::VideoMode(50, 50), "SFML works!");

int main() {
	/*Point p1(1, 1), p2(3, 2), p3(4, 2), p4(2, 5), p5(4, 7), p6(10, 7), p7(9, 4), p8(11, 3), p9(10, 1);
	GroupOfPoints gop(std::vector<Point>{p1, p2, p3, p4, p5, p6, p7, p8, p9});
	LinearEnvelope le(gop);
	GroupOfVectors res = le.findLinearEnvelopeFunc();
	std::vector<Vector> resPoints = res.getVectors();
	//std::cout << "Res: " << std::endl;
	//for (Vector v : resPoints) {
		//v.printVector();
	//}
	//GroupOfPoints res = getLeftPartOfEnvelope(std::vector<Point>{p1, p2, p3, p4, p5});
	//std::vector<Point> resPoints = res.getPoints();
	//for (Point p : resPoints) {
		//p.printPoint();
	//}
	system("pause");
	return 0;*/
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	Point p1(3, 1), p2(3, 2), p3(4, 2), p4(2, 5), p5(4, 7), p6(10, 7), p7(9, 4), p8(11, 3), p9(10, 1);
	GroupOfPoints gop(std::vector<Point>{p1, p2, p3, p4, p5, p6, p7, p8, p9});
	LinearEnvelope le(gop);
	GroupOfVectors res = le.findLinearEnvelopeFunc();
	GroupOfPoints resGop = groupOfVectorsToGroupOfPoints(res);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		drawPoints(gop);
		drawPoints(resGop, sf::Color::Red);
		window.display();
	}

	return 0;
}

Point::Point(float a) : x(a), y(a) {};

Point::Point(float x, float y) : x(x), y(y) {};

float Point::getX() const{
	return x;
}

float Point::getY() const{
	return y;
}

bool Point::operator== (Point other) {
	return x == other.x && y == other.y;
}

void Point::printPoint() const{
	std::cout << "(" << x << "; " << y << ")" << std::endl;
}

Vector::Vector(float a) : begin(a), end(a) {};

Vector::Vector(Point begin, Point end) : begin(begin), end(end) {};

float Vector::length() const {
	Point vectorCoordinates = coordinates();
	float xCoordinate = vectorCoordinates.getX();
	float yCoordinate = vectorCoordinates.getY();
	return sqrt(pow(xCoordinate, 2) + pow(yCoordinate, 2));
}

Point Vector::coordinates() const {
	float beginX = begin.getX();
	float beginY = begin.getY();
	float endX = end.getX();
	float endY = end.getY();
	return Point(endX - beginX, endY - beginY);
}

void Vector::printVector() const {
	std::cout << "Begin: ";
	begin.printPoint();
	std::cout << "End:   ";
	end.printPoint();
}

Point Vector::getBegin() const {
	return begin;
}

Point Vector::getEnd() const {
	return end;
}

GroupOfPoints::GroupOfPoints(std::vector<Point> groupOfPoints) : points(groupOfPoints) {};

std::vector<Point> GroupOfPoints::getPoints() const {
	return points;
}

void GroupOfPoints::printGroupOfPoints() const {
	for (int i = 0; i < points.size(); ++i) {
		std::cout << "Point " << i + 1 << ": " << std::endl;
		points.at(i).printPoint();
	}
}

void GroupOfPoints::add(Point p) {
	points.push_back(p);
}

GroupOfVectors::GroupOfVectors(std::vector<Vector> groupOfVectors) : vectors(groupOfVectors) {};

std::vector<Vector> GroupOfVectors::getVectors() const {
	return vectors;
}

void GroupOfVectors::printGroupOfVectors() const {
	for (int i = 0; i < vectors.size(); ++i) {
		std::cout << "Vector " << i + 1 << ": " << std::endl;
		vectors.at(i).printVector();
	}
}

void GroupOfVectors::add(Vector v) {
	vectors.push_back(v);
}

LinearEnvelope::LinearEnvelope(GroupOfPoints groupOfPoints) : points(groupOfPoints) {};

void LinearEnvelope::setFindLinearEnvelope(FindLinearEnvelope* pfindLinearEnvelope) {
	findLinearEnvelope = pfindLinearEnvelope;
}

GroupOfVectors LinearEnvelope::findLinearEnvelopeFunc() const {
	return findLinearEnvelope->findLinearEnvelope(this->points);
}

GroupOfPoints LinearEnvelope::getPoints() const {
	return points;
}

GroupOfVectors Kirkpatrick::findLinearEnvelope(GroupOfPoints gop) {
	std::cout << "Kirkpatrick: " << std::endl;
	//std::cout << "Sorting: " << std::endl;
	std::vector<GroupOfPoints> sortedPoints = pocketSortGroupOfPoints(gop);
	std::cout << "Lefts: " << std::endl;
	std::vector<Point> lefts = getLeftsInLinearEnvelope(sortedPoints);
	for (Point p : lefts) {
		p.printPoint();
	}
	std::cout << "Rights: " << std::endl;
	std::vector<Point> rights = getRightsInLinearEnvelope(sortedPoints);
	for (Point p : rights) {
		p.printPoint();
	}
	GroupOfPoints rgop;
	std::cout << "LeftEnvelope: ";
	std::vector<Point> leftEnvelope = getLeftPartOfEnvelope(lefts);
	std::vector<Point> oldLeftEnvelope = leftEnvelope;
	std::vector<Point> newLeftEnvelope = leftEnvelope;
	while (true) {
		newLeftEnvelope = getLeftPartOfEnvelope(oldLeftEnvelope);
		std::cout << "NewLeftEnvelope: " << std::endl;
		for (Point p : newLeftEnvelope) {
			p.printPoint();
		}
		std::cout << "OldLeftEnvelope: " << std::endl;
		for (Point p : oldLeftEnvelope) {
			p.printPoint();
		}
		if (std::equal(newLeftEnvelope.begin(), newLeftEnvelope.end(), oldLeftEnvelope.begin())) {
			break;
		}
		oldLeftEnvelope = newLeftEnvelope;
	}
	for (Point p : newLeftEnvelope) {
		p.printPoint();
	}
	std::cout << "RightEnvelope: ";
	std::vector<Point> rightEnvelope = getRightPartOfEnvelope(rights);
	std::vector<Point> oldRightEnvelope = rightEnvelope;
	std::vector<Point> newRightEnvelope = rightEnvelope;
	while (true) {
		newRightEnvelope = getRightPartOfEnvelope(oldRightEnvelope);
		std::cout << "NewRightEnvelope: " << std::endl;
		for (Point p : newRightEnvelope) {
			p.printPoint();
		}
	std::cout << "OldRightEnvelope: " << std::endl;
		for (Point p : oldRightEnvelope) {
			p.printPoint();
		}
		if (std::equal(newRightEnvelope.begin(), newRightEnvelope.end(), oldRightEnvelope.begin())) {
			break;
		}
		oldRightEnvelope = newRightEnvelope;
	}
	for (Point p : newRightEnvelope) {
		p.printPoint();
	}
	std::reverse(newRightEnvelope.begin(), newRightEnvelope.end());
	for (Point p : newLeftEnvelope) {
		rgop.add(p);
	}
	for (Point p : newRightEnvelope) {
		rgop.add(p);
	}
	return groupOfPointToGroupOFVectors(rgop);
}

std::vector<GroupOfPoints> pocketSortGroupOfPoints(GroupOfPoints gop) {
	std::set<int> yCoordinates;
	for (Point p : gop.getPoints()) {
		yCoordinates.insert((int)p.getY());
	}
	std::vector<GroupOfPoints> sortedPoints;
	sortedPoints.resize(*std::max_element(yCoordinates.begin(), yCoordinates.end()));
	for (Point p : gop.getPoints()) {
		sortedPoints.at(p.getY() - 1).add(p);
	}
	return sortedPoints;
}

std::vector<Point> getLeftsInLinearEnvelope(std::vector<GroupOfPoints> vgop) {
	std::vector<Point> lefts;
	for (GroupOfPoints gop : vgop) {
		std::vector<Point> gopPoints = gop.getPoints();
		//std::cout << "Group: " << std::endl;
		//for (Point p : gopPoints) {
			//p.printPoint();
		//}
	}
	for (GroupOfPoints gop : vgop) {
		if (!gop.getPoints().empty()) {
			lefts.push_back(getLeftInGroup(gop));
		}
	}
	return lefts;
}

Point getLeftInGroup(GroupOfPoints gop) {
	//std::cout << "GetLeftingroup: " << std::endl;
	std::vector<Point> gopPoints = gop.getPoints();
	//for (Point p : gopPoints) {
		//p.printPoint();
	//}
	Point left = gopPoints.at(0);
	for (int i = 1; i < gopPoints.size(); ++i) {
		if (gopPoints.at(i).getX() < left.getX()) {
			left = gopPoints.at(i);
		}
	}
	return left;
}

std::vector<Point> getRightsInLinearEnvelope(std::vector<GroupOfPoints> vgop) {
	std::vector<Point> rights;
	for (GroupOfPoints gop : vgop) {
		if (!gop.getPoints().empty()) {
			rights.push_back(getRightInGroup(gop));
		}
	}
	return rights;
}

Point getRightInGroup(GroupOfPoints gop) {
	std::vector<Point> gopPoints = gop.getPoints();
	Point right = gopPoints.at(0);
	for (int i = 1; i < gopPoints.size(); ++i) {
		if (gopPoints.at(i).getX() > right.getX()) {
			right = gopPoints.at(i);
		}
	}
	return right;
}

std::vector<Point> getLeftPartOfEnvelope(std::vector<Point> vp) {
	std::vector<Point> points = { vp.at(0), vp.at(1)};
	//std::cout << "Angle: " << angle << std::endl;
	for (int i = 2; i < vp.size(); ++i) {
		Vector v1 = Vector(points.at(points.size()-2), points.at(points.size() - 1));
		Vector v2 = Vector(points.at(points.size() - 1), vp.at(i));
		float newAngle = getAngle(v1, v2);
		if (newAngle < M_PI/2) {
			//std::cout << "newAngle>angle";
			points.pop_back();
		}
		points.push_back(vp.at(i));
		/*while (points.size() >= 3) {
			std::cout << "Points: ";
			for (Point p : points) {
				p.printPoint();
			}
			std::cout << "Points.size: " << points.size() << std::endl;
			points = getLeftPartOfEnvelope(points);
		}*/
		//for (Point p : points) {
			//p.printPoint();
		//}
	}
	return points;
}

std::vector<Point> getRightPartOfEnvelope(std::vector<Point> vp) {
	std::vector<Point> points = { vp.at(0), vp.at(1) };
	//std::cout << "vp.at(0): ";
	//vp.at(0).printPoint();
	//std::cout << "Angle: " << angle << std::endl;
	for (int i = 2; i < vp.size(); ++i) {
		Vector v1 = Vector(points.at(points.size() - 2), points.at(points.size() - 1));
		Vector v2 = Vector(points.at(points.size() - 2), vp.at(i));
		std::cout << "Vector v1: " << std::endl;
		v1.printVector();
		std::cout << "Vector v2: " << std::endl;
		v2.printVector();
		float newAngle = getAngle(v1, v2);
		std::cout << "Angle: " << newAngle << std::endl;
		if (newAngle < M_PI / 2) {
			//std::cout << "newAngle>angle";
			points.pop_back();
		}
		points.push_back(vp.at(i));
		/*Vector v1 = Vector(vp.at(i - 1), vp.at(i - 2));
		Vector v2 = Vector(vp.at(i - 1), vp.at(i));
		std::cout << "Vector v1: " << std::endl;
		v1.printVector();
		std::cout << "Vector v2: " << std::endl;
		v2.printVector();
		float newAngle = getAngle(v1, v2);
		std::cout << "Angle: " << newAngle << std::endl;
		if (newAngle <= M_PI / 2) {
			//std::cout << "newAngle>angle";
			points.pop_back();
			for (int j = i; j < vp.size(); j++) {
				points.push_back(vp.at(j));
			}
			break;
		}
		points.push_back(vp.at(i));*/

		//for (Point p : points) {
			//p.printPoint();
		//}
	}
	return points;
}

float getScalar(Vector v1, Vector v2) {
	float v1X = v1.coordinates().getX();
	float v1Y = v1.coordinates().getY();
	float v2X = v2.coordinates().getX();
	float v2Y = v2.coordinates().getY();
	return v1X * v2X + v1Y * v2Y;
}

float getAngle(Vector v1, Vector v2) {
	return acos((getScalar(v1, v2)) / (v1.length() * v2.length()));
}

GroupOfVectors Jarvis::findLinearEnvelope(GroupOfPoints gop) {
	GroupOfVectors allVectors, evelopeVectors;
	std::vector<Point> startedPoints = gop.getPoints();
	for (int i = 0; i < startedPoints.size(); ++i) {
		for (int j = 0; j < startedPoints.size(); ++j) {
			if (i != j) {
				allVectors.add(Vector(startedPoints.at(i), startedPoints.at(j)));
			}
		}
	}
	for (Vector v : allVectors.getVectors()) {
		if (checkPoint(v, startedPoints)) {
			evelopeVectors.add(v);
		}
	}
	return evelopeVectors;
}

float getPointValueForLine(Point p, Point line1, Point line2) {
	return ((p.getX() - line1.getX()) / (line2.getX() - line1.getX())) - ((p.getY() - line1.getY()) / (line2.getY() - line1.getY()));
}

bool checkPoint(Vector v, std::vector<Point> vp) {
	bool allMoreThenZero = true;
	bool allLessThenZero = true;
	for (int i = 0; i < vp.size(); ++i) {
		if (getPointValueForLine(vp.at(i), v.getBegin(), v.getEnd()) > 0) {
			allLessThenZero = false;
		}
		else if (getPointValueForLine(vp.at(i), v.getBegin(), v.getEnd()) < 0) {
			allMoreThenZero = false;
		}
		if (!(allMoreThenZero || allLessThenZero)) {
			return false;
		}
	}
	return true;
}

GroupOfVectors groupOfPointToGroupOFVectors(GroupOfPoints gop) {
	std::vector<Point> points = gop.getPoints();
	GroupOfVectors vectors;
	for (int i = 0; i < points.size(); ++i) {
		if (i != points.size() - 1) {
			vectors.add(Vector(points.at(i), points.at(i + 1)));
		}
		else {
			vectors.add(Vector(points.at(i), points.at(0)));
		}
	}
	return vectors;
}

GroupOfVectors Graham::findLinearEnvelope(GroupOfPoints gop) {
	//std::cout << "Graham" << std::endl;
	Point q(6, 5);
	GroupOfPoints sortedgop = sortPolar(gop, q);
	std::vector<Point> sortedPoints = sortedgop.getPoints();
	//std::cout << "Sorted points: ";
	//for (Point p : sortedPoints) {
		//p.printPoint();
	//}
	//std::cout << "sdadadsaddas" << std::endl;
	std::vector<Point> res = getGrahamEnvelope(sortedPoints, q);
	//std::cout << "Res: ";
	//for (Point p : res) {
		//p.printPoint();
	//}
	std::vector<Point> oldEnvelope = res;
	std::vector<Point> newEnvelope = res;
	while(true) {
		newEnvelope = getGrahamEnvelope(oldEnvelope, q);
		//std::cout << "OldEnvelope: ";
		//for (Point p : oldEnvelope) {
			//p.printPoint();
		//}
		//std::cout << "NewEnvelope: ";
		//for (Point p : newEnvelope) {
			//p.printPoint();
		//}
		if (std::equal(newEnvelope.begin(), newEnvelope.end(), oldEnvelope.begin())) {
			break;
		}
		oldEnvelope = newEnvelope;
	}
	GroupOfPoints rgop(newEnvelope);
	return groupOfPointToGroupOFVectors(rgop);
}

GroupOfPoints sortPolar(GroupOfPoints gop, Point p) {
	std::vector<Point> points = gop.getPoints();
	bool swapped = false;
	do {
		swapped = false;
		for (int i = 1; i < points.size(); ++i) {
			float os = orientedSquare(p, points[i - 1], points[i]);
			//std::cout << "OrSquare res: " << os << std::endl;
			if ( os <= 0) {
				std::swap(points[i - 1], points[i]);
				swapped = true;
			}
		}
		//std::cout << "Points: ";
		//for (Point p : points) {
			//p.printPoint();
		//}
	} while (swapped);
	return points;
}

float orientedSquare(Point a, Point b, Point c) {
	//std::cout << "oriented squre method: " << std::endl;
	//a.printPoint();
	//b.printPoint();
	//c.printPoint();
	return 0.5 * (a.getX() * b.getY() + b.getX() * c.getY() + c.getX() * a.getY() - a.getY() * b.getX() - b.getY() * c.getX() - c.getY() * a.getX());
}

bool checkPoints(Point a, Point b, Point c, Point p) {
	//std::cout << "getCosinus(a, b, p): "<<getCosinus(a, b, p) << std::endl;
	//std::cout << "getCosinus(b, c, p): " << getCosinus(c, b, p) << std::endl;
	//return getCosinus(a, b, p) < 0 || getCosinus(c, b, p) > 0;
	float abp = getAngle(Vector(b, a), Vector(b, p));
	float cbp = getAngle(Vector(b, c), Vector(b, p));
	return abp+cbp>=M_PI;
}

float getCosinus(Point a, Point b, Point c) {
	Vector v1(b, a), v2(b, c);
	//std::cout << "(a, b): ";
	//v1.coordinates().printPoint();
	//std::cout << "(b, c): ";
	//v2.coordinates().printPoint();
	return (getScalar(v1, v2)) / (v1.length() * v2.length());
}

std::vector<Point> getGrahamEnvelope(std::vector<Point> sortedPoints, Point q) {
	//std::cout << "GetGrahamEnvelope: " << std::endl;
	//std::cout << "Sorted: ";
	//for (Point p : sortedPoints) {
		//p.printPoint();
	//}
	int j = 1;
	std::vector<Point> res = {sortedPoints[0], sortedPoints[1]};
	for (int i = 2; i < sortedPoints.size(); ++i) {
		//std::cout << "i: " << i << std::endl;
		/*if (i != 2) {
			if (!(std::count(res.begin(), res.end(), sortedPoints[i - 2]) >= 1)) {
				res.push_back(sortedPoints[i - 2]);
			}
			if (!(std::count(res.begin(), res.end(), sortedPoints[i - 1]) >= 1)) {
				res.push_back(sortedPoints[i - 1]);
			}
			std::cout << "checkPoints: " << checkPoints(res[i - 2], res[i - 1], sortedPoints[i], q) << std::endl;
			if ((checkPoints(res[i - 2], res[i - 1], sortedPoints[i], q))) {
				res.pop_back();
			}
			res.push_back(sortedPoints[i]);
		}
		else {
			res.push_back(sortedPoints[i - 2]);
			res.push_back(sortedPoints[i - 1]);
			std::cout << "checkPoints: " << checkPoints(res[i - 2], res[i - 1], sortedPoints[i], q) << std::endl;
			if (checkPoints(res[i - 2], res[i - 1], sortedPoints[i], q)) {
				res.pop_back();
			}
			res.push_back(sortedPoints[i]);
		}*/
		if (checkPoints(res[j - 1], res[j], sortedPoints[i], q)) {
			res.pop_back();
			j--;
		}
		res.push_back(sortedPoints[i]);
		j++;
		//std::cout << "Res: ";
		//for (Point p : res) {
			//p.printPoint();
		//}
	}
	return res;
}


GroupOfVectors Recursive::findLinearEnvelope(GroupOfPoints gop) {
	//std::cout << "Recursive: " << std::endl;
	std::vector<Point> sortedPoints = sortByX(gop).getPoints();
	Point l = sortedPoints.at(0);
	Point r = sortedPoints.at(sortedPoints.size() - 1);
	std::vector<Point> s1, s2;
	for (int i = 1; i < sortedPoints.size() - 1; ++i) {
		if (getPointValueForLine(sortedPoints[i], l, r) >= 0) {
			s2.push_back(sortedPoints[i]);
		}
		else {
			s1.push_back(sortedPoints[i]);
		}
	}
	std::vector<std::vector<Point>> pv1 = { s1 }, pv2 = { s2 };
	std::vector<Point> resPoints = { l,r };
	std::vector<std::vector<Point>> p1 = findRecursiveEnvelope(pv1, l, r, resPoints);
	std::vector<std::vector<Point>> p2 = findRecursiveEnvelope(pv2, l, r, resPoints);
	//std::cout << "p1: ";
	//for (std::vector<Point> pv : p1) {
		//for (Point p : pv) {
			//p.printPoint();
		//}
	//}
	//std::cout << "p2: ";
	//for (std::vector<Point> pv : p2) {
		//for (Point p : pv) {
			//p.printPoint();
		//}
	//}
	//std::cout << "RESSSSSSSSSSSSPOints: " << std::endl;
	//for (Point p : resPoints) {
		//p.printPoint();
	//}
	GroupOfPoints resgop(resPoints);
	return groupOfPointToGroupOFVectors(resgop);
}

GroupOfPoints sortByX(GroupOfPoints gop) {
	std::vector<Point> points = gop.getPoints();
	bool swapped = false;
	do {
		swapped = false;
		for (int i = 1; i < points.size(); ++i) {
			if (points[i-1].getX() > points[i].getX()) {
				std::swap(points[i - 1], points[i]);
				swapped = true;
			}
		}
		//std::cout << "Points: ";
		//for (Point p : points) {
			//p.printPoint();
		//}
	} while (swapped);
	return points;
}

std::pair < std::vector<Point>, std::vector<Point>> split(std::vector<Point> points, Point l, Point r, Point h) {
	std::vector<Point> s1, s2;
	for (int i = 0; i < points.size(); i++) {
		if (getPointValueForLine(points[i], l, h) < 0) {
			s1.push_back(points[i]);
		}
		if (getPointValueForLine(points[i], h, r) > 0) {
			s2.push_back(points[i]);
		}
	}
	return std::make_pair(s1, s2);
};

Point findPoint(std::vector<Point> points, Point l, Point r) {
	std::vector<int> indexesOfMaxSquare = { 0 };
	float maxSquare = findTriangleSquare(l, points.at(0), r);
	for (int i = 1; i < points.size(); ++i) {
		float square = findTriangleSquare(l, points.at(i), r);
		if (square == maxSquare) {
			indexesOfMaxSquare.push_back(i);
		}
		else if (square > maxSquare) {
			indexesOfMaxSquare.clear();
			indexesOfMaxSquare.push_back(i);
			maxSquare = square;
		}
	}
	if (indexesOfMaxSquare.size() == 1) {
		return points.at(indexesOfMaxSquare.at(0));
	}
	else {
		int indexOfMaxAngle = indexesOfMaxSquare.at(0);
		float maxAngle = getAngle(Vector(l, points.at(indexesOfMaxSquare.at(0))), Vector(l, r));
		for (int i = 1; i < indexesOfMaxSquare.size(); ++i) {
			float angle = getAngle(Vector(l, points.at(indexesOfMaxSquare.at(i))), Vector(l, r));
			if (angle > maxAngle) {
				maxAngle = angle;
				indexOfMaxAngle = indexesOfMaxSquare.at(i);
			}
		}
		return points.at(indexOfMaxAngle);
	}
}

float findTriangleSquare(Point a, Point b, Point c) {
	float ab = findLength(a, b);
	float bc = findLength(b, c);
	float ac = findLength(a, c);
	float p = (ab + ac + bc) / 2;
	float s = sqrt(p * (p - ab) * (p - bc) * (p - ac));
	return s;
}

float findLength(Point a, Point b) {
	return sqrt(pow(b.getX() - a.getX(), 2) + pow(b.getY() - a.getY(), 2));
}

std::vector<std::vector<Point>> findRecursiveEnvelope(std::vector<std::vector<Point>> s1, Point l, Point r, std::vector<Point>& resPoints) {
	std::vector<std::vector<Point>> res;
	//std::cout << "s1: " << std::endl;
	//for (std::vector<Point> pv : s1) {
		//std::cout << "Point vec: " << std::endl;
		//for (Point p : pv) {
			//p.printPoint();
		//}
	//}
	bool allSizesZero = true;
	for (std::vector<Point> p : s1) {
		if (p.size() > 0) {
			allSizesZero = false;
			break;
		}
	}
	if (allSizesZero) {
		return s1;
	}
	std::vector<Point> s11, s12;
	for (std::vector<Point> p : s1) {
		Point h = findPoint(p, l, r);
		resPoints.push_back(h);
		std::tie(s11,s12) = split(p, l,r,h);
		res.push_back(s11);
		res.push_back(s12);
		std::vector<std::vector<Point>> res1, res2;
		res1.push_back(s11);
		res2.push_back(s12);
		res1 = findRecursiveEnvelope(res1, l, h, resPoints);
		res2 = findRecursiveEnvelope(res2, h, r, resPoints);
	}
	//std::cout << "resPoints: ";
	//for (Point p : resPoints) {
		//p.printPoint();
	//}
	return res;
}

void drawPoint(Point p) {
	sf::Vertex point(sf::Vector2f(p.getX(), p.getY()), sf::Color::White);
	window.draw(&point, 1, sf::Points);
}

void drawPoints(GroupOfPoints gop) {
	std::vector<Point> points = gop.getPoints();
	for (Point p : points) {
		drawPoint(p);
	}
}

void drawLine(Vector v) {
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(v.getBegin().getX(), v.getBegin().getY()), sf::Color::White),
		sf::Vertex(sf::Vector2f(v.getEnd().getX(), v.getEnd().getY()), sf::Color::White)
	};
	window.draw(line, 2, sf::Lines);
}

void drawLines(GroupOfVectors gov) {
	std::vector<Vector> vectors = gov.getVectors();
	for (Vector v : vectors) {
		drawLine(v);
	}
}

GroupOfPoints groupOfVectorsToGroupOfPoints(GroupOfVectors gov) {
	std::vector<Point> points;
	std::vector<Vector> vectors = gov.getVectors();
	for (Vector v : vectors) {
		points.push_back(v.getBegin());
		points.push_back(v.getEnd());
	}
	return GroupOfPoints(points);
}

void drawPoint(Point p, sf::Color c) {
	sf::Vertex point(sf::Vector2f(p.getX(), p.getY()), c);
	window.draw(&point, 1, sf::Points);
}
void drawPoints(GroupOfPoints gop, sf::Color c) {
	std::vector<Point> points = gop.getPoints();
	for (Point p : points) {
		drawPoint(p, c);
	}
}



