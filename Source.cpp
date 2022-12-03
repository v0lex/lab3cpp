#include <iostream>
#include <vector>
#include <set>
#include<algorithm>

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

FindLinearEnvelope* LinearEnvelope::findLinearEnvelope = new Jarvis();

int main() {
	Point p1(1, 1), p2(3, 2), p3(4, 3), p4(2, 5), p5(4, 7), p6(10,7), p7(9,4), p8(11,3), p9(10,1);
	GroupOfPoints gop(std::vector<Point>{p1, p2, p3, p4, p5, p6, p7, p8, p9});
	LinearEnvelope le(gop);
	GroupOfVectors res = le.findLinearEnvelopeFunc();
	std::vector<Vector> resPoints = res.getVectors();
	std::cout << "Res: " << std::endl;
	for (Vector v : resPoints) {
		v.printVector();
	}
	//GroupOfPoints res = getLeftPartOfEnvelope(std::vector<Point>{p1, p2, p3, p4, p5});
	//std::vector<Point> resPoints = res.getPoints();
	//for (Point p : resPoints) {
		//p.printPoint();
	//}
	system("pause");
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
	std::cout << "Sorting: " << std::endl;
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
		std::cout << "Group: " << std::endl;
		for (Point p : gopPoints) {
			p.printPoint();
		}
	}
	for (GroupOfPoints gop : vgop) {
		if (!gop.getPoints().empty()) {
			lefts.push_back(getLeftInGroup(gop));
		}
	}
	return lefts;
}

Point getLeftInGroup(GroupOfPoints gop) {
	std::cout << "GetLeftingroup: " << std::endl;
	std::vector<Point> gopPoints = gop.getPoints();
	for (Point p : gopPoints) {
		p.printPoint();
	}
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
	Point p1(3, 1);
	Point p2 = vp.at(0);
	Vector vec(p2, p1);
	float angle = getAngle(Vector(points.at(0), points.at(1)), vec);
	std::cout << "Angle: " << angle << std::endl;
	for (int i = 2; i < vp.size(); ++i) {
		Vector newVector = Vector(vp.at(0), vp.at(i));
		float newAngle = getAngle(newVector, vec);
		if (newAngle > angle) {
			std::cout << "newAngle>angle";
			points.pop_back();
		}
		angle = newAngle;
		points.push_back(vp.at(i));
		/*while (points.size() >= 3) {
			std::cout << "Points: ";
			for (Point p : points) {
				p.printPoint();
			}
			std::cout << "Points.size: " << points.size() << std::endl;
			points = getLeftPartOfEnvelope(points);
		}*/
		for (Point p : points) {
			p.printPoint();
		}
	}
	return points;
}

std::vector<Point> getRightPartOfEnvelope(std::vector<Point> vp) {
	std::vector<Point> points = { vp.at(0), vp.at(1) };
	std::cout << "vp.at(0): ";
	vp.at(0).printPoint();
	Point p1(3, 1);
	Point p2 = vp.at(0);
	std::cout << "p1: ";
	p1.printPoint();
	std::cout << "p2: ";
	p2.printPoint();
	Vector vec(p2, p1);
	std::cout << "vec: ";
	vec.printVector();
	float angle = getAngle(Vector(points.at(0), points.at(1)), vec);
	std::cout << "Angle: " << angle << std::endl;
	for (int i = 2; i < vp.size(); ++i) {
		Vector newVector = Vector(vp.at(0), vp.at(i));
		std::cout << "newVector: ";
		newVector.printVector();
		float newAngle = getAngle(newVector, vec);
		std::cout << "newAngle: " << newAngle << std::endl;
		if (newAngle > angle) {
			std::cout << "newAngle>angle";
			points.pop_back();
		}
		angle = newAngle;
		points.push_back(vp.at(i));
		for (Point p : points) {
			p.printPoint();
		}
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


