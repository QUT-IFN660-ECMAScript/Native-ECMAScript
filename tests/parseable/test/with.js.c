#include "./runtime/core.hpp"
#include "./runtime/console.hpp"
#include "./scope/reference.hpp"
	ESObject* globalObj = new ESObject();
int main() {
	ESValue* r0 = new Reference(new String("radius"));
	ESValue* r1 = new Number(10);
	ESValue* r2 = Core::assign(r0, r1);
	return 0;
}
