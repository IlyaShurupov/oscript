
#include "compiler.h"

#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>

#include "oscript.h"

#include "oscript_compiler.h"

bool parse_error = false;
bool initialized = false;
lalr::GrammarCompiler* g_compiler = NULL;
lalr::Parser<const char*, code_node*>* parser = NULL;
oscript_compiler* oscr_comp = NULL;

void rep_err(string err_def, const lalr::ParserNode<>* node, bool terminate = true, string tp = "syntax") {
  printf("%s error (%i, %i): %s\n", tp.str, node->line(), node->column(), err_def.str);
  parse_error = true;
  if (terminate) throw 1;
}

void check_err(const lalr::ParserNode<>* node) {
  if (oscr_comp->is_err) {
    rep_err(oscr_comp->get_last_error(), node, true, "compile");
  }
}

// AST nodes

List<struct id_node*> id_nodes;

struct id_node : code_node {
  string val;
  id_node(string val) : val(val) {
    id_nodes.PushBack(this);
  }
  ~id_node() {
  }
};


code_node* create(code_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
  code_node* node = oscr_comp->add_node_create(((id_node*)data[0])->val, ((id_node*)data[1])->val);
  check_err(nodes);
  return node;
}

code_node* destroy(code_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
  code_node* node = oscr_comp->add_node_destroy(((id_node*)data[1])->val);
  check_err(nodes);
  return node;
}

code_node* identifier(code_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
  return new id_node(nodes->lexeme().c_str());
}

// errors

code_node* missed_semicoloumn(code_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
  rep_err("missed ';'", nodes);
  return NULL;
}

code_node* invalid_statement(code_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
  rep_err("invalid statement", nodes);
  return NULL;
}


void osc_init() {

  if (initialized) {
    return;
  }

  string grammar = read_file("A:/src/oscript/rsc/oscript.cfg");
  lalr::ErrorPolicy errp;
  g_compiler = new lalr::GrammarCompiler();

  if (g_compiler->compile(grammar.str, grammar.str + strlen(grammar.str), &errp)) {
    throw "invalid grammar given";
  }
  parser = new lalr::Parser<const char*, code_node*>(g_compiler->parser_state_machine());

  parser->parser_action_handlers()
  ("create", create)
  ("missed_semicoloumn", missed_semicoloumn)
  ("invalid_statement", invalid_statement)
  ("identifier", identifier)
  ("destroy", destroy);

  oscr_comp = new oscript_compiler();

  initialized = true;
}

void osc_compile(code* out, string* oscript) {

  parse_error = false;
  
  try {
    id_nodes.Clear();
    parser->parse(oscript->str, oscript->str + strlen(oscript->str));
    parse_error = !(parser->accepted() && parser->full());
  }
  catch (...) {
    parse_error = true;
  }
  
  if (parse_error) {
    printf("\ncompilation failed\n");
    out->Reserve(1);
    (*out)[0] = { 0,  0, 0, 0, 0, 0, 0, 0, 0 }; // none op
  }

  oscr_comp->compile_code(out);
}

void osc_finalize() {
  delete g_compiler;
  delete parser;
  delete oscr_comp;
}

/*
string string_type = "str";
string int_type = "int";
string method_type = "method";
string list_type = "list";
string save_path = "A:/src/oscript/example/nd.nd";

out->Reserve(16);
(*out)[0] = { OP_CREATE,  0, 0, 0, 0, 0, 0, 0, (alni)(&int_type) }; // my_int = 0;
(*out)[1] = { OP_CREATE,  1, 0, 0, 0, 0, 0, 0, (alni)(&list_type) }; // my_list = [];
(*out)[2] = { OP_CREATE,  2, 0, 0, 0, 0, 0, 0, (alni)(&string_type) }; // save_path = "A:/data/tmp/nd.nd";
(*out)[3] = { OP_SET,     2, 2, 0, 0, 0, 0, 0, (alni)(&save_path) };
(*out)[4] = { OP_CREATE,  3, 0, 0, 0, 0, 0, 0, (alni)(&method_type) }; // def method() {};
(*out)[5] = { OP_CALL,    3, 0, 0, 0, 0, 0, 0, (alni)(MethodObjectType.methods[METHOD_OBJECT_COPILE].adress) }; // method.compile();
(*out)[6] = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress) }; // my_list.push_back(method);
(*out)[7] = { OP_CALL,    1, 0, 3, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress) }; // my_list.push_back(my_int);
(*out)[8] = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_GET_LENGTH].adress) }; // my_int = my_list.len();
(*out)[9] = { OP_SAVE,    1, 2, 0, 0, 0, 0, 0, 0 }; // save(my_list, save_path);
(*out)[10] = { OP_LOAD,    4, 2, 0, 0, 0, 0, 0, 0 }; // loaded_list = load(save_path);
(*out)[11] = { OP_DESTROY, 0, 0, 0, 0, 0, 0, 0, 0 }; //~my_int;
(*out)[12] = { OP_DESTROY, 1, 0, 0, 0, 0, 0, 0, 0 }; // ~my_list;
(*out)[13] = { OP_DESTROY, 2, 0, 0, 0, 0, 0, 0, 0 }; // ~my_list;
(*out)[14] = { OP_DESTROY, 3, 0, 0, 0, 0, 0, 0, 0 }; // ~method;
(*out)[15] = { OP_DESTROY, 4, 0, 0, 0, 0, 0, 0, 0 };
*/