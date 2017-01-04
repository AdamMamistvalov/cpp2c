/*		Adam Mamistvalov						*/
/*		purpose - demonstrate cpp using c code	*/
/*		Last Update : 19.12.16					*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>		/*on_exit func for static units*/

typedef enum __Vtable_Index { DTOR = 0, DRAW, DRAW_COLOR, SCALE, AREA}__Vtable_Index;

typedef enum __Color_ColorEnum { RED = 0, GREEN, DEFAULT }__Color_ColorEnum;

void __Static_Set_Color_(__Color_ColorEnum color)
{		
	static const char *pallete[] = { "\x1B[31m", "\x1B[32m", "\033[0m" };
	puts(pallete[color]);
}

int __Shape_NumOfShapes = 0;		/*counter for the number of shapes in the whole project*/

/*===================================================================================================================================
																SCALEABLE
===================================================================================================================================*/
void __Shape_stat_Print_Inventory()
{
	printf("Shape::printInventory - %d\n", __Shape_NumOfShapes);
}
typedef struct Scaleable
{
	void* (**m_scaleableVTable)(void*);		/*ptr to the vtable*/
}Scaleable;

/*************************************************************************
								  Scaleable DTOR
*************************************************************************/
void __Scaleable_Scaleable_dtor(Scaleable *_this)
{
	puts("~Scaleable");
}

/*************************************************************************
							  Scaleable V Table
*************************************************************************/
void* (*ScaleableVTable[])(void*) = {(void* (*)(void*))__Scaleable_Scaleable_dtor};

/*************************************************************************
								  Scaleable CTOR
*************************************************************************/
void __Scaleable_Scaleable(Scaleable *_this)
{
	_this->m_scaleableVTable = ScaleableVTable;		
}

/*===================================================================================================================================
																SHAPE
===================================================================================================================================*/
typedef struct Shape
{
	Scaleable m_scalableBase;
	int m_id;
}Shape;

void __Shape_Shape_dtor(Shape *_this);	 /*forward decleration for V table*/

/*************************************************************************
								Shape Draw 
*************************************************************************/
void __Shape_Draw(const Shape *_this)
{
	printf("[%d] Shape::draw()\n", _this->m_id);
}

/*************************************************************************
							Shape Draw With Color
*************************************************************************/
void __Shape_Draw_Color(const Shape *_this, __Color_ColorEnum _color)
{
	__Static_Set_Color_(_color);
	printf("[%d] Shape::draw(Color)\n", _this->m_id);
	__Shape_Draw(_this);
	__Static_Set_Color_(DEFAULT);
}

/*************************************************************************
								Shape Scale 
*************************************************************************/
void __Shape_Scale(double f) {}

/*************************************************************************
								Shape Area 
*************************************************************************/
double __Shape_Area(void) 
{
	return -1;
}

/*************************************************************************
							   Shape V Table
*************************************************************************/
void* (*ShapeVTable[])(void*) = 
{
	(void* (*)(void*))__Shape_Shape_dtor, 
	(void* (*)(void*))__Shape_Draw, 
	(void* (*)(void*))__Shape_Draw_Color, 
	(void* (*)(void*))__Shape_Scale, 
	(void* (*)(void*))__Shape_Area
};

/*************************************************************************
								  Shape CTOR
*************************************************************************/
void __Shape_Shape_def(Shape *_this)
{
	__Scaleable_Scaleable(&_this->m_scalableBase);
	_this->m_scalableBase.m_scaleableVTable = ShapeVTable;
	/*MIL*/
	_this->m_id = ++__Shape_NumOfShapes;
	/*Body*/
	printf("[%d] Shape::CTOR\n", _this->m_id);
}

/*************************************************************************
								Shape Copy CTOR
*************************************************************************/
void __Shape_Shape_Copy(Shape *_this, Shape *_source)
{
	__Scaleable_Scaleable(&_this->m_scalableBase);
	_this->m_scalableBase.m_scaleableVTable = ShapeVTable;  
	/*MIL*/
	_this->m_id = ++__Shape_NumOfShapes;
	/*Body*/
	printf("[%d] Shape::CCTOR from [%d]\n", _this->m_id, _source->m_id);
}

