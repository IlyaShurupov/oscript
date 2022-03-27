#pragma once

#include "map.h"
#include "oscript.h"
#include "strings.h"

enum class NodeType {
  NONE,
  CREATE,
  DESTROY,
  BLOCK,
  IF_STM,
};

struct code_node {
  NodeType tp = NodeType::NONE;
  bool stack_use = false;
  virtual instruction compile(struct oscript_compiler* osc, alni inst_idx) {
    return instruction();
  };
};

struct oscript_compiler {
  HashMap<alni, string> otypes;

  list<code_node*> code_nodes;
  HashMap<code_node*, string> identifiers;

  string last_err;
  bool is_err = false;

  oscript_compiler() {

  }

  void compile_code(code* out);
  string* get_last_error();
  void clear_cache();
  void rep_err(string err);

  code_node* add_node_create(string type, string id);
  code_node* add_node_destroy(string id);

  ~oscript_compiler() {}
};