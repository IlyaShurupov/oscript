#pragma once

#include "list.h"
#include "map.h"

#include "treecode.h"

namespace osc {

	namespace listcode {

		struct instruction {
			inst::itype tp = inst::itype::NONE;
			enum class none_inst_type { WHILELOOP, NONE } none_tp = none_inst_type::NONE;
		};

		struct object_definition {
			tp::string name;
			instruction* inst = NULL;
		};

		struct scope {
			scope_node* tc_scope = NULL;
			tp::HashMap<object_definition, tp::string> locals;
			tp::alni locals_idx_start = 0;

			bool define(tp::string name, instruction* inst) {
				auto idx = locals.presents(name);
				if (idx) {
					return false;
				}
				locals.put(name, {name, inst});
				return true;
			}
		};

		struct create_inst : public instruction {
			bool temp = false;
			tp::uint1 temp_local_idx = 0;
			create_inst() {
				tp = inst::itype::CREATE;
			}
		};

		struct delete_inst : public instruction {
			bool temp = false;
			tp::uint1 temp_local_idx = 0;
			delete_inst() {
				tp = inst::itype::DESTROY;
			}
		};

		struct jump_ifnot_inst : public instruction {
			instruction* next_inst = NULL;
			bool after = false;
			tp::uint1 local_boolean_idx = 0;

			jump_ifnot_inst() {
				tp = inst::itype::JUMPIFNOT;
			}
		};

		struct jump_inst : public instruction {
			instruction* next_inst = NULL;
			bool after = false;
			bool to_the_end = false;
			jump_inst() {
				tp = inst::itype::JUMP;
			}
		};

		struct whileloop_inst : public instruction {
			instruction* loop_end = NULL;
			whileloop_inst() {
				tp = inst::itype::NONE;
				none_tp = instruction::none_inst_type::WHILELOOP;
			}
		};

		struct return_val_inst : public instruction {
			tp::uint1 local_idx = 0;
			return_val_inst() {
				tp = inst::itype::RET;
			}
		};

		struct listcode {
			tp::uint1 current_line_ntemps = 0;
			tp::uint1 current_defined_locals = 0;
			bool last_expr_was_define = false;
			tp::Stack<scope*> scope_stack;
			tp::List<instruction*> instructions;

			scope* enter_scope(scope_node* tree_node) {
				scope* child_scope = new scope();
				child_scope->locals_idx_start = current_defined_locals;
				child_scope->tc_scope = tree_node;
				scope_stack.push(child_scope);
				return child_scope;
			}

			void leave_sope() {
				scope* current_scope = scope_stack.last->data;
				for (tp::uint1 local_idx = current_scope->locals_idx_start; local_idx < current_defined_locals; local_idx++) {
					add_idelete(local_idx, false);
				}
				delete current_scope;
				scope_stack.pop();
				scope_stack.pop();
			}

			void new_statement() {
				current_line_ntemps = 0;
			}

			void statement_end() {
				for (tp::uint1 temp_idx = 0; temp_idx < current_line_ntemps; temp_idx++) {
					add_idelete(temp_idx, true);
				}
			}

			bool add_icreate(bool temp, tp::string name = "", tp::string type = "") {
				create_inst* create = new create_inst();
				if (temp) {
					create->temp = true;
					create->temp_local_idx = current_line_ntemps;
					current_line_ntemps++;
				} else {
					if (!scope_stack.last->data->define(name, create)) {
						delete create;
						return false;
					}
					current_defined_locals++;
					return true;
				}
				return false;
			}

			void add_idelete(tp::uint1 local_idx, bool temp) {
				delete_inst* deli = new delete_inst();
				deli->temp = temp;
				deli->temp_local_idx = local_idx;
				instructions.pushBack(deli);
			}

			jump_ifnot_inst* add_ijump_ifnot(tp::uint1 local_idx) {
				jump_ifnot_inst* jumpi = new jump_ifnot_inst();
				jumpi->local_boolean_idx = local_idx;
				instructions.pushBack(jumpi);
				return jumpi;
			}

			jump_inst* add_ijump() {
				jump_inst* jumpi = new jump_inst();
				instructions.pushBack(jumpi);
				return jumpi;
			}

			void add_ireturn(tp::uint1 local_idx) {
				return_val_inst* retvali = new return_val_inst();
				retvali->local_idx = local_idx;
				instructions.pushBack(retvali);
			}

			~listcode() {
				assert(!scope_stack.length);
				for (auto inst : instructions) {
					delete inst.Data();
				}
			}
		};
	};
};