/*************************************************************************
								  Shape DTOR
*************************************************************************/
void __Shape_Shape_dtor(Shape *_this)
{
	((void (*)(Shape*))(_this->m_scalableBase.m_scaleableVTable[DRAW]))(_this);
	
	printf("[%d] Shape::DTOR\n", _this->m_id);
	--__Shape_NumOfShapes;
	
	_this->m_scalableBase.m_scaleableVTable = ScaleableVTable;	/*reset the Vtable to the base*/
	__Scaleable_Scaleable_dtor((Scaleable*)_this);
}

/*===================================================================================================================================
																 CIRCLE
===================================================================================================================================*/
typedef struct Circle
{
	Shape m_shapeBase;
	double m_radius;
}Circle;

/*************************************************************************
								Circle Draw 
*************************************************************************/
void __Circle_Draw(const Circle *_this)
{
	printf("[%d] Circle::draw() -> m_radius:%f\n", _this->m_shapeBase.m_id, _this->m_radius);
}

/*************************************************************************
								Circle Scale 
*************************************************************************/
void __Circle_Scale(Circle *_this, double _radius)
{
	_this->m_radius *= _radius;
}

/*************************************************************************
								Circle Area 
*************************************************************************/
double __Circle_Area(Circle *_this)
{
	return _this->m_radius * _this->m_radius * 3.1415;
}

/*************************************************************************
								Circle Radius 
*************************************************************************/
double __Circle_Radius(const Circle *_this) 
{
	printf("[%d] Circle::radius()  -> m_radius:%f\n", _this->m_shapeBase.m_id, _this->m_radius);
	return _this->m_radius;
}

/*************************************************************************
						  Circle Radius From Double
*************************************************************************/
double __Circle_Radius_d(Circle *_this, double f)	
{
	printf("[%d] Circle::radius(%f)\n",  _this->m_shapeBase.m_id, _this->m_radius);
	return _this->m_radius;
}

/*************************************************************************
								  Circle DTOR
*************************************************************************/
void __Circle_Circle_dtor(Circle *_this)
{
	/*Body*/
	printf("[%d] Circle::DTOR ->  m_radius:%f\n",  _this->m_shapeBase.m_id, _this->m_radius); 
	/*call Base DTOR*/
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = ShapeVTable;	/*reset the Vtable to the base*/
	__Shape_Shape_dtor((Shape*)_this);
}

/*************************************************************************
							   Circle V Table
*************************************************************************/
void* (*CircleVTable[])(void*) = 
{
	(void* (*)(void*))__Circle_Circle_dtor, 
	(void* (*)(void*))__Circle_Draw, 
	(void* (*)(void*))__Shape_Draw_Color, 
	(void* (*)(void*))__Circle_Scale, 
	(void* (*)(void*))__Circle_Area
};

/*************************************************************************
							 Circle CTOR From Int
*************************************************************************/
void __Circle_Circle_i(Circle *_this, double r) /*r = 3 default*/
{	
	__Shape_Shape_def((Shape*)_this);
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = CircleVTable;
	/*MIL*/
	_this->m_radius = r;
	/*Body*/
	printf("[%d] Circle::CTOR(double) -> r:%f\n", _this->m_shapeBase.m_id, _this->m_radius); 
}

/*************************************************************************
							   Circle Copy CTOR
*************************************************************************/
void __Circle_Circle_Copy(Circle *_this, const Circle *_source)	
{
	__Shape_Shape_Copy((Shape*)_this, (Shape*)_source);
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = CircleVTable;
	/*MIL*/
	_this->m_radius = _source->m_radius;
	/*Body*/
	printf("[%d] Circle::CCTOR -> m_radius:%f\n", _this->m_shapeBase.m_id, _this->m_radius);
}

/*===================================================================================================================================
																 RECTANGLE
===================================================================================================================================*/
typedef struct Rectangle
{
	Shape m_shapeBase;
	int m_a, m_b;
}Rectangle;

/*************************************************************************
								Rectangle Draw 
*************************************************************************/
void __Rectangle_Draw(const Rectangle *_this)
{
	printf("Rectangle::draw()  - %d, a:%d/%d\n", _this->m_shapeBase.m_id, _this->m_a, _this->m_b);
}

/*************************************************************************
							 Rectangle Draw Color
*************************************************************************/
void __Rectangle_Draw_Color(const Rectangle *_this, __Color_ColorEnum _color)
{
	printf("[%d] Rectangle::draw(%d) -> a:%d/%d\n",  _this->m_shapeBase.m_id, _color, _this->m_a, _this->m_b);
}

/*************************************************************************
							 Rectangle Scale
*************************************************************************/
void __Rectangle_Scale(Rectangle *_this, double _f)
{
	_this->m_a *= _f;
	_this->m_b *= _f;
}

