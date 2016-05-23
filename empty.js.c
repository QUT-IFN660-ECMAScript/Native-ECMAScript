#include "./runtime/core.hpp"
#include "./runtime/console.hpp"
#include "./scope/reference.hpp"
ESObject* globalObj = new ESObject();

int main() {
	ESObject* globalObj = new ESObject();
	ESValue* r0 = new Reference(new String("x"));
	ESValue* r1 = new Reference(new String("y"));
	ESValue* r2 = new Number(5);
	ESValue* r3 = Core::plus(r1, r2);
	ESValue* r4 = Core::assign(r0, r3);
	return 0;
}
