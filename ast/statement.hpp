/*class Statement: public Node {};


class StatementList: public Statement {
// private:
//   vector<StatementListItem*> items;
// public:
//   StatementList(std::vector<StatementListItem*> *items) : items(items) { };
//   void dump(int indent) {
//     label(indent, "StatementList\n");
//     for (std::vector<StatementListItem*>::iterator iter = items->begin(); iter != items->end(); ++iter)
//       (*iter)->dump(indent+1);
//   }
};
class StatementListItem: public Statement {

}; */

#include <iostream>
#include <vector>


class Statement: public Node { };

class StatementList : public Statement {

private:
	Statement* statement;
public:
    std::vector<const Statement*>* statements;	
    StatementList(const Statement* s)  {
		statements = new std::vector<const Statement*>();
	 	statements->push_back(s);
    }
    StatementList()  {
         statements = new std::vector<const Statement*>();
    }	
    
    
};