/*************************************************************************
							 Rectangle Area
*************************************************************************/
double __Rectangle_Area(Rectangle *_this)
{
	return _this->m_a * _this->m_b;
}

/*************************************************************************
								  Rectangle DTOR
*************************************************************************/
void __Rectangle_Rectangle_dtor(Rectangle *_this)
{
	/*Body*/
	printf("[%d] Rectangle::DTOR -> a:%d/%d\n", _this->m_shapeBase.m_id, _this->m_a, _this->m_b);
	/*call Base DTOR*/
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = ShapeVTable;	/*reset the Vtable to the base*/
	__Shape_Shape_dtor((Shape*)_this);
}

/*************************************************************************
							   Rectangle V Table
*************************************************************************/
void* (*RectangleVTable[])(void*) = 
{
	(void* (*)(void*))__Rectangle_Rectangle_dtor,
	(void* (*)(void*))__Rectangle_Draw, 
	(void* (*)(void*))__Rectangle_Draw_Color, 
	(void* (*)(void*))__Rectangle_Scale, 
	(void* (*)(void*))__Rectangle_Area
};

/*************************************************************************
							 Rectangle CTOR From Int
*************************************************************************/
void __Rectangle_Rectangle_i(Rectangle *_this, int _num) 
{
	__Shape_Shape_def(&_this->m_shapeBase);
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = RectangleVTable;
	/*MIL*/
	_this->m_a = _num;
	_this->m_b = _num;
	/*Body*/
	printf("[%d] Rectangle::CTOR(int) -> a:%d/%d\n", _this->m_shapeBase.m_id, _this->m_a, _this->m_b);
}
/*************************************************************************
							Rectangle CTOR From 2 Int
*************************************************************************/
void __Rectangle_Rectangle_2i(Rectangle *_this, int _num1, int _num2)
{
	__Shape_Shape_def((Shape*)_this);
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = RectangleVTable;
	/*MIL*/
	_this->m_a = _num1;
	_this->m_b = _num2;
	/*Body*/
	printf("[%d] Rectangle::CTOR(int,int) -> a:%d/%d\n", _this->m_shapeBase.m_id, _this->m_a, _this->m_b);
}

/*************************************************************************
							   Rectangle Copy CTOR
*************************************************************************/
void __Rectangle_Rectangle_Copy(Rectangle *_this, Rectangle *_source)
{
	__Shape_Shape_Copy((Shape*)_this, (Shape*)_source);
	_this->m_shapeBase.m_scalableBase.m_scaleableVTable = RectangleVTable;
	/*MIL*/
	_this->m_a = _source->m_a;
	_this->m_b = _source->m_b;
	/*Body*/
	printf("[%d] Rectangle::CCTOR -> a:%d/%d\n", _this->m_shapeBase.m_id, _this->m_a, _this->m_b);
}

/*===================================================================================================================================
															 STATIC FUNC
===================================================================================================================================*/
/*************************************************************************
								  Report
*************************************************************************/
void report(const Shape *s) 
{
	puts("-----report-----");
	((void (*)(Shape*))(s->m_scalableBase.m_scaleableVTable[DRAW]))((Shape*)s);		/*__Shape_Draw(s);*/
	__Shape_stat_Print_Inventory();
}

/*************************************************************************
								  Draw Shape
*************************************************************************/
void __Draw_Shape(Shape *obj)
{
	((void (*)(Shape*, double))(obj->m_scalableBase.m_scaleableVTable[SCALE]))(obj, 2);
	((void (*)(Shape*))(obj->m_scalableBase.m_scaleableVTable[DRAW]))(obj);
}

/*************************************************************************
							  Static Draw DTOR 
*************************************************************************/
static void __Static_Circle_dtor(int _first, void *_deleteCirc)
{
	__Circle_Circle_dtor((Circle*)_deleteCirc);
}

/*************************************************************************
								  Draw Circle
*************************************************************************/
void __Draw_Circle(Circle *c)
{ 
	static int flagForStaticDraw;							/*static flag in order to do CTOR only once if the func is called*/
	static Circle unit1;																									
	if (0 == flagForStaticDraw)											
	{
		__Circle_Circle_i(&unit1, 1);
		flagForStaticDraw = 1;	
		on_exit(__Static_Circle_dtor, (void*)&unit1);		/*when exit main, need to DTOR this Circle*/
	}
	__Circle_Draw(&unit1);
	__Circle_Draw(c);
}

