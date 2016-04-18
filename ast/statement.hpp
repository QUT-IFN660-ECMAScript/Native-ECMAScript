class StatementList: public Node {
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
class StatementListItem: public Node {

};
