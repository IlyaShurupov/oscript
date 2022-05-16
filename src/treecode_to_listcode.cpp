
#include "treecode.h"
#include "listcode.h"

using namespace oscript;


void irep_error::ensure(bool expr, const string& def) const {
	if (!expr) {
		rterror((string("oscript parser error - ") + def).cstr());
	}
}

// at this point we have full treecode constucted and ready to generate the actual code
void oscript::treecode::evaluate(listcode::listcode* out) {
	entry.evaluate(out);
}

void oscript::function_node::evaluate(listcode::listcode* func) {
	func->enter_scope(NULL);

	for (auto farg : args) {
		bool suc = func->scope_stack.last->data->define(farg.Data().name.id, NULL);
		assert(suc);
	}

	this->body.evaluate(func);

	func->leave_sope();
}

void oscript::scope_node::evaluate(listcode::listcode* out) {
	out->enter_scope(this);

	for (auto cb_iter : code) {
		out->new_statement();
		code_block& cb = cb_iter.Data();
		cb.evaluate(out);
	}

	out->leave_sope();
}

void oscript::code_block::evaluate(listcode::listcode* out) {
	switch (tp) {
		case oscript::code_block::type::EXPR:
			expr.evaluate(out);
			break;
		case oscript::code_block::type::IF:
			ifelse.evaluate(out);
			break;
		case oscript::code_block::type::WHILE:
			whileloop.evaluate(out);
			break;
		case oscript::code_block::type::SCOPE:
			scope->evaluate(out);
			break;
	}
}

void oscript::while_loop_node::evaluate(listcode::listcode* out) {

	listcode::whileloop_inst* whileloop_i = new listcode::whileloop_inst();
	out->instructions.PushBack(whileloop_i);
	
	listcode::jump_ifnot_inst* jumpifnot = out->add_ijump_ifnot(expr.evaluate(out));
	scope->evaluate(out);
	listcode::jump_inst* jumpi = out->add_ijump();
	jumpi->next_inst = jumpifnot;
	jumpifnot->next_inst = jumpi;
	jumpifnot->after = true;

	whileloop_i->loop_end = jumpi;
}

void oscript::ifelse_node::evaluate(listcode::listcode* out) {
	expr.evaluate(out);

	listcode::jump_ifnot_inst* jumpifnot = out->add_ijump_ifnot(expr.evaluate(out));

	scope->evaluate(out);

	listcode::jump_inst* jumpi;
	if (else_scope) {
		jumpi = out->add_ijump();
	}

	jumpifnot->next_inst = out->instructions.Last()->data;
	jumpifnot->after = true;

	if (else_scope) {
		else_scope->evaluate(out);
		jumpi->next_inst = out->instructions.Last()->data;
		jumpi->after = true;
	}
}

// returns expresion local idx
uint1 oscript::expr_node::evaluate(listcode::listcode* out_p) {
	switch (tp) {
		case oscript::expr_node::type::CREATION:
			create.evaluate(out_p);
			return out_p->current_defined_locals;

		case oscript::expr_node::type::ARIPHM:
			ariphm.evaluate(out_p);
			return out_p->current_line_ntemps;
			
		case oscript::expr_node::type::BOOL_ARIPHM:
			boolean_ariphm.evaluate(out_p);
			return out_p->current_line_ntemps;

		case oscript::expr_node::type::CALL:
			call.evaluate(out_p);
			return out_p->current_line_ntemps;
			
		case oscript::expr_node::type::GET:
			get.evaluate(out_p);
			return out_p->current_line_ntemps;

		case oscript::expr_node::type::CFLOW:
			cflow.evaluate(out_p);
			break;
	}
	return -1;
}

void oscript::control_flow_expr_node::evaluate(listcode::listcode* out) {
	switch (tp) {
		case oscript::control_flow_expr_node::type::RETURN: {
			listcode::jump_inst* jumpi = out->add_ijump();
			jumpi->to_the_end = true;
			return;
		}
		case oscript::control_flow_expr_node::type::RETURN_NONE: {
			out->add_ireturn(expr->evaluate(out));
		}
		case oscript::control_flow_expr_node::type::BREAK: {
			for (auto inst : out->instructions) {
				if (inst->none_tp == listcode::instruction::none_inst_type::WHILELOOP) {
					// after while loop
					listcode::whileloop_inst* whinst = (listcode::whileloop_inst*) inst.Data();
					assert(whinst->loop_end && "break is outside while loop");
				}
			}
			break;
		}
	}
	assert(0 && "unhandled control flow");
}

void oscript::get_expr_node::evaluate(listcode::listcode* out) {

}

void oscript::creation_expr_node::evaluate(listcode::listcode* out) {
	if (raw) {
		out->add_icreate(true);
	} else {
		out->add_icreate(false, name.id, type.id);
	}
}

void oscript::call_expr_node::evaluate(listcode::listcode* out) {

}

void oscript::boolean_ariphmetic_expr_node::evaluate(listcode::listcode* out) {

}

void oscript::ariphmetic_expr_node::evaluate(listcode::listcode* out) {

}

void oscript::creation_expr_node::evaluate(listcode::listcode* out) {}