/*************************************************************************
						  Draw Circle With Scale
*************************************************************************/
void __Draw_Circle_Scale(Circle *c, int scale) 
{ 
	static int flagForStaticScale;							/*static flag in order to do CTOR only once if the func is called*/
	static Circle unit2;	
	if(scale != 0) 
	{
		if (0 == flagForStaticScale)
		{
			__Circle_Circle_i(&unit2, 1);
			flagForStaticScale = 1;
			on_exit(__Static_Circle_dtor, (void*)&unit2);	/*when exit main, need to DTOR this Circle*/
		}
		__Circle_Scale(&unit2, scale);
		__Circle_Draw(&unit2);		
	}	
	__Circle_Scale(c, scale);
}

/*===================================================================================================================================
															  EMPTY
===================================================================================================================================*/
typedef struct Empty
{
	char m_sizeOnly;	/*the size of an empty class in cpp is minimum 1 - as char here*/
}Empty;

/*************************************************************************
						  		Empty CTOR
*************************************************************************/
void __Empty_ctor(Empty *_this)
{
	puts("Empty()");
}

/*************************************************************************
						  		Empty Copy CTOR
*************************************************************************/
void __Empty_Copy_ctor(Empty *_this, const Empty *_source)
{
	_this = (Empty*)_source;
}

/*************************************************************************
						  		Empty DTOR
*************************************************************************/
void __Empty_dtor(Empty *_this)
{
	puts("~Empty()");
}

/*===================================================================================================================================
															  EMPTYEMPTY
===================================================================================================================================*/
typedef struct EmptyEmpty
{
	Empty m_emptyBase;
}EmptyEmpty;

/*************************************************************************
						  		EmptyEmpty CTOR
*************************************************************************/
void __EmptyEmpty_ctor(EmptyEmpty *_this)
{
	__Empty_ctor(&_this->m_emptyBase);
	puts("EmptyEmpty()");
}

/*************************************************************************
						  		EmptyEmpty DTOR
*************************************************************************/
void __EmptyEmpty_dtor(EmptyEmpty *_this)
{
	__Empty_dtor(&_this->m_emptyBase);
}

/*===================================================================================================================================
															  EMPTY BAG
===================================================================================================================================*/
typedef struct EmptyBag
{
	EmptyEmpty m_emptyEmptyBase;
	Empty m_e1;
	Empty m_e2;
	EmptyEmpty m_ee;
}EmptyBag;

/*************************************************************************
						  		Empty Bag CTOR
*************************************************************************/
void __EmptyBag_ctor(EmptyBag *_this)
{
	__EmptyEmpty_ctor(&_this->m_emptyEmptyBase);
	
	__Empty_ctor(&_this->m_e1);
	__Empty_ctor(&_this->m_e2);
	__EmptyEmpty_ctor(&_this->m_ee);
}

/*************************************************************************
						   Empty Bag CTOR From Empty
*************************************************************************/
void __EmptyBag_ctor_Empty(EmptyBag *_this, const Empty *_e)
{
	__EmptyEmpty_ctor(&_this->m_emptyEmptyBase);

	__Empty_Copy_ctor(&_this->m_e1, (Empty*)_e);
	__Empty_ctor(&_this->m_e2);
	
	__EmptyEmpty_ctor(&_this->m_ee);
	puts("EmptyBag");
}

/*************************************************************************
						   Empty Bag DTOR 
*************************************************************************/
void __EmptyBag_dtor(EmptyBag *_this)
{
	__EmptyEmpty_dtor(&_this->m_ee);
	__Empty_dtor(&_this->m_e2);
	__Empty_dtor(&_this->m_e1);
	
	__EmptyEmpty_dtor(&_this->m_emptyEmptyBase);
}

/*************************************************************************
						  	Empty Bag Copy CTOR
*************************************************************************/
void __EmptyBag_Copy_ctor(EmptyBag *_this, const EmptyBag *_source)
{	
	memcpy(_this, _source, sizeof(EmptyBag));
}
/*===================================================================================================================================
															  STATIC FUNCS
===================================================================================================================================*/
void __Empty_From_Empty_Bag(EmptyBag *_bag)
{
	Empty e;
	__Empty_ctor(&e);
	
	EmptyBag eb(Empty());
	
	__Empty_dtor(&e);	
}

