#include <cstdio>

struct Color {
	enum ColorEnum { RED, GREEN, DEFAULT };
	static void setColor(ColorEnum color){		
		static const char * pallete[] = { "\x1B[31m", "\x1B[32m", "\033[0m" };
		std::puts(pallete[color]);
	}
};

/*
template <class T>
double drawBig(T& shape){
	double a0 = shape.area();
	shape.scale(2);
	double a1 = shape.area();
	return a1 - a0;
}
*/

class Scaleable {
public:
	virtual ~Scaleable() = 0;	
	virtual void scale(double f = 2.0) = 0;
};

Scaleable::~Scaleable(){
	std::puts("~Scaleable");
}

class Shape : public Scaleable {
public:
	Shape() : m_id(++NumOfShapes) {
		std::printf("[%d] Shape::CTOR\n", m_id); 
	}

	~Shape(){
		draw();
		std::printf("[%d] Shape::DTOR\n", m_id);
		--NumOfShapes; 
	}

	Shape(const Shape& other) : m_id(++NumOfShapes) {
		std::printf("[%d] Shape::CCTOR from [%d]\n", m_id, other.m_id);
	}

	virtual void draw() const {
		std::printf("[%d] Shape::draw()\n", m_id);		
	}
	
	virtual void draw(Color::ColorEnum c) const {
		Color::setColor(c);
		std::printf("[%d] Shape::draw(Color)\n", m_id);
		draw();
		Color::setColor(Color::DEFAULT);
	}
	
	virtual void scale(double) { }
	virtual double area() { return -1; }
	
	static void printInventory() {
		std::printf("Shape::printInventory - %d\n", NumOfShapes);
	}

private:
	static int NumOfShapes;
	Shape& operator=(const Shape &); // disabled, no implementation provided

protected:
	 int m_id;
};

int Shape::NumOfShapes = 0;


class Circle : public Shape {	
public:
	Circle(double r = 3) : m_radius(r) { 
		std::printf("[%d] Circle::CTOR(double) -> r:%f\n", m_id, m_radius); 
	}

	Circle(const Circle& other)
	: Shape(other), m_radius(other.m_radius) { 
		std::printf("[%d] Circle::CCTOR -> m_radius:%f\n", m_id, m_radius);
	}

	~Circle() { 
		std::printf("[%d] Circle::DTOR ->  m_radius:%f\n", m_id, m_radius); 
	}

	void draw() const { 
		std::printf("[%d] Circle::draw() -> m_radius:%f\n", m_id, m_radius);
	}

	void scale(double f){
		m_radius *= f;
	}
    
	double area()  {
		return m_radius * m_radius * 3.1415;	
	}

	double radius() const { 
		std::printf("[%d] Circle::radius()  -> m_radius:%f\n", m_id, m_radius);
		return m_radius;
	}
	
	double radius(double r) { 
		std::printf("[%d] Circle::radius(%f)\n", m_id, m_radius);
		return m_radius;
	}

private:
	double m_radius;
};

class Rectangle: public Shape {
public:
	Rectangle(int a = 16) : m_a(a), m_b(a) { 
		std::printf("[%d] Rectangle::CTOR(int) -> a:%d/%d\n", m_id, m_a, m_b);
	}

	Rectangle(int a, int b) : m_a(a), m_b(b) { 
		std::printf("[%d] Rectangle::CTOR(int,int) -> a:%d/%d\n", m_id, m_a, m_b);
	}
	
	Rectangle(const Rectangle &other ) : Shape(other),m_a(other.m_a), m_b(other.m_b) { 
		std::printf("[%d] Rectangle::CCTOR -> a:%d/%d\n", m_id, m_a, m_b);
	}
	
	~Rectangle() { 
		std::printf("[%d] Rectangle::DTOR -> a:%d/%d\n", m_id, m_a, m_b);
	}

	void draw() const { 
		std::printf("Rectangle::draw()  - %d, a:%d/%d\n", m_id, m_a, m_b);
	}

    void draw(Color::ColorEnum c) const {
		std::printf("[%d] Rectangle::draw(%d) -> a:%d/%d\n",  m_id, c, m_a, m_b);
	}

	void scale(double f){
		m_a *= f;
		m_b *= f;
	}

	double area(){
		return m_a * m_b;
	}

private:
	int m_a, m_b;
};

void report(const Shape& s) {
	std::puts("-----report-----");
	s.draw(); 
	Shape::printInventory();
}


inline void draw(Shape& obj) { 
	obj.scale(2);
	obj.draw();	
}


void draw(Circle c) { 
	static Circle unit(1);
	
	unit.draw();	
	c.draw(); 
}

void draw(Circle& c, int scale) { 
	if(scale != 0) {
		static Circle unit(1);			
		unit.scale(scale);
		unit.draw();		
	}		
	c.scale(scale);	
}

// Not in phase II

void doPointerArray(){
	Shape *array[] =  {
	    new Circle(),
	    new Rectangle(3),
	    new Circle(4)
	};

    for(int i = 0; i < 3; ++i) 
		array[i]->draw();
	//std::printf("area: %f\n", drawBig(*array[2]));

    for(int i = 0; i < 3; ++i) { 
		delete array[i]; 
	}

}

void doObjArray(){
	Shape objects[] = {
	    Circle(),
	    Rectangle(4),
	    Circle(9)
	};
    for(int i = 0; i < 3; ++i) 
		objects[i].draw();
}


class Empty {
public:
    Empty() { std::puts("Empty()"); }
   ~Empty() { std::puts("~Empty()");}	
};

class EmptyEmpty  : public Empty{
public:
	EmptyEmpty() { std::puts("EmptyEmpty()"); }
};

class EmptyBag : public EmptyEmpty {
	Empty e1;
	Empty e2;
	EmptyEmpty ee;
public: 
	EmptyBag(const Empty& e) : e1(e)
	{
		std::puts("EmptyBag");
	}
};

void empty(EmptyBag bag){
	Empty e;
	// Not in phase II
	//EmptyEmpty ee[10];
		
	EmptyBag eb(Empty());	
}


int main(int argc, char **argv, char **envp)
{
	std::puts("0 ++++++++++++++++++");
    Circle c;
	Rectangle s(4);
	
	draw(c);
	draw(c, 4);
	draw(c, 8);

    draw(s);

	report(c);
	std::puts("1 ++++++++++++++++++");
	
	
	doPointerArray();
	std::puts("2 ++++++++++++++++++");
	
   
    doObjArray();
	std::puts("3 ++++++++++++++++++");

    Shape::printInventory();

    Circle c2(17);
    c2.printInventory();
    
    // Not in phase II
    //Circle olympics[5];
	//drawBig(olympics[1]);

	// Not in phase II
    //Rectangle *fourRectangles = new Rectangle[4];
	//drawBig(*fourRectangles);
    //delete[] fourRectangles;

	std::puts("4 ++++++++++++++++++");
	
	Empty t;
	EmptyBag eb(t);
	empty(eb);
	
	std::puts("5 ++++++++++++++++++");
    return 0;
}

