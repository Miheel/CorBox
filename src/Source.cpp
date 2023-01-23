#include "container/array.hpp"
#include "uniquePtr.hpp"
#include "utility.hpp"
#include "time/timer.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <memory>
#include "memory.hpp"
#include <list>
#include "container/linked_list.hpp"
#include <initializer_list>

// visual studio release and debug mode flag
#if _DEBUG
#include <crtdbg.h>
#endif

#undef new
#include "MemoryLeakChecker.hpp"

struct A
{
	A(int &&n) { std::cout << "rvalue overload, n=" << n << "\n"; }
	A(int &n) { std::cout << "lvalue overload, n=" << n << "\n"; }
};

class B
{
public:
	template <class T1, class T2, class T3>
	B(T1 &&t1, T2 &&t2, T3 &&t3) : a1_{cor::forward<T1>(t1)},
								   a2_{cor::forward<T2>(t2)},
								   a3_{cor::forward<T3>(t3)}
	{
	}

private:
	A a1_, a2_, a3_;
};

template <class T, class U>
std::unique_ptr<T> make_unique1(U &&u)
{
	return std::unique_ptr<T>(new T(cor::forward<U>(u)));
}

template <class T, class... U>
std::unique_ptr<T> make_unique2(U &&...u)
{
	return std::unique_ptr<T>(new T(cor::forward<U>(u)...));
}

struct Car
{
	Car(double speed, size_t age, const char *name)
		: speed(speed), age(age), name(name) {}
	Car() {}
	~Car() = default;
	double speed;
	size_t age;
	const char *name;
};

std::ostream &operator<<(std::ostream &out, const Car &car)
{
	return out << "name: " << car.name << " age: " << car.age << " speed: " << car.speed;
}

void print(Car &car)
{
	std::cout << "name: " << car.name << " age: " << car.age << " speed: " << car.speed << "\n";
}

int main()
{

#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::cout << "Debug\n";
#else
	std::cout << "Release\n";
#endif
	{
		// std::list<int> list1;
		//cor::Linked_list<Car> list;
		cor::size_ty size = 5;
		cor::Array<Car> arr1;
		cor::Array<Car> arr2(size);
		cor::Array<Car> buf3;

		for (size_t i = 0; i < size + 2; i++)
		{
			Car car = {i * 1.5, i, "car name"};
			arr1.pushBack(cor::isMovable(car));
			// list.push_back(car);
		}
		Car car = {2 * 1.5, 32, "ford"};
		//cor::Linked_list<Car> l2;
		// l2 = cor::isMovable(list);
		// list.add_node(list[4]);
		// std::cout << std::boolalpha << "Cycle detected: " << cor::cycleDetect(&list);

		// Arr test*************************
		arr1.pushBack(car);
		// buf3.pushBack(car);
		std::cout << "arr1\n";
		for (size_t i = 0; i < arr1.size(); i++)
		{
			print(arr1[i]);
		}
		arr2 = cor::isMovable(arr1);

		std::cout << "arr2\n";
		for (size_t i = 0; i < arr2.size(); i++)
		{
			print(arr2[i]);
		}

		std::cout << "cap: " << arr2.capacity() << " size: " << arr2.size() << "\n";
		arr2.reserv(15);
		arr2.pushBack(car);
		arr2.pushBack(car);
		arr2.pushBack(car);
		std::cout << "cap: " << arr2.capacity() << " size: " << arr2.size() << "\n";
		// arr2.shrinkToFit();
		std::cout << "cap: " << arr2.capacity() << " size: " << arr2.size() << "\n";
		for (size_t i = 0; i < arr2.size(); i++)
		{
			print(arr2[i]);
		}

		arr2.clear();
		std::cout << "cap: " << arr2.capacity() << " size: " << arr2.size() << "\n";

		 //auto p1 = make_unique1<A>(2); // rvalue
		 //int i = 1;
		 //auto p2 = make_unique1<A>(i); // lvalue
		
		 //std::cout << "B\n";
		 //auto t = make_unique2<B>(2, i, 3);
		 //t.get();
		
		 //int* newint = &i;
		 //auto u = cor::UniquePtr<int>(newint);
		 //auto ug = *u.get();
		 //auto ud = *u;
		
		 //const int a = 23;
		
		 //auto alloc = cor::mem::allocate<Car>(Car(60, 6, "ford"));
		
		 //auto alloci = cor::mem::allocate<int>(a);
		
		 //std::cout << *alloc << "\n";
		
		 //cor::mem::deallocate(alloc);
		 //cor::mem::deallocate(alloci);
	}
	MemoryLeakChecker::DumpInformation(true);
	
	//  std::locale swedish("swedish");
	//  std::locale::global(swedish);
	//  std::string str("������");
	//  std::string temp1(str.size(), 'x');
	//  std::string temp2(str.size(), 'x');
	//  std::cout << "Unchanged: " << str << "\n";
	//  cor::transform(str.begin(), str.end(), str.begin(), ::tolower);
	//  std::cout << "Tolower: " << str << "\n";
	//  cor::transform(str.begin(), str.end(), str.begin(), ::toupper);
	//  std::cout << "Toupper: " << str << "\n";

	return 0;
}