void __do_Ptr_Array()
{
	Shape* objects[3];
	Circle tempCircleFor0;
	Rectangle tempRectangleFor1;
	Circle tempCircleFor2;
	int index = 0;
	/*creating first shape ptr*/
	__Circle_Circle_i(&tempCircleFor0, 3);
	objects[0] = (Shape*)&tempCircleFor0;
	/*creating second shape ptr*/
	__Rectangle_Rectangle_i(&tempRectangleFor1, 3);
	objects[1] = (Shape*)&tempRectangleFor1;
	/*creating third shape ptr*/
	__Circle_Circle_i(&tempCircleFor2, 4);
	objects[2] = (Shape*)&tempCircleFor2;
	/*draw func*/
	for (index = 0; index < 3; ++index)
	{	
		((void (*)(Shape*))(objects[index]->m_scalableBase.m_scaleableVTable[DRAW]))(objects[index]);
	}
	/*dtor*/
	for (index = 0; index < 3; ++index)
	{	
		((void (*)(Shape*))(objects[index]->m_scalableBase.m_scaleableVTable[DTOR]))(objects[index]);
	}
}


void __do_Obj_Array()
{
	Shape objects[3];
	Circle tempCircleFor0;
	Rectangle tempRectangleFor1;
	Circle tempCircleFor2;
	int _index = 0;
	/*creating first shape*/
	__Circle_Circle_i(&tempCircleFor0, 3);
	__Shape_Shape_Copy(&objects[0], (Shape*)&tempCircleFor0);
	__Circle_Circle_dtor(&tempCircleFor0);
	/*creating second shape*/
	__Rectangle_Rectangle_i(&tempRectangleFor1, 4);
	__Shape_Shape_Copy(&objects[1], (Shape*)&tempRectangleFor1);
	__Rectangle_Rectangle_dtor(&tempRectangleFor1);
	/*creating third shape*/
	__Circle_Circle_i(&tempCircleFor2, 9);
	__Shape_Shape_Copy(&objects[2], (Shape*)&tempCircleFor2);
	__Circle_Circle_dtor(&tempCircleFor2);	
	/*draw func*/
	for (; _index < 3; _index++)
	{
		((void (*)(Shape*))(objects[_index].m_scalableBase.m_scaleableVTable[DRAW]))(&objects[_index]);
	} 
	/*dtors*/
	__Shape_Shape_dtor(&objects[2]);	/*objects are destructed in opposit order*/
	__Shape_Shape_dtor(&objects[1]);
	__Shape_Shape_dtor(&objects[0]);	
}


int main()
{
	puts("0 ++++++++++++++++++");
	Circle c;
	__Circle_Circle_i(&c,3);
	
	Rectangle s;
	__Rectangle_Rectangle_i(&s,4);

	Circle tempForDrawCircle;							/*  creates copy CTOR bc the func get by value	*/
	__Circle_Circle_Copy(&tempForDrawCircle, &c);		/*						"						*/
	__Draw_Circle(&tempForDrawCircle);					/*			the func that get by value			*/
	__Circle_Circle_dtor(&tempForDrawCircle);			/*  destroy the temp obj at the end of the func	*/
	
	__Draw_Circle_Scale(&c, 4);
	__Draw_Circle_Scale(&c, 8);
	
	__Draw_Shape((Shape*)&s);
	
	report((const Shape*)&c);
	puts("1 ++++++++++++++++++");
	
	__do_Ptr_Array();
	puts("2 ++++++++++++++++++");

	__do_Obj_Array();

	puts("3 ++++++++++++++++++");
	__Shape_stat_Print_Inventory();

	Circle c2;
	__Circle_Circle_i(&c2,17);
	
	__Shape_stat_Print_Inventory();
	
	puts("4 ++++++++++++++++++");
	Empty t;
	__Empty_ctor(&t);

	EmptyBag eb;
	__EmptyBag_ctor_Empty(&eb, &t);
	
	EmptyBag tempEB;						/*  creates copy CTOR bc the func get by value	*/
	__EmptyBag_Copy_ctor(&tempEB, &eb);		/*						"						*/
	__Empty_From_Empty_Bag(&tempEB);		/*			the func that get by value			*/
	__EmptyBag_dtor(&tempEB);				/*  destroy the temp obj at the end of the func	*/
	
	puts("5 ++++++++++++++++++");
	
	__EmptyBag_dtor(&eb);
	__Empty_dtor(&t);

	__Circle_Circle_dtor(&c2);
	
	__Rectangle_Rectangle_dtor(&s);
	__Circle_Circle_dtor(&c);
	
	return 0;
}





