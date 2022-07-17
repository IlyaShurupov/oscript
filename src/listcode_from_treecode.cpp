
#include "treecode.h"
#include "listcode.h"

namespace osc {

	void treecode::evaluate(listcode::listcode* out) {
		entry.evaluate(out);
	}

	void function_node::evaluate(listcode::listcode* func) {
		func->enter_scope(NULL);

		for (auto farg : args) {
			bool suc = func->scope_stack.last->data->define(farg.data().name.id, NULL);
			assert(suc);
		}

		this->body.evaluate(func);

		func->leave_sope();
	}

	void scope_node::evaluate(listcode::listcode* out) {
		out->enter_scope(this);

		for (auto cb_iter : code) {
			out->new_statement();
			code_block& cb = cb_iter.data();
			cb.evaluate(out);
			out->statement_end();
		}

		out->leave_sope();
	}

	void code_block::evaluate(listcode::listcode* out) {
		switch (tp) {
			case code_block::type::EXPR:
				expr.evaluate(out);
				break;
			case code_block::type::IF:
				ifelse.evaluate(out);
				break;
			case code_block::type::WHILE:
				whileloop.evaluate(out);
				break;
			case code_block::type::SCOPE:
				scope->evaluate(out);
				break;
		}
	}

	void while_loop_node::evaluate(listcode::listcode* out) {

		listcode::whileloop_inst* whileloop_i = new listcode::whileloop_inst();
		out->instructions.pushBack(whileloop_i);

		listcode::jump_ifnot_inst* jumpifnot = out->add_ijump_ifnot(expr.evaluate(out));
		scope->evaluate(out);
		listcode::jump_inst* jumpi = out->add_ijump();
		jumpi->next_inst = jumpifnot;
		jumpifnot->next_inst = jumpi;
		jumpifnot->after = true;

		whileloop_i->loop_end = jumpi;
	}

	void ifelse_node::evaluate(listcode::listcode* out) {
		expr.evaluate(out);

		listcode::jump_ifnot_inst* jumpifnot = out->add_ijump_ifnot(expr.evaluate(out));

		scope->evaluate(out);

		listcode::jump_inst* jumpi = NULL;
		if (else_scope) {
			jumpi = out->add_ijump();
		}

		jumpifnot->next_inst = out->instructions.last()->data;
		jumpifnot->after = true;

		if (jumpi) {
			else_scope->evaluate(out);
			jumpi->next_inst = out->instructions.last()->data;
			jumpi->after = true;
		}
	}

	// returns expresion local idx
	tp::uint1 expr_node::evaluate(listcode::listcode* out_p) {
		switch (tp) {
			case expr_node::type::CREATION:
				create.evaluate(out_p);
				return out_p->current_defined_locals;

			case expr_node::type::ARIPHM:
				ariphm.evaluate(out_p);
				return out_p->current_line_ntemps;

			case expr_node::type::BOOL_ARIPHM:
				boolean_ariphm.evaluate(out_p);
				return out_p->current_line_ntemps;

			case expr_node::type::CALL:
				call.evaluate(out_p);
				return out_p->current_line_ntemps;

			case expr_node::type::GET:
				get.evaluate(out_p);
				return out_p->current_line_ntemps;

			case expr_node::type::CFLOW:
				cflow.evaluate(out_p);
				break;
		}
		return -1;
	}

	void control_flow_expr_node::evaluate(listcode::listcode* out) {
		switch (tp) {
			case control_flow_expr_node::type::RETURN:
			{
				listcode::jump_inst* jumpi = out->add_ijump();
				jumpi->to_the_end = true;
				return;
			}
			case control_flow_expr_node::type::RETURN_NONE:
			{
				out->add_ireturn(expr->evaluate(out));
			}
			case control_flow_expr_node::type::BREAK:
			{
				for (tp::ListNode<listcode::instruction*>* iter = out->instructions.last(); iter; iter = iter->prev) {
					if (iter->data->none_tp == listcode::instruction::none_inst_type::WHILELOOP) {
						// after while loop
						listcode::whileloop_inst* whinst = (listcode::whileloop_inst*) iter->data;
						assert(whinst->loop_end && "break is outside while loop");
					}
				}
				break;
			}
		}
		assert(0 && "unhandled control flow");
	}

	void get_expr_node::evaluate(listcode::listcode* out) {

	}

	void creation_expr_node::evaluate(listcode::listcode* out) {
		if (raw) {
			out->add_icreate(true);
		} else {
			out->add_icreate(false, name.id, type.id);
		}
	}

	void call_expr_node::evaluate(listcode::listcode* out) {

	}

	void boolean_ariphmetic_expr_node::evaluate(listcode::listcode* out) {

	}

	void ariphmetic_expr_node::evaluate(listcode::listcode* out) {

	}
};